#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "../common.h"

struct msgbuf {
    long mtype;
    int n;
    int arr[100];
};

void execFirstProcess(int*, int, int);
void execSecondProcess(int, int);

void execFirstProcess(int *arr, int n, int msgid){
    displayArr(arr, n);

    struct msgbuf msg;
    msg.mtype = 1;
    msg.n = n;
    for (int i = 0; i < n; i++){
        msg.arr[i] = arr[i];
    }

    msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
    msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 2, 0);

    for (int i = 0; i < msg.n; i++){
        arr[i] = msg.arr[i];
    }

    wait(NULL);

    displayArr(arr, n);

    free(arr);
    arr = NULL;

    msgctl(msgid, IPC_RMID, NULL);
}

void execSecondProcess(int msgid, int n){
    struct msgbuf msg;
    msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0);

    sort(msg.arr, msg.n);

    msg.mtype = 2;
    msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

    exit(0);
}

int main(){
    int n = 0;
    int *arr = readIntArr(&n);

    key_t key = ftok(".", 1);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if(pid > 0){
        execFirstProcess(arr, n, msgid);
    } else if(pid == 0){    
        execSecondProcess(msgid, n);
    } else{
        perror("fork failed");
        exit(1);
    }

    return 0;
}
