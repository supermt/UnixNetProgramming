#include      "unp.h"

#define PORT 8080
#define BACKLOG 5
#define MAXDATASIZE 1000

void func_waitpid(int signo) {
    pid_t pid;
    int stat;
    while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) {
        printf( "child %d exit\n", pid );
    }
    return;
}

void process_cli(int s)
{
  ssize_t size = 0;
  char buffer[1024];    /* 数据的缓冲区 */

  for(;;){/* 循环处理过程 */
      /* 从套接字中读取数据放到缓冲区buffer中 */
      size = read(s, buffer, 1024);
      if(size == 0){/* 没有数据 */
          return;
      }
      /* 构建响应字符，为接收到客户端字节的数量 */
      time_t now;
      time (&now);
      snprintf(buffer, sizeof(buffer), "%s", ctime(&now));
      write(s, buffer, strlen(ctime(&now)) + 1);
  }
}

int main()
{
  signal(SIGCHLD, &func_waitpid);//将func_waitpid方法注册给SIGCHLD信号，使子进程在退出时加载函数做收尾工作
  //signal(SIGCHLD,SIG_IGN);使用IGN方法无视SIGCHLD
  int  listenfd, connfd;
  pid_t  pid;
  struct  sockaddr_in  server;
  struct sockaddr_in  client;
  int  len;

  if ((listenfd =socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Creatingsocket failed.");
    exit(1);
  }

  int opt =SO_REUSEADDR;
  setsockopt(listenfd,SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  bzero(&server,sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr.s_addr= htonl (INADDR_ANY);

  if (bind(listenfd,(struct sockaddr *)&server, sizeof(server)) == -1) {
  perror("Bind()error.");
  exit(1);
  }

  if(listen(listenfd,BACKLOG)== -1){
  perror("listen() error\n");
  exit(1);
  }
  len=sizeof(client);

  while(1)
  {
    if ((connfd =accept(listenfd,(struct sockaddr *)&client,(socklen_t *)&len))==-1) {
      perror("accept() error\n");
      exit(1);
    }
    if ((pid=fork())>0){
      close(connfd);
      continue;
    }
    else if (pid==0) {
      close(listenfd);
      process_cli(connfd);
      exit(0);
    }
    else {
      printf("fork()error\n");
      exit(0);
    }
  }
  close(listenfd);
}
