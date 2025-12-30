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
    Node* prevNode = nodeToDelete->prev;
    Node* nextNode = nodeToDelete->next;
    if(prevNode == NULL){
        queue->front = nextNode;
    } else{
        prevNode->next = nextNode;
    }

    if(nextNode == NULL){
        queue->rear = prevNode;
    } else{
        nextNode->prev = prevNode;
    }    
    queue->currSize--;
    return nodeToDelete;
}

void releaseQueueMemory(Queue* queue){
    Node* currNode = queue->front;
    Node* nodeToDelete = NULL;
    while(currNode){
        nodeToDelete = currNode;
        currNode = currNode->next;
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