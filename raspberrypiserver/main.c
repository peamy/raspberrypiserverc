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
int main(int argc, char** argv) {
    int sockfd, clisockfd, portno;
    char * start = "hello";
    char * end = "bye";
    socklen_t clilen;
    char buffer[256];
    char contentBuffer[255];
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
    portno = 5000;
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

    while (strcmp(end, contentBuffer) !=0)
    {
        bzero(buffer,256);
        bzero(contentBuffer,255);
        /* If connection is established then start communicating */
        n = read( clisockfd,buffer,255 );
        
        printf("I got : %s\n", buffer);
        
        write(clisockfd, buffer, strlen(buffer));
        
        if (n < 0)
        {
            perror("ERROR reading from socket");
            return(1);        
        }
    }
    close(sockfd);
    return 0;
}

