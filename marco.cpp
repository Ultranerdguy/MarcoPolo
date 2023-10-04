#include <iostream>
#include <stdlib.h>
#include <thread>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.hpp"
#include "socket.hpp"

void usage_and_quit()
{
  std::cout << "Usage: marco <ip> <port> <tcp|udp>\n";
  exit(0);
}

int main(int argc, char** argv)
{
  if (argc < 4) usage_and_quit();
  
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = string_to_ip(argv[1]);
  server.sin_port = string_to_port(argv[2]);

  int proto;
  if ((proto = string_to_protocol(argv[3])) == 0) usage_and_quit();

  Socket server_socket(server.sin_family, proto, 0);
  server_socket.Connect(server);

  while (server_socket >= 0)
  {
    time_t t = time(nullptr);
    tm* tm = localtime(&t);
    
    char format[] = "%04d-%02d-%02d %02d:%02d:%02d";
    char buffer[128];
    snprintf(buffer, 128, format, tm->tm_year+1900, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    
    std::cout << "Sending message " << buffer;
    server_socket.Send(buffer);
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
  
  return 0;
}
