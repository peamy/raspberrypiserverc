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

/*
 * based on http://stackoverflow.com/questions/19561941/socket-comunication-java-client-c-server
 */
int main( int argc, char *argv[] )
{
    int sockfd, clisockfd, portno;
    socklen_t clilen;
    char buffer[256];
    char contentBuffer[255];
    char password[255];
    int passworded = 0; //0 = false, 1 = true;
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    //int optval;

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
            strcpy(question, "Wrong password, try again!\n");
        }
    }
    
    write(clisockfd, "You're in!\n", 12);
    while (1)
    {
        bzero(buffer,256);
        bzero(contentBuffer,255);
        /* If connection is established then start communicating */
        n = read( clisockfd,buffer,255 );
        
        printf("I got : %s\n", buffer);
        
	if(strncmp(buffer,"stop",4)==0) {
		return 0;
	}

	write(clisockfd, "(This is a test) ", 17);
        write(clisockfd, "I got your message, thanks!\n",28 );
        
        if (n < 0)
        {
            perror("ERROR reading from socket");
            return(1);        
        }

    }
    close(sockfd);
    return 0;


}
