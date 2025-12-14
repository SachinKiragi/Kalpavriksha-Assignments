#include "customQueue.h"

StatusCode insertInQueue(int pid, Queue* q){
    QueueNode* newQueueNode = (QueueNode*) calloc (1, sizeof(QueueNode));
    if(newQueueNode == NULL){
        return ERR;
    }
    newQueueNode->data = pid;
    QueueNode* curr = q->head;
    QueueNode* prev = NULL;

    while(curr && curr->next){
        curr = curr->next;
    }
    if(curr == NULL){
        q->head = newQueueNode;
    } else{
        curr->next = newQueueNode;
    }
    q->size++;
    return SUCCESS;
}

StatusCode removeFromQueue(Queue* q){
    if(q->head == NULL){
        return EMPTY;
    }
    QueueNode* nodeToDelete = q->head;
    q->head = q->head->next;
    q->size--;
    free(nodeToDelete);
    nodeToDelete = NULL;
    return SUCCESS;
}

int getFrontFromQueue(Queue* q){
    if(q->head){
        return q->head->data;
    } else{
        return -1;
    }
}

void freeQueueMemory(Queue* q){
    QueueNode* curr = q->head;
    QueueNode* nodeToDelete = NULL;
    while(curr){
        nodeToDelete = curr;
        curr = curr->next;
        free(nodeToDelete);
        nodeToDelete = NULL;
    }
    free(q);
    q = NULL;
}

Queue* initQueue(){
    Queue* q = (Queue*) calloc (1, sizeof(Queue));
    if(q == NULL){
        return q;
    }
    q->insertInQueue = &insertInQueue;
    q->removeFromQueue = &removeFromQueue;
    q->getFrontFromQueue = &getFrontFromQueue;
    q->freeQueueMemory = &freeQueueMemory;
    return q;
}