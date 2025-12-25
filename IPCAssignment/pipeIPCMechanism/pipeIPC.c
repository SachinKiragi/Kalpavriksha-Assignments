#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../common.h"

void execFirstProcess(int*, int, int[2], int[2]);
void execSecondProcess(int, int[2], int[2]);

void execFirstProcess(int *arr, int n, int parentToChild[2], int childToParent[2]){
    displayArr(arr, n);

    close(parentToChild[0]);
    close(childToParent[1]);

    write(parentToChild[1], &n, sizeof(int));
    write(parentToChild[1], arr, n * sizeof(int));
    close(parentToChild[1]);

    read(childToParent[0], &n, sizeof(int));
    read(childToParent[0], arr, n * sizeof(int));
    close(childToParent[0]);

    wait(NULL);

    displayArr(arr, n);

    free(arr);
    arr = NULL;
}

void execSecondProcess(int n, int parentToChild[2], int childToParent[2]){
    close(parentToChild[1]);
    close(childToParent[0]);

    read(parentToChild[0], &n, sizeof(int));
    int *childArr = (int*) calloc(n, sizeof(int));
    read(parentToChild[0], childArr, n * sizeof(int));
    close(parentToChild[0]);

    sort(childArr, n);

    write(childToParent[1], &n, sizeof(int));
    write(childToParent[1], childArr, n * sizeof(int));
    close(childToParent[1]);

    free(childArr);
    childArr = NULL;

    exit(0);
}

int main(){
    int n = 0;
    int *arr = readIntArr(&n);

    int parentToChild[2], childToParent[2];
    pipe(parentToChild);
    pipe(childToParent);

    pid_t pid = fork();

    if(pid > 0){
        execFirstProcess(arr, n, parentToChild, childToParent);
    } else if(pid == 0){
        execSecondProcess(n, parentToChild, childToParent);
    } else{
        exit(1);
    }

    return 0;
}
