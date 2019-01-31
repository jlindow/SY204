/* 
   Author: MIDN Jacob A. Lindow
 
   This is a sample program that sets up a socket as a client
   It does not perform error checking!

   This program takes two arguments:
      1. IP Address
      2. Port Number
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

char buffer[1024];
struct sockaddr_in address;     // sockaddr_in structure needed for connect()

if(argc!=3){                    // Ensures the user supplies the correct arguments
        printf("Usage: ./client IPAdress PortNumber\n");
        exit(0);
}

int sockfd = socket(AF_INET, SOCK_STREAM, 0);   // Create a socket (path) for communication
if(sockfd == -1) {
    perror("Socket error");
    exit(0); }


int port = atoi(argv[2]);
address.sin_family = AF_INET;                   // Sets the IP address Family to IPv4
address.sin_port = htons(port);                 // Converts the port from Host Byte Order (human readable) to Network Byte Order (machine readable)

if (inet_pton(AF_INET, argv[1], &address.sin_addr) != 1){         // Converts a 32-bit IP Address from human readbale to machine readable form
    printf("Invalid IP address....");
    exit(0); }


if( (connect(sockfd, (struct sockaddr*)&address, sizeof(address))) == -1){   // Connects to the IP Address and Port specified by the user
    perror("Error Connecting");}

while(1){
        //Read from socket into buffer
        int num = recv(sockfd, buffer, 1024, 0);
        if (num == -1){
          perror("Error reading from socket");}

        //Write to STDOUT what was received
        if( (write(1, buffer, num) == -1)){
          perror("Error writing to STDOUT");}
        
        //Read whatever is typed form STDIN
        num = read(0, buffer, 1024);
        if(num == -1){
          perror("Error reading from STDIN");}

        //Send buffer contents to socket
        if( (send(sockfd, buffer, num, 0)) == -1){ 
          perror("Error sending");}
        
}

close(sockfd);

return(0);
}
