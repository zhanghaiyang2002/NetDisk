#include"head.h"

int sendFD(int Pipe,int fd)
{
      struct msghdr msg;
      memset(&msg,0,sizeof(msg));
      struct iovec iov[2];
      char buf[10]="hello";
      iov[0].iov_base=buf;//iov_base指向缓冲区
      iov[0].iov_len=5;//iov_len指定缓冲区大小
      msg.msg_iov=iov;//数据缓冲区
      msg.msg_iovlen=1;//缓冲区个数

      //填充控制信息
      size_t cmsgLen=CMSG_LEN(sizeof(int));
      struct cmsghdr *pcmsg=(struct cmsghdr*)calloc(1,cmsgLen);
      pcmsg->cmsg_len=cmsgLen;
      pcmsg->cmsg_level=SOL_SOCKET;
      pcmsg->cmsg_type=SCM_RIGHTS;
      *(int*)CMSG_DATA(pcmsg)=fd;

      msg.msg_control=pcmsg;
      msg.msg_controllen=cmsgLen;

      int ret=sendmsg(Pipe,&msg,0);
      ERROR_CHECK(ret,-1,"sendmsg");
      return 0;

}

int recvFD(int Pipe,int *fd)
{
      struct msghdr msg;
      memset(&msg,0,sizeof(msg));
      struct iovec iov[1];
      char buf[6]={0};
      iov[0].iov_base=buf;
      iov[0].iov_len=5;
      msg.msg_iov=iov;
      msg.msg_iovlen=1;

      size_t cmsgLen=CMSG_LEN(sizeof(int));
      struct cmsghdr *pcmsg=(struct cmsghdr*)calloc(1,cmsgLen);
      pcmsg->cmsg_len=cmsgLen;
      pcmsg->cmsg_level=SOL_SOCKET;
      pcmsg->cmsg_type=SCM_RIGHTS;

      msg.msg_control=pcmsg;
      msg.msg_controllen=cmsgLen;

      int ret=recvmsg(Pipe,&msg,0);
      ERROR_CHECK(ret,-1,"recvmsg");
      *fd=*(int*)CMSG_DATA(pcmsg);
      return 0;
}