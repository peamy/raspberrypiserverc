/* 
 * File:   main.c
 * Author: Remco
 *
 * Created on 27 June 2015, 12:58
 */

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include <pthread.h>

/*
 * based on http://stackoverflow.com/questions/19561941/socket-comunication-java-client-c-server
 */

const int pinGreen = 4;
const int pinRed   = 3;
int clisockfd;

void *threadFunc(void *arg) {
    int greenOn = 1;
    int redOn = 1;
    
    char input[256];
    int n;
    printf("Thread started");
    while(1) {
   	 bzero(input, 256);
  	 n = read(clisockfd, input, 255);
	printf("\x1B[\x1B[0K");
	printf("Client says: %s\n", input);

	if(strncmp(input,"stop",4)==0) {
		return 0;
	}
	else if(strncmp(input,"green",5)==0) {
		if(greenOn == 0) {
			greenOn = 1;
			digitalWrite(pinGreen, HIGH);
		}
		else {
			greenOn = 0;
			digitalWrite(pinGreen, LOW);
		}
	}
	else if(strncmp(input,"red",3)==0) {
		if(redOn == 0) {
			redOn = 1;
			digitalWrite(pinRed, HIGH);
		}
		else {
			redOn = 0;
			digitalWrite(pinRed, LOW);
		}
	}

    }
    return;
}

int main( int argc, char *argv[] )
{
    int sockfd, portno;
    socklen_t clilen;
    char buffer[256];
    char contentBuffer[255];
    char password[255];
    int passworded = 0; //0 = false, 1 = true;
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    //int optval;

    wiringPiSetupGpio();
    pinMode(pinRed, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    digitalWrite(pinGreen, HIGH);
    digitalWrite(pinRed, HIGH);


    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        return(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    printf("What port number do you want to host on? ");    
    scanf("%d", &portno);    
    
    printf("\nDo you want a password? (y/n)");
    char answer[1];
    scanf("%s", &answer);
    if(strncmp(answer, "y", 1) == 0) {
        passworded = 1;
        printf("\nWhat shall be your password? ");
        scanf("%s", password);        
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        return(1);
    }

    listen(sockfd,5);
    clilen = (socklen_t) sizeof(cli_addr);

    clisockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (clisockfd < 0) 
    {
        perror("ERROR on accept");
        return(1);
    }
    
    char question[255] = "What's the password?\n";
    while(passworded != 0) {
        bzero(buffer,256);
        bzero(contentBuffer,255);        
        
        write(clisockfd, question, strlen(question));
        read( clisockfd,buffer,255 );
        
        if(strncmp(buffer, password, strlen(password)) == 0) {
            passworded = 0;
            printf("Clear!\n");
        } 
        else {
	    printf("Client had wrong pass!");
            strcpy(question, "Wrong password, try again!\n");
        }
    }
    
    write(clisockfd, "You're in!\n", 12);

    pthread_t pth;
    pthread_create(&pth, NULL, threadFunc, &clisockfd);
 
    while (1)
    {
        bzero(buffer,256);
        
	printf("Input?: ");
	scanf("%s", buffer);
	printf("input was: %s\n", buffer);
	write(clisockfd, buffer, strlen(buffer)); 
	write(clisockfd, "!\n", 2);

        if (n < 0)
        {
            perror("ERROR reading from socket");
            return(1);        
        }

    }
    close(sockfd);
    return 0;


}
