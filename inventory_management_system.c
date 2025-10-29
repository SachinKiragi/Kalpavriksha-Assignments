#include<stdio.h>
#include "inventory.h"

int main(){

    int noOfProducts = 0;

    while(noOfProducts < 1 || noOfProducts > 100){
        printf("Enter initial number of products (in the range [1 - 100]): ");
        scanf("%d", &noOfProducts);
        getchar();
    }

    handleInventoryManagementSystem(noOfProducts);

    return 0;
}