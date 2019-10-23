
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define SA struct sockaddr

void chat(int sockfd) {

    int n, message_length, case_aux;
    char message[80], message_aux;

    while(true) {
        memset(message, 0, sizeof(message));
        printf("Enter the string : ");
        n = 0;
        while ((message[n++] = getchar()) != '\n')
            ;

        message_length = strlen(message);

        for(int i = 0; i < (message_length/2); i++) {
            message_aux = message[i];
            message[i] = message[message_length - i - 1];
            message[message_length - i - 1] = message_aux;
        }

        case_aux = 0;
        while (message[case_aux] != '\0') {
            if (message[case_aux] >= 'a' && message[case_aux] <= 'z') {
                message[case_aux] = message[case_aux] - 32;
            } else if (message[case_aux] >= 'A' && message[case_aux <= 'Z']) {
                message[case_aux] = message[case_aux] + 32;
            }
            case_aux++;
        }

        write(sockfd, message, sizeof(message));
        memset(message, 0, sizeof(message));
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
    memset(&server, 0, sizeof(server));

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
    chat(sockfd);

    // close the socket
    close(sockfd);
} 
