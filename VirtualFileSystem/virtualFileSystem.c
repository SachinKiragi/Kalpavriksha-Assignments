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
        freeBlocksHead = deleteAtHeadOfFreeBlocks(freeBlocksHead);
    } while(freeBlocksHead != NULL);
}

void releaseCurrentDirectoryMemory(fileNode* cwd, freeBlock **freeBlocksHead){
    if(cwd == NULL){
        return;
    }
    while(cwd && cwd->childrensHead){
        releaseCurrentDirectoryMemory(cwd->childrensHead, freeBlocksHead); 
    }

    char* command = cwd->isFile ? "delete" : "rmdir";
    char* name = cwd->name;
    if(cwd->parent == NULL){
        free(cwd);
        cwd = NULL;
    } else{
        deleteFileNode(cwd->parent, name, command, freeBlocksHead);
    }
}

void initVirtualFileSystem(){

    printf("$ ./vfs \n");
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    statusCode status = INIT;
    char** disk = initVirtualDisk(); 
    freeBlock* freeBlocksHead = initFreeBlocks();
    fileNode* cwd = initRootDirectory();

    while(1){
        strcmp(cwd->name, "root") != 0 ? printf("%s> ", cwd->name) : printf("/> ");

        char line[MAX_INPUT_LENGTH];
        char name[MAX_FILENODE_NAME_LENGTH];
        char command[MAX_COMMAND_LENGTH];

        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = '\0';
        sscanf(line, "%s %s", command, name);

        if(strcmp(command, "mkdir") == 0 || strcmp(command, "create") == 0){
            handleFileNodeInsertion(cwd, name, command);

        } else if(strcmp(command, "rmdir") == 0 || strcmp(command, "delete") == 0){
            handleFileNodeDeletion(cwd, name, command, &freeBlocksHead);

        } else if(strcmp(command, "ls") == 0){
            displaySubDirectoriesAndFiles(cwd);
            
        } else if(strcmp(command, "cd") == 0){
            cwd = handleChangeDirectory(cwd, name);
            if(cwd->parent != NULL){
                printf("Moved to /%s\n", getPathOfCwd(cwd));
            }
        } else if(strcmp(command, "write") == 0){
            handleWriteCommand(cwd, command, name, line, &freeBlocksHead, disk);

        } else if(strcmp(command, "read") == 0){
            displayFileContent(cwd, name, disk);

        } else if(strcmp(command, "pwd") == 0){
            printf("/%s\n", getPathOfCwd(cwd));

        } else if(strcmp(command, "df") == 0){
            displayDiskInfo(freeBlocksHead);
            
        } else if(strcmp(command, "exit") == 0){
            releaseDiskMemory(disk);
            releaseFreeBlocks(freeBlocksHead);
            releaseCurrentDirectoryMemory(cwd, &freeBlocksHead);
            printf("Memory released. Exiting program...\n");
            break;
        } else{
            printf("Invalid command\n");
        }
    }     
}