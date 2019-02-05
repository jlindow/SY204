/* 
    MIDN Jacob A. Lindow
        "socket talk" 

    This program establishes a 
     socket connection for 
     communication between 
     two IPv4 addresses. 

*/ 

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#define BUF_LEN 2048
char buffer[BUF_LEN];

int main(int argc, char *argv[])
{
    int sockfd_listen;
    int sockfd_connect;
    int port;

    struct sockaddr_in address;     // Stores IP Address of Server
    int length = sizeof(address);

    sockfd_listen = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd_listen < 0)
    {
        perror("Error:");
    }

    bzero(&address, sizeof(address));

    port = atoi(argv[1]);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htons(INADDR_ANY);
    address.sin_port = htons(port);

    if(bind(sockfd_listen, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Error:");
    }

    if(listen(sockfd_listen, 10) < 0)
    {
        perror("Error:");
    }

    if(sockfd_connect = accept(sockfd_listen, (struct sockaddr *) NULL, NULL) < 0)
    {
        perror("Error:");
    }
    
    while(1) 
    {
        int length = read(sockfd_connect, buffer, BUF_LEN); 
        write(1, buffer, length); 

        length = read(0, buffer, BUF_LEN); 
        write(sockfd_connect, buffer, length);
    }

    close(sockfd_listen);
    close(sockfd_connect);
}
