#ifndef _MARCO_POLO_TCP_TRANSFER_HPP_
#define _MARCO_POLO_TCP_TRANSFER_HPP_

#include <thread>
#include <vector>
#include <memory>
#include "transfer_interface.hpp"
#include "epoll.hpp"
#include "socket.hpp"
#include "common.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TCPTransfer : public ITransfer
{
public:
  TCPTransfer();
  virtual void StartServer(ServerInfo const& info) override;
  virtual void Connect(ServerInfo const& info) override;
  virtual void Send(std::string const& msg) override;

private:

  void AcceptConnection();
  void ReadSocket();

  EPoll m_epoll;
  Socket m_socket;
  std::vector<std::unique_ptr<Socket>> m_sockets;
};

TCPTransfer::TCPTransfer()
  : m_socket(AF_INET, SOCK_STREAM, 0)
{}

void TCPTransfer::StartServer(ServerInfo const& info)
{
  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(info.m_port);
  m_socket.Bind(server);
  m_socket.Listen(4);

  std::thread acceptThread(&TCPTransfer::AcceptConnection, this);
  std::thread readThread(&TCPTransfer::ReadSocket, this);

  acceptThread.join();
  readThread.join();
}

void TCPTransfer::Connect(ServerInfo const& info)
{
  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = string_to_ip(info.m_ip.c_str());
  server.sin_port = htons(info.m_port);
  m_socket.Connect(server);
}

void TCPTransfer::Send(std::string const& msg)
{
  m_socket.Send(msg);
}

void TCPTransfer::AcceptConnection()
{
  sockaddr_in client;
  epoll_event eventTemplate;
  eventTemplate.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP | EPOLLERR;
  while (true)
  {
    auto& socket = m_sockets.emplace_back(new Socket(m_socket.Accept(client)));
    m_epoll.Add(*socket, eventTemplate, socket.get());
  }
}

void TCPTransfer::ReadSocket()
{
  while (true)
  {
    auto events = m_epoll.Wait(8,10);
    for (auto& event : events)
    {
      Socket& socket = *reinterpret_cast<Socket*>(event.data.ptr);
      if (event.events & EPOLLIN)
      {
        sockaddr_in clientInfo;
        auto message = socket.ReadFrom(clientInfo);
        auto ip = ip_to_string(clientInfo.sin_addr);
        std::cout << ip << '\t' << message << '\n';
      }
      if (event.events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR))
      {
        m_epoll.Remove(socket);
        socket.Close();
      }
    }
  }
}

#endif // #ifndef _MARCO_POLO_TCP_TRANSFER_HPP_
