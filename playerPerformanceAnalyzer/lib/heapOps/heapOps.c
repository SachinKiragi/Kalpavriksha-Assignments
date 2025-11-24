#include "heapOps.h"

typedef struct PlayerInfo PlayerInfo;

void heapify(PlayerInfo** heap, int n, int i){
    int largest = i;

    int leftChild = 2 * i + 1;
    int rightChild = 2 * i + 2;
    if(leftChild < n && heap[leftChild]->performanceIndex > heap[largest]->performanceIndex){
        largest = leftChild;
    }
    if(rightChild < n && heap[rightChild]->performanceIndex > heap[largest]->performanceIndex){
        largest = rightChild;
    }
    if(i != largest){
        PlayerInfo* temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;
        heapify(heap, n, largest);
    }
}


PlayerInfo* pop(PlayerInfo** heap, int *heapSize){
    if(heapSize == 0){
        return NULL;
    }
    PlayerInfo* topPlayer = heap[0];
    PlayerInfo* temp = heap[0];
    heap[0] = heap[*heapSize - 1];
    heap[*heapSize - 1] = temp;
    (*heapSize)--;
    heapify(heap, *heapSize, 0);
    return topPlayer;
}


void push(PlayerInfo** heap, int *heapSize, PlayerInfo* newPlayer){
    if(newPlayer == NULL){
        return;
    }
    heap[*heapSize] = newPlayer;
    (*heapSize)++;
    int index = (*heapSize) - 1;
    while(index > 0 && heap[index]->performanceIndex > heap[(index -1) / 2]->performanceIndex){
        PlayerInfo* temp = heap[index];
        heap[index] = heap[(index -1) / 2];
        heap[(index - 1) / 2] = temp;
        index = (index - 1) / 2;
    } 
}


PlayerInfo** initHeap(TeamInfo** myTeams, int teamCount, int role){
    PlayerInfo** heap = (PlayerInfo**) malloc (teamCount * sizeof(PlayerInfo*));
    int heapSize = teamCount;
    for(int i = 0; i < teamCount; i++){
        heap[i] = myTeams[i]->headPlayersByRole[role];
    }
    for(int i = heapSize - 1; i >=0; i--){
        heapify(heap, heapSize, i);
    }
    return heap;
}