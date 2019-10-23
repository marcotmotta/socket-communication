#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

// Driver code
int main() {

    int soquete, n, port = 5000, aux_index;
    char received[80], message[80];
    struct sockaddr_in server;

    fgets(message, sizeof(message), stdin);
    aux_index = strlen(message) - 1;
    if(message[aux_index] == '\n') {
        message[aux_index] = '\0';
    }

    // clear server
    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    // create datagram socket
    soquete = socket(AF_INET, SOCK_DGRAM, 0);

    // connect to server
    connect(soquete, (struct sockaddr *)&server, sizeof(server));

    // request to send datagram
    // no need to specify server address in sendto
    // connect stores the peers IP and port
    sendto(soquete, message, 81, 0, (struct sockaddr*)NULL, sizeof(server));

    // waiting for response
    recvfrom(soquete, received, sizeof(received), 0, (struct sockaddr*)NULL, NULL);

    printf("%s", received);

    // close the descriptor
    close(soquete);
} 