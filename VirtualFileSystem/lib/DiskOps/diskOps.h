#ifndef DISKOPS_H
#define DISKOPS_H

#include "../fileNode/fileNode.c"

char* getContent(char*);
void displayFileContent(fileNode*, char*, char**);
statusCode appendIntoDiskHelper(char*, fileNode*, char**, int);
statusCode appendContentIntoDisk(fileNode*, char*, char*, freeBlock**, char**);
void handleWriteCommand(fileNode*, char*, char*, char*, freeBlock**, char**);
void displayDiskInfo(freeBlock*);

#endif