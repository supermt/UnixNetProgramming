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

typedef struct arguments {
  FILE * fp;
  char ip[20];
  int port;
}threadArg;

typedef struct arguments2 {
  int client_fd;
}Arg;

void processSend(void * arg){
  Arg * argument = (Arg*)arg;
  //printf("%d\n",argument->client_fd);
}

void processRecieve(){}

void process(void * arg)
{
  int sockfd;
  struct hostent  *he;
  struct sockaddr_in  server;
  threadArg * arguments = (threadArg*) arg;
  if((he=gethostbyname(arguments->ip))==NULL){
    printf("gethostbyname() error\n");
    exit(1);
  }

  if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1){
    printf("socket()error\n");
    exit(1);
  }

  bzero(&server,sizeof(server));
  server.sin_family =AF_INET;
  server.sin_port=htons(arguments->port);
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

  sprintf(sendBuff,"hello");
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
//sleep(3);
  close(sockfd);
}


int main(int argc,char *argv[])
{
  //argv[1]:  目的IP
  //argv[2]:  目的端口
  //argv[3]:  同时并发数量
  Arg * arg;
  // if (argc != 4){
  //   printf("USAGE: TCPecho <server_ip> <port> <loop_count>\n\n");
  // }
  //
  // int count = atoi(argv[3]);
  // pthread_t tids[count];
  pthread_t tidInput;
  pthread_t tidOutput;

  arg = (Arg*)malloc(sizeof(Arg));

  //准备连接服务器
  int sockfd;
  struct hostent  *he;
  struct sockaddr_in  server;

  // printf("%s\n",argv[1]);
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

  arg->client_fd = 1;

  if (pthread_create(tidInput, NULL, processSend, (void*)arg)) printf("Thread Create Error");



  //这个是原来的多线程并发请求
  // printf("%d\n", count);
  // for (int i = 0; i < count; i++) {
  //   arg = (threadArg *)malloc(sizeof(threadArg));
  //   arg->fp = stdin;
  //   sprintf(arg->ip,"%s",argv[1]);
  //   arg->port = atoi(argv[2]);
  //   if(pthread_create(tids+i, NULL, process, (void*)arg)) printf("Thread Create Error");
  // }
  // pthread_join(tids[0],NULL);
  // pthread_join(tids[0],NULL);
  // pthread_join(tids[0],NULL);
  // for (int i = 0; i < count; i++) {
  //   pthread_join(tids+i,NULL);
  // }

}
