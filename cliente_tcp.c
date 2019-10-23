#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

void chat(int soquete) {

    int aux_index;
    char message[80];

    memset(message, 0, sizeof(message));

    fgets(message, sizeof(message), stdin);
    aux_index = strlen(message) - 1;
    if(message[aux_index] == '\n') {
        message[aux_index] = '\0';
    }

    write(soquete, message, sizeof(message));
    memset(message, 0, sizeof(message));
    read(soquete, message, sizeof(message));
    printf("%s", message);

}

int main(int argc, char *argv[]) {

    int soquete, port = 8080;
    struct sockaddr_in server;

    soquete = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);

    connect(soquete, (struct sockaddr*)&server, sizeof(server));

    chat(soquete);

}
