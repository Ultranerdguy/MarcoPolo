#include <iostream>
#include <thread>
#include <memory>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.hpp"
#include "epoll.hpp"
#include "socket.hpp"

void usage_and_quit()
{
  std::cout << "Usage: polo <port> <tcp|udp>\n";
  exit(0);
}

Socket server_socket;
EPoll g_epoll;
std::vector<std::unique_ptr<Socket>> g_sockets;

void Accept_Connection()
{
  std::cout << "Started accept thread\n\tServer socket fd " << (int)server_socket << '\n';
  sockaddr_in client;
  epoll_event eventTemplate;
  eventTemplate.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP | EPOLLERR;
  while (server_socket >= 0)
  {
    auto& socket = g_sockets.emplace_back(new Socket(server_socket.Accept(client)));
    std::cout << "Accepted connection " << (int)(*socket) << '\n';
    g_epoll.Add(*socket, eventTemplate, socket.get());
  }
}

void Read_Socket()
{
  std::cout << "Started read thread\n\tEPoll fd " << (int)g_epoll << '\n';
  while (g_epoll >= 0)
  {
    auto events = g_epoll.Wait(8, 10);
    for (auto& event : events)
    {
      if (event.events & EPOLLIN)
      {
        Socket& sock = *reinterpret_cast<Socket*>(event.data.ptr);
        std::cout << (int)sock << '\t';
        std::cout << sock.Read() << '\n';
      }
      if (event.events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR))
      {
        Socket& sock = *reinterpret_cast<Socket*>(event.data.ptr);
        std::cout << (int)sock << " Closing\n";
        g_epoll.Remove(sock);
        sock.Close();
      }
    }
  }
}

int main(int argc, char** argv)
{
  if (argc < 3) usage_and_quit();

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = string_to_port(argv[1]);
  
  int proto;
  if ((proto = string_to_protocol(argv[2])) == 0) usage_and_quit();
  
  server_socket = std::move(Socket(server.sin_family, proto, 0));
  server_socket.Bind(server);
  server_socket.Listen(4);

  std::thread acceptThread(Accept_Connection);
  std::thread readThread(Read_Socket);

  acceptThread.join();
  readThread.join();

  return 0;
}
