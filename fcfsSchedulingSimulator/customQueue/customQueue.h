#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H

#include<stdio.h>
#include "../statusCodes.h"

typedef struct QueueNode QueueNode;
typedef struct Queue Queue;

typedef struct QueueNode{
    int data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue{
    QueueNode* head;
    int size;
    StatusCode (*insertInQueue)(int, Queue*);
    StatusCode (*removeFromQueue)(Queue*);
    int (*getFrontFromQueue)(Queue*);
    void (*printQueue)(Queue*);
    void (*freeQueueMemory)(Queue*);
} Queue;


StatusCode insertInQueue(int, Queue*);
StatusCode removeFromQueue(Queue*);
int getFrontFromQueue(Queue*);
Queue* initQueue();
void freeQueueMemory(Queue*);

#endif