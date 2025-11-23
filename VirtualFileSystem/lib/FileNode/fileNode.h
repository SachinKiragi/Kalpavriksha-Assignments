#ifndef FILENODE_H
#define FILENODE_H

#include "../macros.h"
#include "../FreeBlock/freeBlock.c"
#include<string.h>

typedef struct VfsState VfsState;

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
bool fileNodeExists(fileNode*, char*);
fileNode* createNewFileNode();
fileNode* getFileNode(fileNode*, char*);
fileNode* addNewFileNodeInCurrentDirectory(fileNode*, fileNode*);
statusCode insertNewFileNode(fileNode*, char*, char*);
fileNode* getTail(fileNode*);
statusCode iscurrentDeleteOperationValid(fileNode*, char*, char*);
void releaseAllocatedBlocks(fileNode*, freeBlock**);
statusCode deleteFileNode(VfsState*, char*, char*);
fileNode* getNewWorkingDirectory(fileNode*, char*);
void handleChangeDirectory(VfsState*, char*);
void displaySubDirectoriesAndFiles(fileNode*);
char* getPathOfCwd(fileNode*);
void handleFileNodeInsertion(VfsState*, char*, char*);
void handleFileNodeDeletion(VfsState*, char*, char*);

#endif