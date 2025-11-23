#include "diskOps.h"

char* getContent(char* line){
    int i = 0;
    int j = 0;
    char* content = (char*)calloc(MAX_CONTENT_LENGTH, sizeof(char));
    while(i < strlen(line) && line[i] != '"'){
        i++;
    }
    i++;
    while(i < strlen(line) && line[i] != '"'){
        content[j++] = line[i++];
    }
    content[j] = 0;
    return content;
}


void displayFileContent(VfsState* currVfsState, char* name){
    fileNode* cwd = currVfsState->cwd;
    fileNode* currFileNode = getFileNode(cwd, name);
    char** disk = currVfsState->disk;
    if(currFileNode == NULL){
        printf("File not found!\n");
        return;
    }

    if(currFileNode->reqBlocks == 0){
        printf("(empty)\n");
        return;
    }

    int *blocks = currFileNode->blockPointers;
    int reqBlocks = currFileNode->reqBlocks;

    for(int i = 0; i < reqBlocks; i++){
        for(int j = 0; j < MAX_BLOCK_SIZE; j++){
            printf("%c", disk[blocks[i]][j]);
            if(disk[blocks[i]][j] == '\0'){
                break;
            }
        }
    }
    printf("\n");
}


statusCode appendIntoDiskHelper(char* content, fileNode* currFileNode, char **disk, int reqBlocks){
    int *blocks = currFileNode->blockPointers;
    int indexForContent = 0;  
    int i = 0, j = 0;
    for(i = 0; i < reqBlocks && content[indexForContent] != '\0'; i++){
        for(j = 0; j < MAX_BLOCK_SIZE && content[indexForContent] != '\0'; j++){
            disk[blocks[currFileNode->reqBlocks + i]][j] = content[indexForContent++];
        }
        if(content[indexForContent] == '\0'){
            if(j == MAX_BLOCK_SIZE){
                i++;
            }
            disk[blocks[currFileNode->reqBlocks + i]][j] = '\0';
            break;
        }
    }
    currFileNode->reqBlocks += reqBlocks;
    return SUCCESS;
}


statusCode appendContentIntoDisk(VfsState* currVfsState, char* fileName, char* content){
    fileNode* cwd = currVfsState->cwd;
    freeBlock** freeBlocksHead = &currVfsState->freeBlocksHead;
    char** disk = currVfsState->disk;

    fileNode* currFileNode = getFileNode(cwd, fileName);

    statusCode status = INIT;
    if(currFileNode == NULL || currFileNode->isFile == false){
        status = FILE_NOT_FOUND;
        return status;
    }

    int contentLength = strlen(content) + 1;
    int reqBlocks = contentLength / MAX_BLOCK_SIZE + ((contentLength % MAX_BLOCK_SIZE )!= 0);
    int noOfFreeBlocks = getNumberOfFreeBlocks(*freeBlocksHead);

    if(reqBlocks > noOfFreeBlocks){
        status = DISK_IS_FULL;
        return status;
    }
    for(int i = 0; i < reqBlocks; i++){
        freeBlock* newFreeBlock = getFreeBlock(freeBlocksHead);
        currFileNode->blockPointers[currFileNode->reqBlocks + i] = newFreeBlock->index;    
    }

    return appendIntoDiskHelper(content, currFileNode, disk, reqBlocks);
}


void handleWriteCommand(VfsState* currVfsState, char* command, char *name, char* line){

    while(strlen(line) > MAX_CONTENT_LENGTH){
        printf("Content Legth exceeded\n");
        return;
    }
    char* content = getContent(line);
    content[strlen(content)] = 0;
    statusCode status = INIT;
    status = appendContentIntoDisk(currVfsState, name, content);
    if(status == SUCCESS){
        printf("Data written successfully (size=%d bytes). \n", (int)strlen(content)+1);
    } else{
        displaystatus(status);
    }
}


void displayDiskInfo(VfsState* currVfsState){
    freeBlock* freeBlocksHead = currVfsState->freeBlocksHead;
    int noOfFreeBlocks = getNumberOfFreeBlocks(freeBlocksHead);
    int totalNumberOfBlocks = MAX_NUMBER_OF_BLOCKS;
    int usedBlocks = totalNumberOfBlocks - noOfFreeBlocks;
    float diskUsage = ((float)usedBlocks / (float)totalNumberOfBlocks) * 100.0;

    printf("Total Blocks: %d\n", totalNumberOfBlocks);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", noOfFreeBlocks);
    printf("Disk Usage: %.2f%%\n", diskUsage);
}