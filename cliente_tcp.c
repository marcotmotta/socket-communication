
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define SA struct sockaddr

void func(int sockfd) {

    char message[80];
    int n;
    for (;;) {
        bzero(message, sizeof(message));
        printf("Enter the string : ");
        n = 0;
        while ((message[n++] = getchar()) != '\n')
            ;
        write(sockfd, message, sizeof(message));
        bzero(message, sizeof(message));
        read(sockfd, message, sizeof(message));
        printf("From Server : %s", message);
        if ((strncmp(message, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main(int argc, char *argv[]) {

    int sockfd, connfd, port = 8080;
    struct sockaddr_in server;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&server, sizeof(server));

    // assign IP, port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&server, sizeof(server)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
} 
