#include "fileNode.h"

void displaystatus(statusCode status){
    switch(status){
        case FILENODE_EXISTS: printf("Name already exists in current directory.\n");
        break;
        case FILE_NOT_FOUND: printf("File not found!.\n");
        break;
        case DIRECTORY_NOT_FOUND: printf("Directory not found!.\n");
        break;
        case DIRECTORY_NOT_EMPTY: printf("Directory not empty. Remove files first.\n");
        break;
        case EMPTY_DIRECTORY: printf("Directory is empty.\n");
        break;
        case DISK_IS_FULL: printf("Disk is full.\n");
        break;
        case SUCCESS: printf("Operation completed successfully\n");
        break;
        default: printf("No match\n");
    }
}

bool fileNodeExists(fileNode* cwd, char *name){
    fileNode* curr = cwd->childrensHead;
    if(curr == NULL){
        return false;
    }
    do{
        if(strcmp(curr->name, name) == 0){
            return true;
        }
        curr = curr->next;
    } while(curr != cwd->childrensHead);
    return false;
}

fileNode* createNewFileNode(){
    fileNode* newFile = (fileNode*)calloc(1, sizeof(fileNode));
    newFile->parent = NULL;
    newFile->next = NULL;
    newFile->childrensHead = NULL;
    return newFile;
}

fileNode* getFileNode(fileNode* cwd, char* fileName){
    fileNode* curr = cwd->childrensHead;
    if(curr == NULL){
        return NULL;
    }
    do{
        if(strcmp(curr->name, fileName) == 0){
            return curr;
        }
        curr = curr->next;
    } while(curr != cwd->childrensHead);

    return NULL;
}

//helper Function To Insert New File/Directory Inside cwd->childrens
fileNode* addNewFileNodeInCurrentDirectory(fileNode* childrensHead, fileNode* newFileNode){
    if(childrensHead == NULL){
        newFileNode->next = newFileNode;
        childrensHead = newFileNode;
    } else{
        fileNode* lastFileNode = childrensHead;
        fileNode* startFileNode = childrensHead;
        do{
            lastFileNode = lastFileNode->next;
        } while(lastFileNode->next != startFileNode);

        lastFileNode->next = newFileNode;
        newFileNode->next = startFileNode;
    }
    return childrensHead;
}

// Function To Insert New File/Directory Inside cwd (exectuted for cmd 'mkdir docs' or 'create notes.txt')
statusCode insertNewFileNode(fileNode* cwd, char* name, char* command){
    fileNode* newFileNode = createNewFileNode();
    if(fileNodeExists(cwd, name) == true){
        return FILENODE_EXISTS;
    }
    newFileNode->isFile = strcmp(command, "mkdir") == 0 ? false : true;
    newFileNode->parent = cwd;
    strcpy(newFileNode->name, name);
    cwd->childrensHead = addNewFileNodeInCurrentDirectory(cwd->childrensHead, newFileNode);
    return SUCCESS;
}

fileNode* getTail(fileNode* head){
    fileNode* temp = head;
    do{
        temp = temp->next;
    } while(temp->next != head);
    return temp;
}

// Checks whether delete command and given name is valid
statusCode iscurrentDeleteOperationValid(fileNode* curr, char*name, char* command){
    statusCode status = INIT;
    if(curr->isFile == true && strcmp(command, "rmdir") == 0){ // True when rmdir a.txt (a.txt is file)
        status = DIRECTORY_NOT_FOUND;
    } else if(curr->isFile == false && strcmp(command, "delete") == 0){ // True when delete docs (docs is directory)
        status = FILE_NOT_FOUND;
    } else if(curr->isFile == false &&  curr->childrensHead != NULL){ // True when rmdir is given but directory is not empty
        status = DIRECTORY_NOT_EMPTY;
    } else{
        status = SUCCESS;
    }
    return status;
}

// Function to free all blocks allocated for file
void releaseAllocatedBlocks(fileNode* currFileNode, freeBlock** freeBlocksHead){
    int* blocks = currFileNode->blockPointers;
    int blocksLength = currFileNode->reqBlocks;
    for(int i = 0; i < blocksLength; i++){
       *freeBlocksHead = insertAtTailOfFreeBlocks(*freeBlocksHead, blocks[i]);
    }
}

// Function which remover fileNode  (exectuted for cmd 'rmdir docs' or 'delete notes.txt')
statusCode deleteFileNode(fileNode* cwd, char* name, char* command, freeBlock** freeBlocksHead){
    if(cwd == NULL){
        return EMPTY_DIRECTORY;
    }
    fileNode* curr = cwd->childrensHead;
    statusCode status = INIT;  
    if(curr == NULL){
        status = strcmp(command, "rmdir") == 0 ? DIRECTORY_NOT_FOUND : FILE_NOT_FOUND;
        return status;
    }
    fileNode* prev = getTail(cwd->childrensHead);  
    do{
        if(strcmp(curr->name, name) == 0){
            break;
        }
        prev = curr;
        curr = curr->next;
    } while(curr != cwd->childrensHead);

    if(strcmp(curr->name, name) == 0){
        status = iscurrentDeleteOperationValid(curr, name, command);
        if(status == SUCCESS){
            fileNode* nodeToDelete = curr;
            if(nodeToDelete->isFile == true){
                releaseAllocatedBlocks(nodeToDelete, freeBlocksHead); 
            }
            if(prev == curr){
                cwd->childrensHead = NULL;
            } else if(curr == cwd->childrensHead){
                cwd->childrensHead = cwd->childrensHead->next;
                prev->next = cwd->childrensHead;
            } else{
                prev->next = curr->next;
            }
            free(nodeToDelete);
            nodeToDelete = NULL;
            status = SUCCESS;
        }
    } else{
        status = strcmp(command, "rmdir") ? DIRECTORY_NOT_FOUND : FILE_NOT_FOUND;
    }
    return status;
}

fileNode* getNewWorkingDirectory(fileNode* cwd, char *name){
    fileNode* temp = cwd->childrensHead;
    if(temp == NULL){
        return NULL;
    }
    do{
        if(strcmp(temp->name, name) == 0 && temp->isFile == false){
            return temp;
        }
        temp = temp->next;
    } while(temp != cwd->childrensHead);
    printf("Directory not found.\n");
    return cwd;
}

// Function which returns new cwd -> (exectuted for cmd 'cd docs')
fileNode* handleChangeDirectory(fileNode* cwd, char *name){
    fileNode* newCwd = NULL;
    if(strcmp(name, "..") == 0){
        if(cwd->parent != NULL){
            cwd = cwd->parent;
        }
    } else{
        newCwd = getNewWorkingDirectory(cwd, name);
        if(newCwd == NULL){
            printf("Invalid Directory\n");
        } else{
            cwd = newCwd;
        }
    }
    return cwd;
}

// Function to print all subdirectories and files within cwd (exectuted for cmd 'ls')
void displaySubDirectoriesAndFiles(fileNode* cwd){
    fileNode* curr = cwd->childrensHead;
    if(curr){
        do{
            printf("%s", curr->name);
            if(curr->isFile == false){
                printf("/");
            }
            printf("\n");
            curr = curr->next;
        } while(curr != cwd->childrensHead);
    } else{
        printf("(empty)\n");
    }
    printf("\n");
}

// Function which return path of cwd (exectuted for cmd 'pwd')
char* getPathOfCwd(fileNode* cwd){
    char temp[MAX_FILENODE_NAME_LENGTH * 20] = "";
    fileNode* curr = cwd;
    char path[MAX_FILENODE_NAME_LENGTH * 20] = "";
    if(cwd->parent == NULL){
        path[0] = '/';
    }
    while(curr->parent != NULL){
        strcpy(temp, curr->name);
        strcat(temp, "/");
        strcat(temp, path);
        strcpy(path, temp);
        curr = curr->parent;
    }
    char* finalPath = strdup(path);
    finalPath[strlen(finalPath) - 1] = 0;
    return finalPath;
}

void handleFileNodeInsertion(fileNode* cwd, char* name, char *command){
    statusCode status = insertNewFileNode(cwd, name, command);
    if(status == SUCCESS){
        printf("%s '%s' created successfully.\n", strcmp(command, "mkdir") == 0 ? "Directory" : "File", name);
    } else{
        displaystatus(status);
    }
}

void handleFileNodeDeletion(fileNode* cwd, char* name, char *command, freeBlock** freeBlocksHead){
    statusCode status = deleteFileNode(cwd, name, command, freeBlocksHead);
    if(status == SUCCESS){
        printf("%s successfully.\n", strcmp(command, "rmdir") == 0 ? "Directory removed" : "File deleted");
    } else{
        displaystatus(status);
    }
}