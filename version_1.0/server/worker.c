#include"head.h"

int make_child(processData_t *workerList,int childNum)
{
      int i;
      int fds[2];
      pid_t pid;
      for(i=0;i<childNum;i++)
      {
            socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
            pid=fork();
            if(pid==0)//子进程
            {
                  close(fds[1]);
                  handleEvent(fds[0]);
            }
            close(fds[0]);
            workerList[i].pid=pid;
            workerList[i].status=FREE;
            workerList[i].pipeFd=fds[1];
      }
      return 0;
}

int handleEvent(int pipeFd)
{
      int newFd,finishFlag=1;
      while(1)
      {
            recvFD(pipeFd,&newFd);
            printf("recv parent task %d\n",newFd);
            transFile(newFd);
            close(newFd);
            write(pipeFd,&finishFlag,sizeof(int));
      }
}