#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void withdrawOp(int sock){
    int amt, res;
    scanf("%d", &amt);
    send(sock, &amt, sizeof(int), 0);
    recv(sock, &res, sizeof(int), 0);

    if(res == -1){
        printf("Insufficient Balance\n");
    } else{
        printf("Current Balance: %d\n", res);
    }
}

void depositOp(int sock){
    int amt, res;
    scanf("%d", &amt);
    send(sock, &amt, sizeof(int), 0);
    recv(sock, &res, sizeof(int), 0);
    printf("Current Balance: %d\n", res);
}

void displayOp(int sock){
    int res;
    recv(sock, &res, sizeof(int), 0);
    printf("Current Balance: %d\n", res);
}

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    int ch;

    while(1){
        printf("1. Withdraw\n2. Deposit\n3. Display Balance\n4. Exit\n");
        scanf("%d", &ch);

        if(ch < 1 || ch > 4){
            printf("Invalid Choice\n");
            continue;
        }

        send(sock, &ch, sizeof(int), 0);

        switch(ch){
            case 1:
                withdrawOp(sock);
                break;
            case 2:
                depositOp(sock);
                break;
            case 3:
                displayOp(sock);
                break;
            case 4:
                close(sock);
                return 0;
        }
    }

    close(sock);
    return 0;
}
