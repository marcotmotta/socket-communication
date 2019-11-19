#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>

void* chat(void* arg) {
    int message_length, case_aux, soquete = *((int*)arg);
    char message[81], message_aux;

    free(arg);

    while(1){
        memset(message, 0, 81);

        read(soquete, message, sizeof(message));

        if (strncmp("exit", message, 4) == 0) {
            break;
        }



        write(soquete, message, sizeof(message));
    }

    return NULL;
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

    listen(soquete, 256);

    while(1) {
        len = sizeof(client);
        communication = accept(soquete, (struct sockaddr*)&client, &len);

        int *arg = malloc(sizeof(int));
        *arg = communication;

        pthread_t t;
        pthread_create(&t, NULL, chat, arg);
    }

    close(soquete);
}
