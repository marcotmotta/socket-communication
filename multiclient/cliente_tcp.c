#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

//Nome do cliente
char *nome;
//Soquete do cliente
int soquete;

void* readChat(void *arg){
    char message[81];

    //Loop para cliente permanecer lendo o que o servidor mandar
    while(1){
        memset(message, 0, sizeof(message));
        //Lê mensagem do servidor
        read(soquete, message, sizeof(message));

        //Interrompe caso a mensagem recebida seja exit
        if(strcmp("exit", message) == 0){
            break;
        }

        printf("%s\n", message);
    }

    return NULL;
}

void* writeChat(void *arg) {
    int aux_index;
    char message[81];

    //Envia string com o nome do cliente para o servidor
    write(soquete, nome, sizeof(nome));

    //Loop para permanecer enviando mensagens ao servidor
    while(1){
        memset(message, 0, sizeof(message));

        //Lê mensagem do stdin
        fgets(message, sizeof(message), stdin);

        aux_index = strlen(message) - 1;
        if(message[aux_index] == '\n') {
            message[aux_index] = '\0';
        }

        //Envia mensagem para servidor
        write(soquete, message, sizeof(message));

        //Sai caso a mensagem escrita for exit
        if(strcmp("exit", message) == 0){
            break;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int port;
    char *ip;
    struct sockaddr_in server;

    ip = strdup(argv[1]);
    port = atoi(argv[2]);
    nome = argv[3];

    soquete = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    connect(soquete, (struct sockaddr*)&server, sizeof(server));

    //Cria duas threads.
    //Uma pra ler do servidor e outra pra escrever pro servidor.
    //Assim, ambas as ações independem uma da outra
    pthread_t t[2];
    pthread_create(&(t[0]), NULL, writeChat, NULL);
    pthread_create(&(t[1]), NULL, readChat, NULL);

    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
}
