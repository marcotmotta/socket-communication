#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {

    int soquete, n, port = 5000, aux_index;
    char received[80], message[80];
    struct sockaddr_in server;

    fgets(message, sizeof(message), stdin);
    aux_index = strlen(message) - 1;
    if(message[aux_index] == '\n') {
        message[aux_index] = '\0';
    }

    soquete = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    connect(soquete, (struct sockaddr *)&server, sizeof(server));
    sendto(soquete, message, 81, 0, (struct sockaddr*)NULL, sizeof(server));

    recvfrom(soquete, received, sizeof(received), 0, (struct sockaddr*)NULL, NULL);

    printf("%s", received);

    close(soquete);
} 