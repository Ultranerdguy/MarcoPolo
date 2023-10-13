#include <iostream>
#include <string>
#include <ctime>
#include "transfer_factory.hpp"

void usage_and_quit()
{
  std::cout << "Usage: marco <ip> <port> <tcp|udp>\n";
  exit(0);
}

int main(int argc, char** argv)
{
  if (argc < 4) usage_and_quit();

  ServerInfo targetInfo{argv[1], static_cast<in_port_t>(std::atoi(argv[2]))};

  auto transferObj = string_to_protocol(argv[3]);
  if (!transferObj) usage_and_quit();

  transferObj->Connect(targetInfo);

  while (true)
  {
    time_t t = time(nullptr);
    tm* tm = localtime(&t);
    
    char format[] = "%04d-%02d-%02d %02d:%02d:%02d";
    char buffer[128];
    snprintf(buffer, 128, format, tm->tm_year+1900, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    
    std::cout << "Sending message " << buffer << '\n';
    transferObj->Send(buffer);
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
  
  return 0;
}
