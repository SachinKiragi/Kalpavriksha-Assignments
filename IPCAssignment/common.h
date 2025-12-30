#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

int* readIntArr(int*);
void displayArr(int*, int);
void sort(int*, int);

int* readIntArr(int *n){
    scanf("%d", n);
    int *arr = (int*) calloc(*n, sizeof(int));
    for(int i = 0; i < *n; i++){
        scanf("%d", &arr[i]);
    }
    return arr;
}

void displayArr(int *arr, int n){
    for(int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void sort(int *arr, int n){
    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < n - i - 1; j++){
            if(arr[j] > arr[j + 1]){
                int t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }
}

#endif
