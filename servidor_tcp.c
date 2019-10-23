
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

// Function designed for chat between client and server.
void chat(int soquete) {

    int message_length, case_aux;
    char message[80], message_aux;

    while(true) {
        memset(message, 0, 80);

        // read the message from client and copy it in buffer
        read(soquete, message, sizeof(message));
        // print buffer which contains the client contents

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

        // and send that buffer to client
        write(soquete, message, sizeof(message));
    }
}

// Driver function
int main(int argc, char *argv[]) {

    int soquete, connfd, len, port = 8080;
    struct sockaddr_in server, client;

    // socket create and verification
    soquete = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));

    // assign IP, port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    bind(soquete, (struct sockaddr*)&server, sizeof(server));

    // Now server is ready to listen and verification
    listen(soquete, 5);

    len = sizeof(client);
    printf("alo3\n");
    // Accept the data packet from client and verification
    connfd = accept(soquete, (struct sockaddr*)&client, &len);
    printf("alo2\n");
    // Function for chatting between client and server
    chat(connfd);
    printf("alo\n");

    // After chatting close the socket
    close(soquete);
}
