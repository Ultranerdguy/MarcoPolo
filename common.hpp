#ifndef _MARCO_POLO_COMMON_HPP_
#define _MARCO_POLO_COMMON_HPP_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// Map the input string to an IPv4 address
in_addr_t string_to_ip(std::string str)
{
  in_addr_t ip = 0;
  if ((ip = inet_addr(str.c_str())) != (in_addr_t)(-1)) return ip;
  return 0;
}

std::string ip_to_string(in_addr ip)
{
  return inet_ntoa(ip);
}

#endif // #ifndef _MARCO_POLO_COMMON_HPP_
