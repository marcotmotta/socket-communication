#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

/**
  *  O esqueleto do código abaixo foi inspirado na seguinte implementação:
  *  https://www.geeksforgeeks.org/udp-client-server-using-connect-c-implementation/
  *  Vale ressaltar que apesar da lógica ter sido aproveitada o código é estritamente
  *  diferente, além de realizar ações e produzir resultados totalmente distintos.
  */

int main(int argc, char *argv[]) {

    int soquete, n, port, aux_index;
    char received[81], message[81], *ip;
    struct sockaddr_in server;

    ip = strdup(argv[1]);
    port = atoi(argv[2]);

    fgets(message, sizeof(message), stdin);

    aux_index = strlen(message) - 1;
    if(message[aux_index] == '\n') {
        message[aux_index] = '\0';
    }

    soquete = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    connect(soquete, (struct sockaddr *)&server, sizeof(server));
    sendto(soquete, message, 81, 0, (struct sockaddr*)NULL, sizeof(server));

    recvfrom(soquete, received, sizeof(received), 0, (struct sockaddr*)NULL, NULL);

    printf("%s", received);

    close(soquete);

}