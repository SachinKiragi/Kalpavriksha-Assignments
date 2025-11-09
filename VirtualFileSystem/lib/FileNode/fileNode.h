#ifndef FILENODE_H
#define FILENODE_H

#include "../macros.h"
#include "../FreeBlock/freeBlock.c"
#include<string.h>

typedef enum{
    FILENODE_EXISTS,
    FILE_NOT_FOUND,
    DIRECTORY_NOT_FOUND,
    DIRECTORY_NOT_EMPTY,
    EMPTY_DIRECTORY,
    DISK_IS_FULL,
    SUCCESS,
    INIT,
} statusCode;

typedef struct fileNode{
    bool isFile;
    int reqBlocks;
    char name[MAX_FILENODE_NAME_LENGTH];
    struct fileNode* parent;
    struct fileNode* next;
    struct fileNode* prev;
    int blockPointers[MAX_NUMBER_OF_BLOCKS];
    struct fileNode* childrensHead;
} fileNode;


void displaystatus(statusCode);
bool doesFileNodeExistsAlready(fileNode*, char*);
fileNode* createNewFileNode();
fileNode* getFileNode(fileNode*, char*);
fileNode* addNewFileNodeInCurrentDirectory(fileNode*, fileNode*);
statusCode insertNewFileNode(fileNode*, char*, char*);
fileNode* getTail(fileNode*);
statusCode iscurrentDeleteOperationValid(fileNode*, char*, char*);
void releaseAllocatedBlocks(fileNode*, freeBlock**);
statusCode deleteFileNode(fileNode* cwd, char*, char*, freeBlock**);
fileNode* getNewWorkingDirectory(fileNode*, char*);
fileNode* handleChangeDirectory(fileNode*, char*);
void displaySubDirectoriesAndFiles(fileNode*);
char* getPathOfCwd(fileNode*);
void handleFileNodeInsertion(fileNode*, char*, char*);
void handleFileNodeDeletion(fileNode*, char*, char*, freeBlock**);
fileNode* initRootDirectory();
char** initVirtualDisk();
freeBlock* initFreeBlocks();

#endif