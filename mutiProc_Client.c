#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include  <time.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>

#define MAXDATASIZE 100

void func_waitpid(int signo) {
    pid_t pid;
    int stat;
    while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) {
        printf( "child %d exit\n", pid );
    }
    return;
}

void process(FILE *fp,char * argv[])
{
  int sockfd;
  struct hostent  *he;
  struct sockaddr_in  server;

  if((he=gethostbyname(argv[1]))==NULL){
    printf("gethostbyname() error\n");
    exit(1);
  }

  if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1){
    printf("socket()error\n");
    exit(1);
  }

  bzero(&server,sizeof(server));
  server.sin_family =AF_INET;
  server.sin_port=htons(atoi(argv[2]));
  server.sin_addr= *((struct in_addr *)he->h_addr);

  if(connect(sockfd,(struct sockaddr *)&server,sizeof(server))==-1){
    printf("connect() error\n");
    exit(1);
  }

  char sendBuff[MAXDATASIZE],recvBuff[MAXDATASIZE];
    // struct tm *ptr;
    // time_t It;
    // It=time(NULL);
    // ptr = localtime(&It);
    // char * timeStr[20];
    // sprintf(timeStr,"\t%d:%d:%d\n",ptr->tm_hour,ptr->tm_min,ptr->tm_sec);
    //
    // int received,echolen=strlen(sendBuff);

  sprintf(sendBuff,"hello %d",getpid());
  int received=0,echolen=strlen(sendBuff);
  send(sockfd,sendBuff, strlen(sendBuff),0);

  while (received < echolen) {
      int bytes = 0;
      if ((bytes = recv(sockfd, recvBuff, MAXDATASIZE-1, 0)) < 0) {
        printf("Failed to receive bytes from server");
        break;
    }
    received += bytes;
    recvBuff[bytes]='\0';
    printf("Server Message: %s\n",recvBuff);
  }

  close(sockfd);
}


int main(int argc,char *argv[])
{

  //argv[1]:  目的IP
  //argv[2]:  目的端口
  //argv[3]:  同时并发数量
  int pid = 0;
  if (argc != 4){
    printf("USAGE: TCPecho <server_ip> <port> <loop_count>\n\n");
  }
  int count = atoi(argv[3]);
  // printf("%d\n", count);
  for (int i = 0; i < count; i++) {
    pid = fork();
    if (pid>0){
      continue;
    }
    if (pid==0) {
      process(stdin,argv);
      exit(0);
    }
    else {
      printf("fork()error\n");
      exit(0);
    }
  }

}
