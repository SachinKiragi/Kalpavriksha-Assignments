#include "customPcbMap.h"

int getIndex(int key, PcbHashMap* pcbMap){
    return (key % pcbMap->hashArraySize);
}

HashNode* getHashNode(PcbNode* pcbNodeToInsert){
    HashNode* newHashNode = (HashNode*) calloc (1, sizeof(HashNode));
    if(newHashNode == NULL){
        return newHashNode;
    }
    newHashNode->key = pcbNodeToInsert->pid;
    newHashNode->value = pcbNodeToInsert;
    return newHashNode;
}

StatusCode insertPcbNodeInMap(PcbNode* pcbNodeToInsert, PcbHashMap* pcbMap){
    HashNode* newHashNode = getHashNode(pcbNodeToInsert);
    if(newHashNode == NULL){
        return ERR;
    }
    int index = getIndex(pcbNodeToInsert->pid, pcbMap);
    newHashNode->next = pcbMap->hashArray[index];
    pcbMap->hashArray[index] = newHashNode;
    return SUCCESS;
}

PcbNode* getPcbFromMap(int pid, PcbHashMap* pcbMap){
    int index = getIndex(pid, pcbMap);
    HashNode* curr = pcbMap->hashArray[index];
    while(curr){
        if(curr->key == pid){
            break;
        }
        curr = curr->next;
    }
    return curr ? curr->value : NULL;
}

void printMap(PcbHashMap* pcbMap){
    printf("+-----+---------+-----+-----+----------------+------------+---------+\n");
    printf("| PID | Name    | CPU | IO  | Status         | Turnaround | Waiting |\n");
    printf("+-----+---------+-----+-----+----------------+------------+---------+\n");

    HashNode** arr = pcbMap->hashArray;
    for(int i = 0; i < pcbMap->hashArraySize; i++){
        HashNode* curr = pcbMap->hashArray[i];
        if(curr == NULL) continue;
        while(curr){
            printNode(curr->value);
            printf(" ");
            curr = curr->next;
        }
        printf("\n");
    }
    printf("+-----+---------+-----+-----+----------------+------------+---------+\n");
}

void freeMapMemory(PcbHashMap* pcbMap){
    HashNode* curr = NULL;
    HashNode* nodeToDelete = NULL;
    for(int i = 0; i < pcbMap->hashArraySize; i++){
        curr = pcbMap->hashArray[i];
        while(curr){
            nodeToDelete = curr;
            curr = curr->next;
            free(nodeToDelete);
            nodeToDelete = NULL;
        }
    }
    free(pcbMap);
    pcbMap = NULL;
}

PcbHashMap* initMap(){
    PcbHashMap* pcbMap = (PcbHashMap*) calloc (1, sizeof(PcbHashMap));
    if(pcbMap == NULL){
        return pcbMap;
    }
    pcbMap->hashArraySize = MAX_HASH_ARRAY_SIZE;
    pcbMap->hashArray = (HashNode**) calloc (pcbMap->hashArraySize, sizeof(HashNode*));
    if(pcbMap->hashArray == NULL){
        freeMapMemory(pcbMap);
        return pcbMap;
    }
    pcbMap->insertPcbNodeInMap = &insertPcbNodeInMap;
    pcbMap->getPcbFromMap = &getPcbFromMap;
    pcbMap->printMap = &printMap;
    pcbMap->freeMapMemory = &freeMapMemory;
    return pcbMap;
}