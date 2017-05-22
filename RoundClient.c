#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFSIZE 1024


int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoserver;
  unsigned int echolen;

  if (argc != 4) {
    printf("USAGE: TCPecho <server_ip> <word> <port>\n");
    exit(1);
  }

  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("Failed to create socket\n");
    exit(1);
  }


  memset(&echoserver, 0, sizeof(echoserver));
  echoserver.sin_family = AF_INET;
  echoserver.sin_addr.s_addr = inet_addr(argv[1]);
  echoserver.sin_port = htons(atoi(argv[3]));

  if (connect(sock,(struct sockaddr *) &echoserver,sizeof(echoserver)) < 0) {
    printf("Failed to connect with server\n");
    exit(1);
  }


while (1) {
  int received = 0;
  char buffer[BUFFSIZE];
  /* Send the word to the server */
   echolen = strlen(argv[2]);
   if (send(sock, argv[2], echolen, 0) != echolen) {
     printf("Mismatch in number of sent bytes\n");
     exit(1);
   }
   printf("Received: ");
   while (received < echolen) {
       int bytes = 0;
       if ((bytes = recv(sock, buffer, BUFFSIZE-1, 0)) < 1) {
         printf("Failed to receive bytes from server\n");
         exit(1);
     }
       received += bytes;
       buffer[bytes] = '\0';  //防止空字符串
       printf(buffer);
   }
   printf("\n");
   sleep(1);
}

  close(sock);
  exit(0);
}
