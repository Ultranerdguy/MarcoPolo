#ifndef _MARCO_POLO_TRANSFER_INTERFACE_HPP_
#define _MARCO_POLO_TRANSFER_INTERFACE_HPP_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

struct ServerInfo
{
  std::string m_ip;
  in_port_t m_port;
};

struct ITransfer
{
  virtual void StartServer(ServerInfo const& info) = 0;
  virtual void Connect(ServerInfo const& info) = 0;
  virtual void Send(std::string const& msg) = 0;
};

#endif // #ifndef _MARCO_POLO_TRANSFER_INTERFACE_HPP_
