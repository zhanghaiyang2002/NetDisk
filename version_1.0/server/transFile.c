#include"head.h"

int transFile(int newFd)
{
      int ret;
      train_t t;
      t.size=strlen("file1");
      strcpy(t.buf,"file1");
      send(newFd,&t,4+t.size,0);

      int fd=open("file1",O_RDONLY);
      ERROR_CHECK(fd,-1,"open");

      while((t.size=read(fd,t.buf,sizeof(t.buf))))
      {
            ret=send(newFd,&t,4+t.size,0);
            if(ret==-1)
            {
                  goto end;
            }
      }
      t.size=0;
      send(newFd,&t,4+t.size,0);
end:
      close(fd);
      return 0;
}

//循环接收，要接多少字节，接完那么多才会返回
int recvn(int newFd,void *pStart,int len)
{
      int total=0,ret;
      char *p=(char*)pStart;
      while(total<len)
      {
            ret=recv(newFd,p+total,len-total,0);
            ERROR_CHECK(ret,-1,"recv");
            if(0==ret)
            {
                  return -1;
            }
            total+=ret;
      }
      return 0;
}

//接收文件
int recvFile(int new_fd)
{
    //先接文件名,每次先接4个字节
    int recvLen;
    char buf[1000]={0};
    recvn(new_fd,&recvLen,4);
    recvn(new_fd,buf,recvLen);
    printf("buf=%s\n",buf);
    int fd=open(buf,O_WRONLY|O_CREAT,0666);
    ERROR_CHECK(fd,-1,"open");

    //再接文件内容
    while(1)
    {
        recvn(new_fd,&recvLen,4);//接火车车厢
        if(recvLen>0)
        {
            recvn(new_fd,buf,recvLen);//接recvLen长度的数据
            write(fd,buf,recvLen);//把接收到的内容写入文件
        }else{
            break;
        }
    }
    close(fd);//关闭文件
    return 0;
}