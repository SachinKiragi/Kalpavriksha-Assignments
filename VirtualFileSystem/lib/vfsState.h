#ifndef VFS_STATE_H
#define VFS_STATE_H

#include "FileNode/fileNode.h"
#include "FreeBlock/freeBlock.h"

typedef struct VfsState{
    fileNode* cwd;
    freeBlock* freeBlocksHead;
    char** disk;
} VfsState;

#endif
