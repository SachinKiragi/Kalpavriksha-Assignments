#ifndef VFS_H
#define VFS_H

#include "lib/DiskOps/diskOps.c"

fileNode* initRootDirectory();
char** initVirtualDisk();
freeBlock* initFreeBlocks();
void releaseDiskMemory(char**);
void releaseFreeBlocks(freeBlock*);
void releaseCurrentDirectoryMemory(VfsState*);
void initVirtualFileSystem();
void releaseMemory(VfsState*);
void initMemory(VfsState*);
fileNode* getRoot(fileNode*);
#endif