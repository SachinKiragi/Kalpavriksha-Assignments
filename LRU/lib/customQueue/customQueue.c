#include "customQueue.h"

Node* createNewNode(int key, char* value){
    Node* newNode = (Node*)calloc(1, sizeof(Node));
    if(newNode == NULL){
        return NULL;
    }
    newNode->key = key;
    strcpy(newNode->value, value);
    return newNode;
}

Node* insertNode(int key, char* value, Queue *queue){
    Node* newNode = createNewNode(key, value);
    if(newNode == NULL){
        return NULL;
    }
    if(queue->front == NULL){
        queue->front = newNode;
        queue->rear = newNode;
    } else{
        newNode->next = queue->front;
        queue->front->prev = newNode;
        queue->front = newNode;
    }
    queue->currSize++;
    return newNode;
}

Node* removeNode(Node* nodeToDelete, Queue* queue){
    Node* prev = nodeToDelete->prev;
    Node* next = nodeToDelete->next;
    if(prev == NULL){
        queue->front = next;
    } else{
        prev->next = next;
    }

    if(next == NULL){
        queue->rear = prev;
    } else{
        next->prev = prev;
    }    
    queue->currSize--;
    return nodeToDelete;
}

Node* releaseQueueMemory(Queue* queue){
    Node* curr = queue->front;
    Node* nodeToDelete = NULL;
    while(curr){
        nodeToDelete = curr;
        curr = curr->next;
        free(nodeToDelete);
        nodeToDelete = NULL;
    }
}

Queue* initQueue(int capacity){
    Queue* queue = (Queue*) calloc (1, sizeof(Queue));
    if(queue == NULL){
        return NULL;
    }
    queue->insertNode = &insertNode;
    queue->removeNode = &removeNode;
    queue->releaseQueueMemory = &releaseQueueMemory;
    queue->capacity = capacity;
    return queue;
}