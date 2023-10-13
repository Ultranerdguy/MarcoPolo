#ifndef _MARCO_POLO_SOCKET_HPP_
#define _MARCO_POLO_SOCKET_HPP_

#include <string>
#include <system_error>
#include <sys/socket.h>

class Socket
{
public:
  Socket() = default;
  Socket(int fd)
  {
    m_socket_fd = fd;
  }
  Socket(int domain, int type, int protocol)
  {
    m_socket_fd = socket(domain, type, protocol);
  }
  ~Socket()
  {
    Close();
  }
  // No copy constructor
  Socket(Socket const&) = delete;
  Socket& operator=(Socket const&) = delete;
  // Move will swap, so move source will destroy old data
  Socket(Socket&& mv)
  {
    *this = std::move(mv);
  }
  Socket& operator=(Socket&& mv)
  {
    std::swap(m_socket_fd, mv.m_socket_fd);
    return *this;
  }

  void Close()
  {
    close(m_socket_fd);
    m_socket_fd = 0;
  }

  operator int() const
  {
    return m_socket_fd;
  }
  
  template <typename TSOCK_ADDR>
  void Connect(TSOCK_ADDR const& serverInfo)
  {
    connect(m_socket_fd, (sockaddr*)&serverInfo, sizeof(TSOCK_ADDR));
  }

  void Send(std::string const& message, int flags = 0)
  {
    send(m_socket_fd, message.data(), message.size(), flags);
  }

  template <typename TSOCK_ADDR>
  void Bind(TSOCK_ADDR const& serverInfo)
  {
    bind(m_socket_fd, (sockaddr*)&serverInfo, sizeof(TSOCK_ADDR));
  }

  void Listen(int backlog)
  {
    listen(m_socket_fd, backlog);
  }

  template <typename TSOCK_CLIENT>
  Socket Accept(TSOCK_CLIENT& clientInfo)
  {
    socklen_t clientInfoLength = sizeof(TSOCK_CLIENT);
    return accept(m_socket_fd, (sockaddr*)&clientInfo, &clientInfoLength);
  }
  Socket Accept()
  {
    return accept(m_socket_fd, nullptr, nullptr);
  }

  std::string Read()
  {
    std::string output;
    ssize_t readLength;
    char buffer[1024];
    while ((readLength = recv(m_socket_fd, buffer, 1024, 0)))
    {
      output.append(buffer, readLength);
      if (readLength != 1024) break;
    }
    return output;
  }

  template <typename TSOCK_ADDR>
  std::string ReadFrom(TSOCK_ADDR& clientInfo)
  {
    std::string output;
    ssize_t readLength;
    char buffer[1024];
    socklen_t clientInfoLength = sizeof(TSOCK_ADDR);
    while ((readLength = recvfrom(m_socket_fd, buffer, 1024, 0, (sockaddr*)&clientInfo, &clientInfoLength)))
    {
      output.append(buffer, readLength);
      if (readLength != 1024) break;
    }
    return output;
  }
  
private:
  int m_socket_fd = 0;
};

#endif // #ifndef _MARCO_POLO_SOCKET_HPP_
