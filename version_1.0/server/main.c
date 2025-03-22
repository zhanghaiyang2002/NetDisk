#include"head.h"

int main(int argc,char *argv[])
{
      if(argc!=4)
      {
         printf("./process_pool IP PORT PROCESS_NUM\n");
         return -1;
      }
      int childNum=atoi(argv[3]);
      processData_t *workerList=(processData_t *)calloc(childNum,sizeof(processData_t));
      make_child(workerList,childNum);
      int socketFd;
      initTcp(argv[1],argv[2],&socketFd);

      int epfd=epoll_create(1);
      epoll_add(epfd,socketFd);

      int i;
      for(i=0;i<childNum;i++)
      {
            epoll_add(epfd,workerList[i].pipeFd);
      }

      int readyFdNum,newFd;
      struct epoll_event *evs=(struct epoll_event *)calloc(childNum+1,sizeof(struct epoll_event));

      struct sockaddr_in clientAddr;
      int addrLen,j,finishFlag;

      while(1)
      {
            readyFdNum=epoll_wait(epfd,evs,childNum+1,-1);//evs填充就绪的事件
            for(i=0;i<readyFdNum;i++)
            {
                  if(socketFd==evs[i].data.fd)
                  {
                        addrLen=sizeof(clientAddr);
                        newFd=accept(socketFd,(struct sockaddr*)&clientAddr,&addrLen);
                        printf("task coming ip=%s,port=%d\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
                        for(j=0;j<childNum;j++)
                        {
                              if(workerList[j].status==FREE)
                              {
                                    sendFD(workerList[j].pipeFd,newFd);
                                    workerList[j].status=BUSY;
                                    printf("%d send to %d child process\n",newFd,workerList[j].pid);
                                    break;
                              }
                        }
                        close(newFd);
                  }else{
                        for(j=0;j<childNum;j++)
                        {
                              if(workerList[j].pipeFd==evs[i].data.fd)
                              {
                                    read(workerList[j].pipeFd,&finishFlag,sizeof(int));
                                    workerList[j].status=FREE;
                                    printf("%d child process finish task\n",workerList[j].pid);
                                    break;
                              }
                        }
                  }
            }
      }
}