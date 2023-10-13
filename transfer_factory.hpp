#ifndef _MARCO_POLO_TRANSFER_FACTORY_HPP_
#define _MARCO_POLO_TRANSFER_FACTORY_HPP_

#include <memory>
#include "transfer_interface.hpp"
#include "tcp_transfer.hpp"
#include "udp_transfer.hpp"

std::unique_ptr<ITransfer> string_to_protocol(std::string const& str)
{
  if (str == "tcp") return std::unique_ptr<ITransfer>(new TCPTransfer);
  if (str == "udp") return std::unique_ptr<ITransfer>(new UDPTransfer);
  return nullptr;
}

#endif // #ifndef _MARCO_POLO_TRANSFER_FACTORY_HPP_


