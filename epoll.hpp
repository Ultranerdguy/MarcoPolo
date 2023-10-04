#ifndef _MARCO_POLO_EPOLL_HPP_
#define _MARCO_POLO_EPOLL_HPP_

#include <vector>
#include <sys/epoll.h>

class EPoll
{
public:
  EPoll(int flags = 0)
  {
    m_epoll_fd = epoll_create1(flags);
  }
  ~EPoll()
  {
    Close();
  }
  // No copy constructor
  EPoll(EPoll const&) = delete;
  EPoll& operator=(EPoll const&) = delete;
  // Move will swap, so move source will destroy old data
  EPoll(EPoll&& mv)
  {
    *this = std::move(mv);
  }
  EPoll& operator=(EPoll&& mv)
  {
    std::swap(m_epoll_fd, mv.m_epoll_fd);
    return *this;
  }

  void Close()
  {
    close(m_epoll_fd);
    m_epoll_fd = 0;
  }

  operator int() const
  {
    return m_epoll_fd;
  }

  void Add(int fd, epoll_event event, void* pData = nullptr)
  {
    if (pData) event.data.ptr = pData;
    else event.data.fd = fd;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event);
  }
  
  void Modify(int fd, epoll_event event, void* pData = nullptr)
  {
    if (pData) event.data.ptr = pData;
    else event.data.fd = fd;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &event);
  }
  
  void Remove(int fd)
  {
    epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
  }

  std::vector<epoll_event> Wait(int maxEvents, int timeout)
  {
    std::vector<epoll_event> eventBuffer;
    eventBuffer.resize(maxEvents);
    eventBuffer.resize(epoll_wait(m_epoll_fd, eventBuffer.data(), maxEvents, timeout));
    return eventBuffer;
  }

private:
  int m_epoll_fd = 0;
};

#endif // #ifndef _MARCO_POLO_EPOLL_HPP_
