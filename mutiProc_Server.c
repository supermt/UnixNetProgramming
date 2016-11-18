#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

void process_cli(int connfd, struct sockaddr_in client)
{
int num;
char  recvbuf[MAXDATASIZE], sendbuf[MAXDATASIZE];
printf("Yougot a connection from %s. ",inet_ntoa(client.sin_addr) );
while(1){
  num = recv(connfd,recvbuf, MAXDATASIZE,0);
  printf("%s\t %d Bytes\n", recvbuf,num);
  if (num<=MAXDATASIZE) break;
}
if (num == 0)
{
close(connfd);
printf("Client disconnected.\n");
return;
}

struct tm *ptr;
time_t It;
It=time(NULL);
ptr = localtime(&It);
char * timeStr[20];
sprintf(timeStr,"\t%d:%d:%d\n",ptr->tm_hour,ptr->tm_min,ptr->tm_sec);

sprintf(sendbuf,"%s",recvbuf);
strcat(sendbuf,timeStr);
puts(sendbuf);
send(connfd,sendbuf,strlen(sendbuf),0);
close(connfd);
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
      process_cli(connfd, client);
      exit(0);
    }
    else {
      printf("fork()error\n");
      exit(0);
    }
  }
  close(listenfd);
}
