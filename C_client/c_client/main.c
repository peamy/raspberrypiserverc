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
#include <wiringPi.h>

int sockfd;
const int pinLed = 3;
const int pinButton = 4;

void *threadFuncButton(void *arg) {
 int state = 0;
 int n;
 int delay = 10000;
 while(sockfd != 0) {
	if(state == 0 && digitalRead(pinButton) == 0) {
		state = 1;
		n = write(sockfd, "a\n", 2);
		usleep(delay);
	}
	else if(state == 1 && digitalRead(pinButton) == 1) {
		state = 0;
		n = write(sockfd, "b\n", 2);
		usleep(delay);
	}

        if (n < 0)
        {
           perror("ERROR writing to socket");
           exit(1);
        }
   }
   digitalWrite(pinLed, LOW);
   return;

}

void *threadFunc(void *arg) {
    char input[256];
    int n;
    while(sockfd != 0) {
        bzero(input, 256);
        n = read(sockfd, input, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
	if((strncmp(input, "KNOP IS AAN", 11))==0) {
		digitalWrite(pinLed, HIGH);
	}
	else if((strncmp(input, "KNOP IS UIT", 11))==0) {
		digitalWrite(pinLed, LOW);
	}

    }
    digitalWrite(pinLed, LOW);
    return;
}

int main(int argc, char *argv[])
{

   int state = 0;
   wiringPiSetupGpio();
   pinMode(pinButton, INPUT);
   pinMode(pinLed, OUTPUT);
   digitalWrite(pinLed, LOW);
   printf("\nPins set, press enter to continue..");
   getchar();
   int portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;

   if (argc < 2){
     printf("Ip required!\n");
     return 0;
   }

   if (argc >= 3){
      portno = atoi(argv[2]);
   } 
   else
   { portno = 1000;}
   printf("Trying to connect to %s:%d", argv[1], portno);
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0)
   {
      perror("ERROR opening socket");
      exit(1);
   }
   server = gethostbyname(argv[1]);
   
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
   pthread_t pth_button;
   pthread_create(&pth_button, NULL, threadFuncButton, "dummydata");
   write(sockfd, "ab\n", 3);
   printf("\nPress any key to exit...");
   getchar();
   printf("Closing...\n");
   write(sockfd, "b\n", 2);
   close(sockfd);
   sockfd = 0;
   
   digitalWrite(pinLed, LOW);
   return 0;
}

