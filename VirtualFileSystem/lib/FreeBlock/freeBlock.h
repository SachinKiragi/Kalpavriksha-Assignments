#ifndef FREEBLOCK_H
#define FREEBLOCK_H

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

typedef struct freeBlock{
    int index;
    struct freeBlock* prev;
    struct freeBlock* next;
} freeBlock;

freeBlock* createNewFreeBlock(int); 
freeBlock* insertAtTailOfFreeBlocks(freeBlock*, int);
freeBlock* deleteAtHeadOfFreeBlocks(freeBlock*);
freeBlock* getFreeBlock(freeBlock**);
int getNumberOfFreeBlocks(freeBlock*);

#endif