#include "freeBlock.h"

freeBlock* createNewFreeBlock(int index){
    freeBlock* newFreeBlock = (freeBlock*)calloc(1, sizeof(freeBlock));
    newFreeBlock->index = index;
    newFreeBlock->prev = NULL;
    newFreeBlock->next = NULL;
    return newFreeBlock;
}

freeBlock* insertAtTailOfFreeBlocks(freeBlock* head, int index){
    freeBlock* newFreeBlock = createNewFreeBlock(index);
    if(head == NULL){
        head = newFreeBlock;
    } else{
        freeBlock* curr = head;
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = newFreeBlock;
        newFreeBlock->prev = curr;
    }
    return head;
}

freeBlock* deleteAtHeadOfFreeBlocks(freeBlock* head){
    if(head == NULL){
        return head;
    } else {
        head = head->next;
        if(head != NULL){
            head->prev = NULL;
        }
    }
    return head;
}

freeBlock* getFreeBlock(freeBlock** head){
    if(*head == NULL){
        return *head;
    }
    freeBlock* tempFreeBlock = *head;
    *head = deleteAtHeadOfFreeBlocks(*head);
    return tempFreeBlock;
}

int getNumberOfFreeBlocks(freeBlock* head){
    freeBlock* curr = head;
    int noOfFreeBlocks = 0;
    while(curr){
        noOfFreeBlocks++;
        curr = curr->next;
    }
    return noOfFreeBlocks;
}