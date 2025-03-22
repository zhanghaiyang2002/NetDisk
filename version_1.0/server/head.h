#include<func.h>

enum workStatus{
      FREE,
      BUSY
};

typedef struct{
      pid_t pid;
      int status;
      int pipeFd;
}processData_t;

typedef struct{
      int size;
      char buf[1000];
}train_t;

//创建子进程
int make_child(processData_t *workerList,int childNum);

//发送描述符
int sendFD(int Pipe,int fd);
//接收描述符
int recvFD(int Pipe,int *fd);

//子进程处理任务
int handleEvent(int pipeFd);

//初始化tcp
int initTcp(char*ip,char*port,int *sfd);

//jiang描述符加入到epfd的监控队列中
int epoll_add(int epfd,int fd);

//传输文件
int transFile(int newFd);