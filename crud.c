#include<stdio.h>

#define MAXLEN 100

typedef struct 
{
    int id;
    char name[MAXLEN];
    int age;
} User;

char *fileName = "users.txt";
char *tempFileName = "temp.txt";

int createFile(){
    FILE *filePtr = fopen(fileName, "a");
    if(filePtr!=NULL){
        fclose(filePtr);
        return 1;
    }
    printf("Error While Creating The File\n");
    return 0;
}

int length(char *s){
    int len = 0;
    while(s[len]!='\0') len++;
    return len;
}

void appendUserIntoFile(FILE* filePtr, User currUser){
    if(currUser.id > 0 && length(currUser.name) > 0 && currUser.age > 0)
        fprintf(filePtr, "%d, %s, %d\n", currUser.id, currUser.name, currUser.age);
    else    
        printf("\nError Occured While Storing User Data\n");
}

int doesUserExits(int id){
    FILE* filePtr = fopen(fileName, "r");
    if(filePtr==NULL) {
        printf("\nError While Opening The File\n");
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

void addUser(){
    FILE* filePtr = fopen(fileName, "a");
    if(filePtr==NULL){
        printf("error");
        return;
    }

    User currUser;

    printf("Enter id (Number): ");
    scanf("%d", &currUser.id);
    getc(stdin);

    if(currUser.id <= 0){
        printf("\nInvalid Id! (%d)\n\n", currUser.id);
        fclose(filePtr);
        return;
    }

    if(doesUserExits(currUser.id)){
        printf("\nUser With Id %d Exists Already!\n\n", currUser.id);
        fclose(filePtr);
        return;
    }

    printf("Enter Name: ");
    fgets(currUser.name, MAXLEN, stdin);
    int len = length(currUser.name);
    
    if(len <= 1){
        printf("\nInvalid Name! (\\n)\n\n");
        fclose(filePtr);
        return;
    }
    currUser.name[len-1] = 0;

    printf("Enter Age: ");
    scanf("%d", &currUser.age);

    if(currUser.age <= 0){
        printf("\nInvalid Age! (%d)\n\n", currUser.age);
        fclose(filePtr);
        return;
    }
    appendUserIntoFile(filePtr, currUser);
    fclose(filePtr);
}

void readUsers(){
    FILE* filePtr = fopen(fileName, "r");
    if(filePtr==NULL){
        printf("\nNo Users Found.\n");
        return;
    }

    printf("\n");
    User currUser;

    int noOfUsers = 0;

    while(fscanf(filePtr, "%d, %[^,], %d\n", &currUser.id, currUser.name, &currUser.age) == 3){
        if(noOfUsers==0){
            printf("\n**************\nUsers List!\n\n");
        }
        printf("Id: %d\nName: %s\nAge: %d\n\n", currUser.id, currUser.name, currUser.age);
        noOfUsers++;
    }    

    if(noOfUsers==0){
        printf("\nUsers List Is Empty!\n\n");
    } else{
        printf("****************\n");
    }

    fclose(filePtr);
}

void updateUser(){
    int id;
    printf("Enter Id Of User: ");
    scanf("%d", &id);
    fflush(stdin);

    if(!doesUserExits(id)){
        printf("\nUser With Id %d Does Not Exist\n\n", id);
        return;
    }

    FILE* filePtr = fopen(fileName, "r");
    FILE* tempFilePtr = fopen(tempFileName, "w");

    User currUser;

    while(fscanf(filePtr, "%d, %[^,], %d\n", &currUser.id, currUser.name, &currUser.age) == 3){
        if(currUser.id == id){
            printf("Enter New Name: ");
            fgets(currUser.name, MAXLEN, stdin);
            int len = length(currUser.name);
            currUser.name[len-1] = 0;

            printf("Enter New Age: ");
            scanf("%d", &currUser.age);
        }
        appendUserIntoFile(tempFilePtr, currUser);
    }

    fclose(filePtr);
    fclose(tempFilePtr);
    remove(fileName);
    rename(tempFileName, fileName);
    printf("\nUser Updated Successfully\n\n");
}

void deleteUser(){
    int id;
    printf("Enter Id Of User: ");
    scanf("%d", &id);
    getc(stdin);

    if(!doesUserExits(id)){
       printf("\nUser With Id %d Does Not Exist\n\n", id);
       return;
    }

    FILE* filePtr = fopen(fileName, "r");
    FILE* tempFilePtr = fopen(tempFileName, "w");

    User currUser;
    while(fscanf(filePtr, "%d, %[^,], %d\n", &currUser.id, currUser.name, &currUser.age) == 3){
        if(currUser.id != id){
            appendUserIntoFile(tempFilePtr, currUser);
        }
    }

    fclose(filePtr);
    fclose(tempFilePtr);
    remove(fileName);
    rename(tempFileName, fileName);
    printf("\nUser Deleted Successfully\n\n");
}

int main(){
    createFile();
    do{
        int op;
        printf("\nEnter Operation: \n1-Add User\n2-Read Users\n3-Update User\n4-Delete User\n5-Exit\n");
        scanf("%d", &op);

        switch(op){
            case 1: addUser();
                    break;
            case 2: readUsers();
                    break;
            case 3: updateUser();
                    break;
            case 4: deleteUser();
                    break;
            case 5: 
                    printf("\nExiting Successfully!\n\n");
                    return 0;
            default: printf("\nInvalid Choice! Try Again\n");
        }
    } while(1);

    return 0;
}
