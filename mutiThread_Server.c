#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 5
#define MAXDATASIZE 1000

typedef struct arguments {
  int connfd;
  struct sockaddr_in client;
}threadArg;

void process_cli(int connfd, struct sockaddr_in client){
  int num=0;
  char  recvbuf[MAXDATASIZE], sendbuf[MAXDATASIZE];
  printf("Yougot a connection from %s:%d. ",inet_ntoa(client.sin_addr),client.sin_port);

  while(1){
    num = recv(connfd,recvbuf, MAXDATASIZE,0);
    printf("%s\t %d Bytes\n", recvbuf,num);
    if (num<=MAXDATASIZE) break;
  }
  if (num == 0){
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


void *threadFlow(void* arg)
{
  threadArg * info = (threadArg*)arg;
  process_cli(info->connfd,info->client);
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

  listenfd = socket(AF_INET,SOCK_STREAM,0);
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

  if (bind(listenfd,(struct sockaddr *)&server, sizeof(server)) == -1) {
    printf("Bind Error\n");
    close(listenfd);
    exit(1);
  }
  if (listen(listenfd,BACKLOG) == -1){
    printf("Listen Error\n");
    close(listenfd);
    exit(1);
  }

  len = sizeof(client);
  while(1)
  {
    connfd =accept(listenfd,(struct sockaddr *)&client,(socklen_t *)&len);
    if (connfd == -1) {
        printf("Accept Error\n");
        close(listenfd);
        exit(1);
    }
    //创建线程参数
    arg = (threadArg *)malloc(sizeof(threadArg));
    arg->connfd =connfd;
    memcpy((void*)&arg->client, &client, sizeof(client));

    if(pthread_create(&tid, NULL, threadFlow, (void*)arg)) {
      printf("Thread Create Error\n");
      close(listenfd);
      exit(1);
    }
  }
  close(listenfd);
}
