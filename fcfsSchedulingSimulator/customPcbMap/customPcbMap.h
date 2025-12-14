#ifndef MAP_H
#define MAP_H

#include<stdbool.h>
#include "../statusCodes.h"
#include "../pcb/pcb.c"

#define MAX_HASH_ARRAY_SIZE 5

typedef struct PcbHashMap PcbHashMap;
typedef struct HashNode HashNode;

typedef struct HashNode{
    int key;
    PcbNode* value;
    struct HashNode* next;
} HashNode;

typedef struct PcbHashMap{
    HashNode** hashArray;
    int hashArraySize;
    StatusCode (*insertPcbNodeInMap)(PcbNode*, PcbHashMap*);
    PcbNode* (*getPcbFromMap)(int, PcbHashMap*);
    void (*printMap)(PcbHashMap*);
    void (*freeMapMemory)(PcbHashMap*);
} PcbHashMap;

int getIndex(int, PcbHashMap*);
HashNode* getHashNode(PcbNode*);
StatusCode insertPcbNodeInMap(PcbNode*, PcbHashMap*);
PcbNode* getPcbFromMap(int, PcbHashMap*);
void printMap(PcbHashMap*);
PcbHashMap* initMap();
void freeMapMemory(PcbHashMap*);

#endif