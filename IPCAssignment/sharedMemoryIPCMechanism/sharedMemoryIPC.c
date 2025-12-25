#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../common.h"

void execFirstProcess(int*, int, int);
void execSecondProcess(int, int);

void execFirstProcess(int *arr, int n, int shmid){
    displayArr(arr, n);

    int *shm = (int*) shmat(shmid, NULL, 0);
    shm[0] = n;
    for(int i = 0; i < n; i++){    
        shm[i + 1] = arr[i];
    } 

    wait(NULL);

    for(int i = 0; i < shm[0]; i++){
        arr[i] = shm[i + 1];
    } 

    displayArr(arr, n);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);

    free(arr);
    arr = NULL;
}

void execSecondProcess(int shmid, int n){
    int *shm = (int*) shmat(shmid, NULL, 0);

    int size = shm[0];
    int *childArr = (int*) calloc(size, sizeof(int));
    for(int i = 0; i < size; i++){
        childArr[i] = shm[i + 1];
    } 

    sort(childArr, size);

    for(int i = 0; i < size; i++){
        shm[i + 1] = childArr[i];
    } 

    shmdt(shm);
    free(childArr);
    childArr = NULL;

    exit(0);
}

int main(){
    int n = 0;
    int *arr = readIntArr(&n);

    key_t key = ftok(".", 1);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if(pid > 0){
        execFirstProcess(arr, n, shmid);
    } else if(pid == 0){
        execSecondProcess(shmid, n);
    } else{
        exit(1);
    }

    return 0;
}