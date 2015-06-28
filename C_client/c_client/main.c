/* 
 * File:   main.c
 * Author: Remco
 *
 * Created on 28 June 2015, 15:42
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

int sockfd;

void *threadFunc(void *arg) {
    char input[256];
    int n;
    while(1) {
        bzero(input, 256);
        n = read(sockfd, input, 255);
        if (n < 0) {           
            perror("ERROR reading from socket");
            exit(1);
        }        
        printf("Server said:%s", input);
    }
    return;
}

int main(int argc, char *argv[])
{
   int portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   char buffer[256];
   
   portno = 1000;
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0)
   {
      perror("ERROR opening socket");
      exit(1);
   }
   server = gethostbyname("192.168.1.200");
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   
   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
   {
      perror("ERROR connecting");
      exit(1);
   }
   
   pthread_t pth;
   
   pthread_create(&pth, NULL, threadFunc, "dummydata");
   
   while(1) {        
        bzero(buffer,256);
        fgets(buffer,255,stdin);

        /* Send message to the server */
        n = write(sockfd, buffer, strlen(buffer));
        printf("You said: %s", buffer);

        if (n < 0)
        {
           perror("ERROR writing to socket");
           exit(1);
        }
   }   
   return 0;
}

