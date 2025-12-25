#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080

pthread_mutex_t lock;

int readBalance(){
    FILE *fp = fopen("../resource/accountDB.txt", "r");
    int bal;
    fscanf(fp, "%d", &bal);
    fclose(fp);
    return bal;
}

void writeBalance(int bal){
    FILE *fp = fopen("../resource/accountDB.txt", "w");
    fprintf(fp, "%d", bal);
    fclose(fp);
}

void handleWithdraw(int sock){
    int amount;
    int balance = readBalance();
    int res;

    recv(sock, &amount, sizeof(int), 0);

    if(amount <= balance){
        balance -= amount;
        writeBalance(balance);
        res = balance;
    } else{
        res = -1;
    }

    send(sock, &res, sizeof(int), 0);
}

void handleDeposit(int sock){
    int amount;
    int balance = readBalance();

    recv(sock, &amount, sizeof(int), 0);
    balance += amount;
    writeBalance(balance);

    send(sock, &balance, sizeof(int), 0);
}

void handleDisplay(int sock){
    int balance = readBalance();
    send(sock, &balance, sizeof(int), 0);
}

void* handleClient(void *arg){
    int sock = *(int*)arg;
    free(arg);

    int choice;

    while(1){
        if(recv(sock, &choice, sizeof(int), 0) <= 0) break;

        pthread_mutex_lock(&lock);

        switch(choice){
            case 1:
                handleWithdraw(sock);
                break;
            case 2:
                handleDeposit(sock);
                break;
            case 3:
                handleDisplay(sock);
                break;
            case 4:
                pthread_mutex_unlock(&lock);
                close(sock);
                return NULL;
            default:
                break;
        }

        pthread_mutex_unlock(&lock);
    }

    close(sock);
    return NULL;
}

int main(){
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSock, 5);

    pthread_mutex_init(&lock, NULL);

    while(1){
        int *clientSock = malloc(sizeof(int));
        *clientSock = accept(serverSock, NULL, NULL);

        pthread_t tid;
        pthread_create(&tid, NULL, handleClient, clientSock);
        pthread_detach(tid);
    }

    close(serverSock);
    pthread_mutex_destroy(&lock);
    return 0;
}
