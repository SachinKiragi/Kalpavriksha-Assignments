#ifndef QUEUE_H
#define QUEUE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../models/models.h"

#define MAX_VAL_LENGHT 100

Node* createNewNode(int key, char* value);
Node* insertNode(int key, char* value, Queue *queue);
Node* removeCurrentNode(Node* nodeToDelete, Queue* queue);
Node* releaseQueueMemory(Queue* queue);
Queue* initQueue(int capacity);

#endif