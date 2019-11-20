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

#define MAX 255

char* nomes[MAX];
int soquetes[MAX];
bool posicoes[MAX];

void* chat(void* arg) {
    int soquete = *((int*)arg), have_name = 0;
    char message[81];

    free(arg);

    while(1){
        memset(message, 0, 81);

        read(soquete, message, sizeof(message));

        if (strcmp("exit", message) == 0) {
            //atualiza lista de ususarios
            break;
        } else if(strcmp("users", message) == 0){
            //retorna todos os usuarios

        } else if(strncmp("all;", message, 4) == 0){
            //envia mensagem para todos os clientes conectados

        } else if(strncmp("uni;", message, 4) == 0){
            //envia mensagem para cliente X
            //lock (or maybe not? its just reading)
            char *nome_aux = message + 4;
            strtok(nome_aux, ";");
            char* msg = message + (5 + strlen(nome_aux));
            for(int i = 0; i < MAX; i++){
                if(strcmp(nomes[i], nome_aux)){
                    write(soquetes[i], msg, sizeof(msg));
                    break;
                }
            }
            //unlock
        } else if(have_name == 0){
            //insere cliente com o nome recebido
            //lock
            for(int i = 0; i < MAX; i++){
                if(posicoes[i] == false){
                    nomes[i] = message;
                    soquetes[i] = soquete;
                    posicoes[i] = true;
                    have_name = 1;
                    break;
                }
            }
            if(have_name == 0){
                printf("Cliente não cadastrado. Limite máximo atingido.\n");
            }
            //unlock
        }

        write(soquete, message, sizeof(message));
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int soquete, communication, len, port;
    struct sockaddr_in server, client;

    for(int i = 0; i < MAX; i++){
        nomes[i] = NULL;
        soquetes[i] = -1;
        posicoes[i] = false;
    }

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
