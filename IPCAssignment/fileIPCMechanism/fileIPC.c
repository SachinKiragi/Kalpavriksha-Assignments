#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../common.h"

const char* fileName = "file.txt";

void writeDataToFile(int*, int);
void readDataFromFile();
void execFirstProcess();
void execSecondProcess();

void writeDataToFile(int* arr, int n){
    FILE *fp = fopen(fileName, "w");
    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++){
        fprintf(fp, "%d ", arr[i]);
    }
    fclose(fp);
}

void readDataFromFile(){
    FILE* fp = fopen(fileName, "r");
    int n = 0, tempVar = 0;
    fscanf(fp, "%d", &n); 
    for (int i = 0; i < n; i++){
        fscanf(fp, "%d", &tempVar);   
        printf("%d ", tempVar);
    }
    printf("\n");
    fclose(fp);
}

void execFirstProcess(){
    readDataFromFile();
}

void execSecondProcess(){
    FILE* fp = fopen(fileName, "r");
    int n = 0;
    fscanf(fp, "%d", &n);
    int *arr = (int*) calloc(n, sizeof(int));
    for (int i = 0; i < n; i++){
        fscanf(fp, "%d", &arr[i]);
    }
    fclose(fp);

    sort(arr, n);
    writeDataToFile(arr, n);

    free(arr);
    arr = NULL;
}

int main(){
    int n = 0;
    int* arr = readIntArr(&n);

    writeDataToFile(arr, n);
    readDataFromFile();
    
    free(arr);
    arr = NULL;

    pid_t pid = fork();

    if(pid > 0){
        wait(NULL);
        execFirstProcess();
    } else if(pid == 0){
        execSecondProcess();
    } else {
        perror("fork failed");
        exit(1);
    }

    return 0; 
}
