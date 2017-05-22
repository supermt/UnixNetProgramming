#include      "unp.h"
#include  <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 5
#define MAXDATASIZE 1000

typedef struct arguments {
  int connfd;
}threadArg;

void process_cli(int s)
{
  ssize_t size = 0;
  char buffer[1024];    /* 数据的缓冲区 */
  
  for(;;){/* 循环处理过程 */
    /* 从套接字中读取数据放到缓冲区buffer中 */
    size = Read(s, buffer, 1024);
    if(size == 0){/* 没有数据 */
    return;
    }
    /* 构建响应字符，为接收到客户端字节的数量 */
    time_t now;
    time (&now);
    snprintf(buffer, sizeof(buffer), "%s", ctime(&now));
    puts(buffer);
    Write(s, buffer, strlen(ctime(&now)) + 1);
  }
}

void *threadFlow(void* arg)
{
  threadArg * info = (threadArg*)arg;
  process_cli(info->connfd);
  pthread_detach(pthread_self());
  free (arg);
  pthread_exit(NULL);
}
int main(){
  int listenfd,connfd;
  pthread_t tid;
  struct sockaddr_in server;
  struct sockaddr_in client;
  int len;
  threadArg * arg;
  
  listenfd = Socket(AF_INET,SOCK_STREAM,0);
  if (listenfd == -1){
    printf("Socket Create Error\n");
    exit(1);
  }
  
  
  int opt =SO_REUSEADDR;
  setsockopt(listenfd,SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  bzero(&server,sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr.s_addr= htonl (INADDR_ANY);
  
  Bind(listenfd,(struct sockaddr *)&server, sizeof(server));
    
    Listen(listenfd,BACKLOG);
    
    
    len = sizeof(client);
  while(1)
  {
    connfd =Accept(listenfd,(struct sockaddr *)&client,(socklen_t *)&len);
    if (connfd == -1) {
      printf("Accept Error\n");
      close(listenfd);
      exit(1);
    }
    //创建线程参数
    arg = (threadArg *)malloc(sizeof(threadArg));
    arg->connfd =connfd;
    
    if(pthread_create(&tid, NULL, threadFlow, (void*)arg)) {
      printf("Thread Create Error\n");
      close(listenfd);
      exit(1);
    }
  }
  close(listenfd);
}
