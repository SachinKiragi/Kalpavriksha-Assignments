#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#define MIN_IMAGE_SIZE 2
#define MAX_IMAGE_SIZE 10
#define MAX_INPUT_LENGTH 100 // Used To Get Valid Integres From User
#define MOD 256


bool isDigit(char ch){
    return ch >= '0' && ch <= '9';
}


bool isSpace(char ch){
    return ch == ' ';
}


int stringToNumber(char *valueAsString, int length){

    int valueAsInt = 0;
    int i = 0;

    while(i < length){
        if(isDigit(valueAsString[i])){
            valueAsInt = valueAsInt * 10 + (valueAsString[i] - '0');
            i++;
        } else if(isSpace(valueAsString[i])){
            i++;
        } else{
            valueAsInt = -1;   
            break;
        }
    }
    return valueAsInt;
}


int getLength(char *valueAsString){
    int i = 0;
    while(valueAsString[i] != '\0'){
        i++;
    }
    return i-1;
}


int getValidInteger(){
    
    char valueAsString[MAX_INPUT_LENGTH];
    fgets(valueAsString, MAX_INPUT_LENGTH, stdin);

    int length = getLength(valueAsString);
    valueAsString[length] = 0;

    int valueAsInt = stringToNumber(valueAsString, length);

    return valueAsInt;

}


int getValueAtij(int ***image, int i, int j, int n){
    if(i < 0 || i >= n || j < 0 || j >= n) {
        return -1;
    } else{
        return (*(*(*(image + i) + j)));
    }
}


void printImage(int ***image, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int value = getValueAtij(image, i, j, n) % MOD;
            printf("%d\t", value);
        }
        printf("\n");
    }
    printf("=================================================================\n");
}


void initializeImage(int ***image, int n){
    
    srand(time(NULL));
    
    for(int i = 0; i < n; i++){
        *(image + i) = (int**)malloc(n * sizeof(int*));
        for(int j = 0; j < n; j++){
            *((*(image + i)) + j) = (int*)malloc(sizeof(int));
            int *addressAtij = (*(*(image + i) + j));
            *addressAtij = rand() % MOD;
        }
    }
}


// Swapping Adresses 
void swap(int **value1, int **value2){
    int *temp = *value1;
    *value1 = *value2;
    *value2 = temp;
}


void reverseCurrentRow(int **image, int n){
    int i = 0;
    int j = n - 1;
    while(i < j){
        swap(image + i, image + j);
        i++;
        j--;
    }
}


void transposeImage(int ***image, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n-i; j++){
             swap((*(image + i) + j), (*(image + (n - j - 1)) + (n - i - 1)));
        }
    }
}

//Rotating Image 90 Degree Clockwise
void rotateImage(int ***image, int n){
    
    for(int i = 0; i < n; i++){
        reverseCurrentRow(*(image + i), n);
    }

    transposeImage(image, n);
}


void applySmoothingAtCurrentIndex(int ***image, int i, int j, int n){
    int rowDir[] = {0, -1, -1, -1, 0, 1, 1, 1, 0};
    int colDir[] = {0, -1, 0, 1, 1, 1, 0, -1, -1};

    int total = 0;
    int currentValue = 0;
    int noOfElements = 0;

     for(int k = 0; k < 9; k++){
        currentValue = getValueAtij(image, i + rowDir[k], j + colDir[k], n) % MOD;
        if(currentValue != -1){
            total += currentValue;
            noOfElements++;
        }
     }
    
     int average = total / noOfElements;
     int oldValue = getValueAtij(image, i, j, n) % MOD;
     int newValue = average * MOD + oldValue;
     
     *(*((*(image + i)) + j)) = newValue;

     /*
        Since We Need To Modify And Store Average In Place!
        Here The Concept Of Storing 2 Values In a Single Value Is Used
        Since The Average As Well As Original Values Can Never Be Greater That 255 We Can Use Below Formula
        newValue = average * 256 + oldValue -----|
        average = newValue / 256 ----------------|---This Helps Us To Store Average As Well As Old Value At a Same Place
        oldValue = newValue % 256 ---------------|
        since the oldValue (i.e. Value In Image Will Always Be < 255 -> Therefor 256 Is Used As MOD);
     */
    
} 


void applySmoothingFilter(int ***image, int n){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            applySmoothingAtCurrentIndex(image, i, j, n);
        }
    }

}


void analayzeSonarImage(int ***image, int n){

    printf("Original\n");
    printImage(image, n);

    rotateImage(image, n);
    printf("Roatated:\n");
    printImage(image, n);

    applySmoothingFilter(image, n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int value = getValueAtij(image, i, j, n);
            *(*((*(image + i)) + j)) = value / MOD;
        }
    }

    printf("Final Output\n");
    printImage(image, n);
}


void deAllocateMemory(int ***image, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            free(*(*(image + i) + j));
        }
        free(*(image + i));
    }
    free(image);
}

int main(){
    int n = 0;
    do{
        n = getValidInteger();
        if(n < MIN_IMAGE_SIZE || n > MAX_IMAGE_SIZE){
            printf("Invalid Integer Input! The Range Should Be [2-10]: \n");
        }
    } while(n < MIN_IMAGE_SIZE || n > MAX_IMAGE_SIZE);

    int*** image = (int***)malloc(n * (sizeof(int**)));

    initializeImage(image, n);
    analayzeSonarImage(image, n);

    deAllocateMemory(image, n);
  
    return 0;
}
