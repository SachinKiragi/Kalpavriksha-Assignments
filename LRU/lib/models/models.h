#ifndef MODELS_H
#define MODELS_H

#include "../macros.h"

typedef struct Node Node;
typedef struct Queue Queue;
typedef struct HashNode HashNode;
typedef struct HashMap HashMap;

typedef struct Node{
    int key;
    char value[MAX_VAL_LENGHT];
    struct Node* next;
    struct Node* prev;
} Node;

struct Queue{
    int capacity;
    int currSize;
    Node* front;
    Node* rear;   
    Node* (*insertNode)(int, char*, Queue*);
    Node* (*removeCurrentNode)(Node*, Queue*);
    Node* (*releaseQueueMemory)(Queue*);
};

typedef struct HashNode{
    Node* node;
    struct HashNode* next;
    struct HashNode* prev;
} HashNode;

typedef struct HashMap{
    HashNode** hashArray;
    int capacity;
    void (*insertNodeInMap)(Node*, HashMap*);
    Node* (*getNodeFromMap)(int, HashMap*);
    void (*modifyMap)(Node*, Node*, HashMap*);
    void (*removeNodeByKey)(int, HashMap*);
    void (*releaseMapMemory)(HashMap*);
} HashMap;

#endif