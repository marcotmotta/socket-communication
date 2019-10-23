
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

void chat(int soquete) {

    int n;
    char message[80];

    memset(message, 0, sizeof(message));
    printf("Enter the string : ");
    n = 0;

    while ((message[n++] = getchar()) != '\n')
        ;

    write(soquete, message, sizeof(message));
    memset(message, 0, sizeof(message));
    read(soquete, message, sizeof(message));
    printf("%s", message);

}

int main(int argc, char *argv[]) {

    int soquete, connfd, port = 8080;
    struct sockaddr_in server;

    // soquete create and varification
    soquete = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));

    // assign IP, port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);

    // connect the client soquete to server soquete
    connect(soquete, (struct sockaddr*)&server, sizeof(server));

    // function for chat
    chat(soquete);

}
