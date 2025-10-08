#include<stdio.h>
#include<stdbool.h>

#define MAXLEN 100

typedef enum{
    INVALID_ID,
    INVALID_USERNAME,
    INVALID_AGE,
    ID_EXISTS,
    SUCCESS
} Error;


void printError(Error err){
    switch(err){
        case INVALID_ID:
                        printf("\nError: Invalid User Id\n\n");
                        break;
        case ID_EXISTS:  
                        printf("\nError: Entered ID Exists Already\n\n");
                        break;
        case INVALID_USERNAME: 
                        printf("\nError: Invalid Username\n\n");
                        break;
        case INVALID_AGE:
                        printf("\nError: Invalid Age\n\n");
                        break;
        case SUCCESS:
                        printf("\nSuccess: Operation Performed Successfully\n\n");
                        break;
    }
}

typedef struct{
    int id;
    char name[MAXLEN];
    int age;
} User;


const char *fileName = "users.txt";
const char *tempFileName = "temp.txt";


bool createFile(){
    FILE *filePtr = fopen(fileName, "a");
    if(filePtr != NULL){
        fclose(filePtr);
        return 1;
    } else {
        printf("Error: While Creating The File\n");
        return 0;
    }
}

int length(char *s){
    int len = 0;
    while(s[len] != '\0') len++;
    return len;
}


bool appendUserIntoFile(User currUser){
    FILE* filePtr = fopen(fileName, "a");
    if(filePtr == NULL){
        printf("\nError: While Opening The File\n\n");
        return 0;
    } else{
        fprintf(filePtr, "%d, %s, %d\n", currUser.id, currUser.name, currUser.age);
        fclose(filePtr);
        return 1;
    }
}

//Checks If User With Given id Exists In users.txt
bool doesUserExist(int id){
    FILE* filePtr = fopen(fileName, "r");

    if(filePtr == NULL) {
        printf("\nError: While Opening The File\n");
        return 0;
    }

    User currUser;
    int found = 0;

    while(fscanf(filePtr, "%d, %[^,], %d\n", &currUser.id, currUser.name, &currUser.age) == 3){
        if(currUser.id == id){
            found = 1;
            break;
        }
    }

    fclose(filePtr);
    return found;
}


Error isUserValid(User *user){
    Error err = SUCCESS;
    if(length(user->name) == 0) { // True When User Presses Enter Key Without Any Text
        err = INVALID_USERNAME;
    } else if(user->id <= 0){
        err = INVALID_ID;
    } else if(doesUserExist(user->id)){
        err = ID_EXISTS;
    } else if(user->age <= 0){
        err = INVALID_AGE;
    } else{
        err = SUCCESS;
    }
    return err;
}


void getUserId(User *currUser){
    printf("Enter id (Number): ");
    scanf("%d", &currUser->id);
    getc(stdin); // Removes '\n' From Buffer
}

void getUserName(User *currUser){
    printf("Enter Name: ");
    fgets(currUser->name, MAXLEN, stdin);
    currUser->name[length(currUser->name)-1] = 0;
}

void getUserAge(User *currUser){
    printf("Enter Age: ");
    scanf("%d", &currUser->age);
    getc(stdin);
}

// Function To Take User Data As Input
Error inputUserInfo(User *currUser){

    getUserId(currUser);
    if(currUser->id == -1){ // True When User Enters Any Char Other That [0-9]
        return INVALID_ID;
    }
    getUserName(currUser);
    getUserAge(currUser);
   
    Error err = SUCCESS;

    err = isUserValid(currUser);

    return err;
}


void addUser(){

    User currUser = {.id = -1};
    Error err = inputUserInfo(&currUser);

    if(err == SUCCESS){ // Append User Into users Only If All Input Fileds Are Valid
        if(appendUserIntoFile(currUser)){
            printf("\nUser With Id %d Added Successfully\n\n", currUser.id);
        }
    } else{
        printError(err);
    }
}

void readUsers(){

    FILE* filePtr = fopen(fileName, "r");

    if(filePtr == NULL){
        printf("\nNo Users Found.\n");
        return;
    }

    printf("\n");

    User currUser;
    int noOfUsers = 0;

    while(fscanf(filePtr, "%d, %[^,], %d\n", &currUser.id, currUser.name, &currUser.age) == 3){
        if(noOfUsers == 0){
            printf("\n**************\nUsers List!\n\n");
        }
        printf("Id: %d\nName: %s\nAge: %d\n\n", currUser.id, currUser.name, currUser.age);
        noOfUsers++;
    }    

    if(noOfUsers == 0){
        printf("\nUsers List Is Empty!\n\n");
    } else{
        printf("****************\n");
    }

    fclose(filePtr);
}

void updateUser(){

    int id = -1;

    printf("Enter Id Of User: ");
    scanf("%d", &id);
    getc(stdin);

    if(!doesUserExist(id)){
        printf("\nUser With Id %d Does Not Exist\n\n", id);
        return;
    }

    FILE* filePtr = fopen(fileName, "r");

    if(filePtr == NULL){
        printf("\nError Accessing File\n\n");
        return;
    }

    User currUser;
    FILE *tempFilePtr = fopen(tempFileName, "w");
    if(tempFilePtr == NULL){ // Clearing The Temp.txt File
        printf("\nError While Opening The File\n");
        return;
    }

    Error err = SUCCESS;

    while(fscanf(filePtr, "%d, %[^,], %d\n", &currUser.id, currUser.name, &currUser.age) == 3){
        if(currUser.id == id){
            printf("Enter New Details: \n");
            getUserName(&currUser);
            getUserAge(&currUser);
            if(length(currUser.name) == 0){
                err = INVALID_USERNAME;
            } else if(currUser.age <= 0){
                err = INVALID_AGE;
            } else {
                err = SUCCESS;
            }
        }
        fprintf(tempFilePtr, "%d, %s, %d\n", currUser.id, currUser.name, currUser.age);
    }

    fclose(filePtr);
    fclose(tempFilePtr);

    if(err == SUCCESS){
        remove(fileName);
        rename(tempFileName, fileName);
        printf("\nUser With Id %d Updated Successfully\n\n", id);
    } else{
        printError(err);
    }
}

void deleteUser(){

    int id = -1;

    printf("Enter Id Of User: ");
    scanf("%d", &id);
    getc(stdin);    

    if(!doesUserExist(id)){
       printf("\nError: User With Id %d Does Not Exist\n\n", id);
       return;
    }

    FILE* filePtr = fopen(fileName, "r");

    if(filePtr == NULL){
        printf("\nError Accessing File\n\n");
        return;
    }

    User currUser = {.id = -1};

    FILE *tempFilePtr = fopen(tempFileName, "w");
    if(tempFilePtr == NULL){ // Clearing The Temp.txt File
        printf("\nError While Opening The File\n");
        return;
    } 

    while(fscanf(filePtr, "%d, %[^,], %d\n", &currUser.id, currUser.name, &currUser.age) == 3){
        if(currUser.id != id){
            fprintf(tempFilePtr, "%d, %s, %d\n", currUser.id, currUser.name, currUser.age);
        }
    }

    fclose(filePtr);
    fclose(tempFilePtr);
    remove(fileName);
    rename(tempFileName, fileName);
    printf("\nUser With Id %d Deleted Successfully\n\n", id);
}

int main(){

    if(createFile() == 0){
        return 0;
    }

    do{
        int op = -1;
        printf("\nEnter Operation: \n");
        printf("1-Add User\n");
        printf("2-Read Users\n");
        printf("3-Update User\n");
        printf("4-Delete User\n");
        printf("5-Exit\n");

        if(scanf("%d", &op) == 0){ // Handles If User Enters any char Instead Of Int
            printf("\nInvalid Input Plz Enter Integer In The Range [1-5]\n\n");
            while(getchar()!='\n');
            continue;
        }
        
        switch(op){
            case 1: addUser();
                    break;
            case 2: readUsers();
                    break;
            case 3: updateUser();
                    break;
            case 4: deleteUser();
                    break;  
            case 5: printf("\nExiting Successfully!\n\n");
                    return 0;
            default: printf("\nError: Invalid Choice! Try Again\n");
        }
    } while(1);

    return 0;
}
