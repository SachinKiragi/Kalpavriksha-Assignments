#ifndef DISKOPS_H
#define DISKOPS_H

#include "../FileNode/fileNode.c"
#include "../vfsState.h"

char* getContent(char*);
void displayFileContent(VfsState*, char*);
statusCode appendIntoDiskHelper(char*, fileNode*, char**, int);
statusCode appendContentIntoDisk(VfsState*, char*, char*);
void handleWriteCommand(VfsState*, char*, char*, char*);
void displayDiskInfo(VfsState*);

#endif