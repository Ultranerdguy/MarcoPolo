#include <iostream>
#include "transfer_factory.hpp"

void usage_and_quit()
{
  std::cout << "Usage: polo <port> <tcp|udp>\n";
  exit(0);
}

int main(int argc, char** argv)
{
  if (argc < 3) usage_and_quit();

  ServerInfo serverInfo{"", static_cast<in_port_t>(std::atoi(argv[1]))};

  auto transferObj = string_to_protocol(argv[2]);
  if (!transferObj) usage_and_quit();

  transferObj->StartServer(serverInfo);
  
  return 0;
}
