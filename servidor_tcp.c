
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define SA struct sockaddr

// Function designed for chat between client and server.
void chat(int sockfd) {

    int n;
    char message[80];

    while(true) {
        memset(message, 0, 80);

        // read the message from client and copy it in buffer
        read(sockfd, message, sizeof(message));
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", message);
        memset(message, 0, 80);
        n = 0;
        // copy server message in the buffer
        while ((message[n++] = getchar()) != '\n')
            ;

        // and send that buffer to client
        write(sockfd, message, sizeof(message));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", message, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Driver function
int main(int argc, char *argv[]) {

    int sockfd, connfd, len, port = 8080;
    struct sockaddr_in server, client;

    // socket create and verification
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
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&server, sizeof(server))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(client);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&client, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // Function for chatting between client and server
    chat(connfd);

    // After chatting close the socket
    close(sockfd);
}
