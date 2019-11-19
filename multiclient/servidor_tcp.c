#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

void chat(int soquete) {

    int message_length, case_aux;
    char message[81], message_aux;

    memset(message, 0, 81);

    read(soquete, message, sizeof(message));

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

    write(soquete, message, sizeof(message));

}

int main(int argc, char *argv[]) {

    int soquete, communication, len, port;
    struct sockaddr_in server, client;

    port = atoi(argv[1]);

    soquete = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    bind(soquete, (struct sockaddr*)&server, sizeof(server));

    while(1) {
        listen(soquete, 5);

        len = sizeof(client);
        communication = accept(soquete, (struct sockaddr*)&client, &len);

        chat(communication);
    }

    close(soquete);

}
