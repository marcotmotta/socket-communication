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

//Nomes dos clientes
char nomes[MAX][16];
//Soquetes dos clientes
int soquetes[MAX];
//Posicao está ocupada por um cliente ou nao
bool posicoes[MAX];

pthread_mutex_t lock;

void* chat(void* arg) {
    int soquete = *((int*)arg), have_name = 0;
    char message[81];

    free(arg);

    while(1){
        memset(message, 0, 81);

        //Lê mensagem do cliente
        read(soquete, message, sizeof(message));

        if (strcmp("exit", message) == 0) {
            //Atualiza lista de ususarios caso um usuario saia
            pthread_mutex_lock(&lock);
            for(int i = 0; i < MAX; i++){
                if(soquetes[i] == soquete){
                    write(soquete, message, sizeof(message));
                    memset(nomes[i], 0, 16);
                    soquetes[i] = -1;
                    posicoes[i] = false;
                    break;
                }
            }
            pthread_mutex_unlock(&lock);
        } else if(strcmp("users", message) == 0){
            //Retorna lista com todos os usuarios conectados
            char msg[4097];
            memset(msg, 0, 4097);
            int first = 1;
            for(int i = 0; i < MAX; i++){
                if(posicoes[i] == true){
                    if(first){
                        strcpy(msg, nomes[i]);
                        first--;
                    } else {
                        strcat(msg, "\n");
                        strcat(msg, nomes[i]);
                    }
                }
            }
            write(soquete, msg, 81);
        } else if(strncmp("all;", message, 4) == 0){
            //Envia mensagem para todos os clientes conectados
            char msg[81];
            memset(msg, 0, 81);
            strcpy(msg, (message + 4));
            for(int i = 0; i < MAX; i++){
                if(posicoes[i] == true){
                    write(soquetes[i], msg, sizeof(msg));
                }
            }
        } else if(strncmp("uni;", message, 4) == 0){
            //Envia mensagem para um cliente especifico
            char msg[81];
            memset(msg, 0, 81);
            char *nome_aux = message + 4;
            strtok(nome_aux, ";");
            strcpy(msg, (message + (5 + strlen(nome_aux))));
            for(int i = 0; i < MAX; i++){
                if(strcmp(nomes[i], nome_aux) == 0){
                    write(soquetes[i], msg, sizeof(msg));
                    break;
                }
            }
        } else if(have_name == 0){
            //Insere cliente com o nome recebido
            pthread_mutex_lock(&lock);
            for(int i = 0; i < MAX; i++){
                if(posicoes[i] == false){
                    strcpy(nomes[i], message);
                    soquetes[i] = soquete;
                    posicoes[i] = true;
                    have_name = 1;
                    break;
                }
            }
            if(have_name == 0){
                char msg[81] = "Cliente não pôde ser cadastrado.";
                write(soquete, msg, sizeof(msg));
            }
            pthread_mutex_unlock(&lock);
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int soquete, communication, len, port;
    struct sockaddr_in server, client;

    for(int i = 0; i < MAX; i++){
        memset(nomes[i], 0, 16);
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

        //Cria uma thread nova de comunicação para cada cliente que se conecta com o servidor
        pthread_t t;
        pthread_create(&t, NULL, chat, arg);
    }

    pthread_mutex_destroy(&lock);

    close(soquete);
}
