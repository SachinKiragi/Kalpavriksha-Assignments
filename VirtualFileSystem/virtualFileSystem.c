#include "virtualFileSystem.h"

fileNode* initRootDirectory(){
    fileNode* root = (fileNode*)calloc(1, sizeof(fileNode));
    strcpy(root->name, "root");
    root->isFile = false;
    return root;
}

char** initVirtualDisk(){
    char** disk = (char**)malloc(MAX_NUMBER_OF_BLOCKS * sizeof(char*));
    for(int i = 0; i < MAX_NUMBER_OF_BLOCKS; i++){
        disk[i] = (char*)calloc(MAX_BLOCK_SIZE, sizeof(char));
    }
    return disk;
}

freeBlock* initFreeBlocks(){
    freeBlock* head = NULL;
    for(int i = 0; i < MAX_NUMBER_OF_BLOCKS; i++){
        head = insertAtTailOfFreeBlocks(head, i);
    }
    return head;
}

void initMemory(VfsState* currVfsState){
    currVfsState->disk = initVirtualDisk();
    currVfsState->freeBlocksHead = initFreeBlocks();
    currVfsState->cwd = initRootDirectory();   
}

void releaseDiskMemory(char** disk){
    for(int i = 0; i < MAX_NUMBER_OF_BLOCKS; i++){
        free(disk[i]);
        disk[i] = NULL;
    }
    free(disk);
    disk = NULL;
}

void releaseFreeBlocks(freeBlock* freeBlocksHead){
    do{
        freeBlock* temp = freeBlocksHead;
        freeBlocksHead = deleteAtHeadOfFreeBlocks(freeBlocksHead);
        free(temp);
        temp = NULL;
    } while(freeBlocksHead != NULL);
}

void releaseCurrentDirectoryMemory(VfsState* currVfsState){
    fileNode* cwd = currVfsState->cwd;
    if(cwd == NULL){
        return;
    }
    while(cwd && cwd->childrensHead){
        currVfsState->cwd = cwd->childrensHead;
        releaseCurrentDirectoryMemory(currVfsState);
    }

    char* command = cwd->isFile ? "delete" : "rmdir";
    char* name = cwd->name;
    if(cwd->parent == NULL){
        free(cwd);
        cwd = NULL;
    } else {
        currVfsState->cwd = currVfsState->cwd->parent;
        deleteFileNode(currVfsState, name, command);
    }
}

void releaseMemory(VfsState* currVfsState){
    releaseDiskMemory(currVfsState->disk);
    releaseCurrentDirectoryMemory(currVfsState);
    releaseFreeBlocks(currVfsState->freeBlocksHead);
}

fileNode* getRoot(fileNode* cwd){
    if(cwd->parent == NULL){
        return cwd;
    }
    return getRoot(cwd->parent);
}

void initVirtualFileSystem(){

    printf("$ ./VfsState \n");
    printf("Compact VfsState - ready. Type 'exit' to quit.\n");

    statusCode status = INIT;

    VfsState *currVfsState = (VfsState*)malloc(sizeof(VfsState));
    initMemory(currVfsState);

    while(1){
        strcmp(currVfsState->cwd->name, "root") != 0 ? printf("%s> ", currVfsState->cwd->name) : printf("/> ");

        char line[MAX_INPUT_LENGTH];
        char name[MAX_FILENODE_NAME_LENGTH];
        char command[MAX_COMMAND_LENGTH];

        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = '\0';
        sscanf(line, "%s %s", command, name);

        if(strcmp(command, "mkdir") == 0 || strcmp(command, "create") == 0){
            handleFileNodeInsertion(currVfsState, name, command);

        } else if(strcmp(command, "rmdir") == 0 || strcmp(command, "delete") == 0){
            handleFileNodeDeletion(currVfsState, name, command);

        } else if(strcmp(command, "ls") == 0){
            displaySubDirectoriesAndFiles(currVfsState->cwd);
            
        } else if(strcmp(command, "cd") == 0){
            handleChangeDirectory(currVfsState, name);
            if(currVfsState->cwd->parent != NULL){
                printf("Moved to /%s\n", getPathOfCwd(currVfsState->cwd));
            }
        } else if(strcmp(command, "write") == 0){
            handleWriteCommand(currVfsState, command, name, line);

        } else if(strcmp(command, "read") == 0){
            displayFileContent(currVfsState, name);

        } else if(strcmp(command, "pwd") == 0){
            printf("/%s\n", getPathOfCwd(currVfsState->cwd));

        } else if(strcmp(command, "df") == 0){
            displayDiskInfo(currVfsState);
            
        } else if(strcmp(command, "exit") == 0){
            currVfsState->cwd = getRoot(currVfsState->cwd);
            releaseMemory(currVfsState);
            printf("Memory released. Exiting program...\n");
            break;
        } else{
            printf("Invalid command\n");
        }
    }     
}