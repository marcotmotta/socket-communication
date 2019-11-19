#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
  *  O esqueleto do código abaixo foi inspirado na seguinte implementação:
  *  https://www.geeksforgeeks.org/udp-client-server-using-connect-c-implementation/
  *  Vale ressaltar que apesar da lógica ter sido aproveitada o código é estritamente
  *  diferente, além de realizar ações e produzir resultados totalmente distintos.
  */

int main(int argc, char *argv[]) {

    int soquete, len, port, case_aux, message_length;
    char message[81], message_aux;
    struct sockaddr_in server, client;

    port = atoi(argv[1]);

    soquete = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    // bind server address to socket descriptor
    bind(soquete, (struct sockaddr*)&server, sizeof(server));

    while(1) {
        len = sizeof(client);
        recvfrom(soquete, message, sizeof(message), 0, (struct sockaddr*)&client,&len);

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

        sendto(soquete, message, 81, 0, (struct sockaddr*)&client, sizeof(client));
    }

} 