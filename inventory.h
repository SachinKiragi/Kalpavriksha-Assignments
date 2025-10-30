#ifndef INVENTORY_H
#define INVENTORY_H

#include<stdbool.h>

#define MIN_PRODUCT_ID 1
#define MAX_PRODUCT_ID 10000
#define MAX_NAME_LENGTH 50
#define MIN_PRICE 0
#define MAX_PRICE 100000
#define MIN_QUANTITY 0
#define MAX_QUANTITY 1000000

typedef enum{
    SUCCESS,
    FAILURE
} statusCode;

typedef struct {
    int productId;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
} productInfo;


// INITIAL INPUT FUNCTIONS
bool doesProductIdExist(int, productInfo*, int);
int getProductId(productInfo*, int); 
int length(char*);
char* getProductName();
float getProductPrice();
int getProductQuantity();
productInfo getCurrentProductInfo(productInfo*, int);
// MENU BASED FUNCTIONS
void displayCurrentProduct(productInfo*);
void displayProducts(productInfo*, int);
statusCode addNewProduct(productInfo**, int*, int*);
int getProductIndexById(productInfo*, int, int);
statusCode updateQuantity(productInfo*, int);
void searchProductById(productInfo*, int);
char toLower(char);
bool isSame(char, char);
bool isMatchingPartially(char*, char*);
void searchProductByName(productInfo*, int);
void searchProductByPriceRange(productInfo*, int);
void swap(productInfo*, productInfo*);
statusCode deleteProduct(productInfo*, int*, int*);

void handleInventoryManagementSystem(int);

#endif