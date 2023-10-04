#ifndef _MARCO_POLO_COMMON_HPP_
#define _MARCO_POLO_COMMON_HPP_

#include <string>

// Map the input string to an IPv4 address
in_addr_t string_to_ip(char* pStr)
{
  in_addr_t ip = 0;
  if ((ip = inet_addr(pStr)) != (in_addr_t)(-1)) return ip;
  return 0;
}

// Convert the input string to a network endian port number
in_port_t string_to_port(char* pStr)
{
  return htons(std::atoi(pStr));
}

// Map the input string to its corresponding protocol id
// tcp -> SOCK_STREAM
// udp -> SOCK_DGRAM
int string_to_protocol(char* pStr)
{
  std::string tcp = "tcp";
  std::string udp = "upd";
  if (pStr == tcp) return SOCK_STREAM;
  if (pStr == udp) return SOCK_DGRAM;
  return 0;
}

#endif // #ifndef _MARCO_POLO_COMMON_HPP_
