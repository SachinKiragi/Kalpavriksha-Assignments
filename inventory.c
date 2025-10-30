#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "inventory.h"

bool doesProductIdExist(int id, productInfo *products, int noOfProducts){
    bool found = false;
    for(int i = 0; i < noOfProducts; i++){
        if(products[i].productId == id){
            found = true;
            break;
        }
    }
    return found;
}

int getProductId(productInfo* products, int noOfProducts){
    bool valid = false;
    int productId = -1;

    while(!valid){
        printf("Product ID: ");
        scanf("%d", &productId);
        getchar();
        if(productId < MIN_PRODUCT_ID || productId > MAX_PRODUCT_ID){
            printf("Invalid Id (Value Is Out Of Range!) Try Again\n");
        } else if(doesProductIdExist(productId, products, noOfProducts)){
            printf("Product With ID %d Exists Already! Please Enter Again\n", productId);
        } else{
            valid = true;
        }
    }

    return productId;
}

int length(char *s){
    int len = 0;
    int emptySpaces = 0;

    while(s[len] != '\n'){
        if(s[len] == ' '){ 
            emptySpaces++;
        }
        len++;
    }

    if(emptySpaces == len) len = 0;
    return len;
}

char* getProductName(){
    char *productName = NULL;
    char bufferString[MAX_NAME_LENGTH];
    bool valid = false;

    while(!valid){
        printf("Product Name: ");
        fgets(bufferString, MAX_NAME_LENGTH, stdin);
        int len = length(bufferString);
        if(len > 0){
            bufferString[len] = 0;
            productName = (char*)malloc(len * sizeof(char));
            strcpy(productName, bufferString);
            valid = true;
        } else{
            printf("Invalid Name! Try Again\n");
        }
    }

    return productName;
}

float getProductPrice(){
    bool valid = false;
    float price = -1;

    while(!valid){
        printf("Product Price: ");
        scanf("%f", &price);
        getchar();
        if(price >= MIN_PRICE && price <= MAX_PRICE){
            valid = true;
        } else{
            printf("Invalid Price! Try Again\n");
        }
    }

    return price;
}

int getProductQuantity(){
    bool valid = false;
    int quantity = -1;

    while(!valid){
        printf("Product Quantity: ");
        scanf("%d", &quantity);
        getchar();
        if(quantity >= MIN_QUANTITY && quantity <= MAX_QUANTITY){
            valid = true;
        } else{
            printf("Invalid Quantity! Try Again\n");
        }
    }

    return quantity;
}

productInfo getCurrentProductInfo(productInfo* products, int noOfProducts){
    productInfo currentProductInfo = {.productId = -1, .name = "", .price = 0, .quantity = 0};
    currentProductInfo.productId = getProductId(products, noOfProducts);

    char* tempProductName = getProductName();
    strcpy(currentProductInfo.name, tempProductName);
    free(tempProductName);
    tempProductName = NULL;
    
    currentProductInfo.price = getProductPrice();
    currentProductInfo.quantity = getProductQuantity();
    return currentProductInfo;
} 

void displayCurrentProduct(productInfo *currentproduct){
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d \n",
          currentproduct->productId,
          currentproduct->name,
          currentproduct->price,
          currentproduct->quantity);
}

void displayProducts(productInfo *products, int noOfProducts){
    printf("\n========= PRODUCT LIST =========\n");
    for(int i = 0; i < noOfProducts; i++){
        displayCurrentProduct(&products[i]);
    }
}

statusCode addNewProduct(productInfo** products, int *noOfProducts, int *capacity){
    if((*capacity) == (*noOfProducts)){
        (*capacity) = (*noOfProducts) * 2;
        *products = (productInfo*)realloc(*products, *capacity * sizeof(productInfo));
        if(*products == NULL){
            printf("Memory Allocation Failed!\n");
            return FAILURE;
        }
    }
    int indexToInsertNewProduct = *noOfProducts;
    (*products)[indexToInsertNewProduct] = getCurrentProductInfo(*products, *noOfProducts);
    (*noOfProducts)++;
    printf("Product added successfully! \n");
    return SUCCESS;
}

int getProductIndexById(productInfo* products, int noOfProducts, int id){
    int currentProductIndex = -1;
    for(int i = 0; i < noOfProducts; i++){
        if(products[i].productId == id){
            currentProductIndex = i;
            break;
        }
    }
    return currentProductIndex;
}

statusCode updateQuantity(productInfo* products, int noOfProducts){
    printf("Enter Product ID to update quantity: ");
    int id;
    scanf("%d", &id);
    getchar();
    int currentProductIndex = getProductIndexById(products, noOfProducts, id);
    if(currentProductIndex != -1){
        products[currentProductIndex].quantity = getProductQuantity();
        printf("Quantity updated successfully!\n");
        return SUCCESS;
    } else{
        printf("Product With Id %d Does Not Exists\n", id);
        return FAILURE;
    }
}

void searchProductById(productInfo* products, int noOfProducts){
    printf("Enter Product ID to search: ");
    int id;
    scanf("%d", &id);
    getchar();
    int currentProductIndex = getProductIndexById(products, noOfProducts, id);
    if(currentProductIndex != -1){
        printf("Product Found: ");
        displayCurrentProduct(&products[currentProductIndex]);
    } else{
        printf("No product found with id '%d'.\n", id);
    }
}

char toLower(char ch){
    if(ch >= 'A' && ch <= 'Z'){
        ch = ch - 'A' + 'a';
    }
    return ch;
}


bool isSame(char ch1, char ch2){
    ch1 = toLower(ch1);
    ch2 = toLower(ch2);
    return ch1 == ch2;
}

bool isMatchingPartially(char *nameToSearch, char *productName){
    int i = 0;

    while(nameToSearch[i] != '\0' && productName[i] != '\0'){
        if(!isSame(nameToSearch[i], productName[i])){
            return false;
        }
        i++;
    }

    return nameToSearch[i] == '\0';
}

void searchProductByName(productInfo *prodcuts, int noOfProducts){
    printf("Enter name to search (partial allowed): ");
    char nameToSearch[MAX_NAME_LENGTH];
    fgets(nameToSearch, MAX_NAME_LENGTH, stdin);
    int len = length(nameToSearch);
    nameToSearch[len] = 0;
    bool found = false;

    for(int i = 0; i < noOfProducts; i++){
        if(isMatchingPartially(nameToSearch, prodcuts[i].name)){
            if(found == false){
                printf("Products Found: \n");
                found = true;
            }
            displayCurrentProduct(&prodcuts[i]);
        }
    }

    if(!found) {
        printf("No products found matching the name '%s'.\n", nameToSearch);
    }
}

void searchProductByPriceRange(productInfo* products, int noOfProducts){
    float minPrice = -1, maxPrice = -1;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);
    bool found = false;

    for(int i = 0; i < noOfProducts; i++){
        float currentproductprice = products[i].price;
        if(currentproductprice >= minPrice && currentproductprice <= maxPrice){
            if(found == false){
                printf("\nProducts in price range:\n");
                found = true;
            }
            displayCurrentProduct(&products[i]);
        }
    }

    if(!found) {
        printf("No products found with price ranging from %.2f to %.2f.\n", minPrice, maxPrice);
    }
}

statusCode deleteProduct(productInfo *products, int *noOfProducts, int *capacity){
    printf("Enter Product ID to delete: ");
    int id;
    scanf("%d", &id);
    getchar();
    int currentProductIndex = getProductIndexById(products, *noOfProducts, id);
    if(currentProductIndex != -1){
        for(int i = currentProductIndex + 1; i < *noOfProducts; i++){
            products[i-1] = products[i];
        }
        (*noOfProducts)--;
        printf("Product deleted successfully! \n");
        return SUCCESS;
    } else{
        printf("No product found with id '%d'.\n", id);
        return FAILURE;
    }
}

void handleInventoryManagementSystem(int noOfProducts){
    productInfo *products = (productInfo*) calloc (noOfProducts, sizeof(productInfo));
    if(products == NULL){
        printf("Error Occured While Alloating Memory!\n");
        return;
    }

    for(int i = 0; i < noOfProducts; i++){
        printf("\nEnter details for product %d\n", i+1);
        products[i] = getCurrentProductInfo(products, noOfProducts);
    }

    int choice = 0;
    int capacity = noOfProducts;

    while(1){
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n"); 
        printf("2. View All Products\n"); 
        printf("3. Update Quantity\n"); 
        printf("4. Search Product by ID\n"); 
        printf("5. Search Product by Name\n"); 
        printf("6. Search Product by Price Range\n"); 
        printf("7. Delete Product\n"); 
        printf("8. Exit\n"); 
        printf("Enter your choice:\n");
        scanf("%d", &choice);
        getchar();
        switch(choice){
            case 1: if(addNewProduct(&products, &noOfProducts, &capacity) == FAILURE){
                        printf("Error Occured While Updating The Product\n");  
                    } 
                    break;
            case 2: displayProducts(products, noOfProducts);
                    break;
            case 3: if(updateQuantity(products, noOfProducts) == FAILURE){
                        printf("Error Occured While Updating The Product\n");
                    }
                    break;
            case 4: searchProductById(products, noOfProducts);
                    break;
            case 5: searchProductByName(products, noOfProducts);
                    break;
            case 6: searchProductByPriceRange(products, noOfProducts);
                    break;
            case 7: if(deleteProduct(products, &noOfProducts, &capacity) == FAILURE){
                        printf("Error Occured While Deleting The Product\n");
                    }
                    break;
            case 8: free(products);
                    products = NULL;
                    printf("Memory released successfully. Exiting program...\n");
                    return;
            default: printf("Invalid Choice\n");
        }
    }
}