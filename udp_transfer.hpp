#ifndef _MARCO_POLO_UDP_TRANSFER_HPP_
#define _MARCO_POLO_UDP_TRANSFER_HPP_

#include "transfer_interface.hpp"
#include "epoll.hpp"
#include "socket.hpp"

class UDPTransfer : public ITransfer
{
public:
  UDPTransfer();
  virtual void StartServer(ServerInfo const& info) override;
  virtual void Connect(ServerInfo const& info) override;
  virtual void Send(std::string const& msg) override;

private:
  EPoll m_epoll;
  Socket m_socket;
};

UDPTransfer::UDPTransfer()
  : m_socket(AF_INET, SOCK_DGRAM, 0)
{}

void UDPTransfer::StartServer(ServerInfo const& info)
{
  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(info.m_port);
  m_socket.Bind(server);

  while (true)
  {
    sockaddr_in clientInfo;
    auto message = m_socket.ReadFrom(clientInfo);
    auto ip = ip_to_string(clientInfo.sin_addr);
    std::cout << ip << '\t' << message << '\n';
  }
}

void UDPTransfer::Connect(ServerInfo const& info)
{
  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = string_to_ip(info.m_ip.c_str());
  server.sin_port = htons(info.m_port);
  m_socket.Connect(server);

}

void UDPTransfer::Send(std::string const& msg)
{
  m_socket.Send(msg);
}

#endif // #ifndef _MARCO_POLO_UDP_TRANSFER_HPP_


