#include"func.h"
int epoll_add(int epfd,int fd)
{
      struct epoll_event event;
      event.data.fd=fd;
      event.events=EPOLLIN;//监控读事件
      int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
      ERROR_CHECK(ret,-1,"epoll_ctl");
      return 0;
}