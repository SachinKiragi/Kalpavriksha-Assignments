#include "lruCache.h"

void handlePut(Queue* queue, HashMap* map, int key, char* value){
    Node* existingNode = map->getNodeFromMap(key, map);

    if(existingNode){
        queue->removeNode(existingNode, queue);
    }

    if(queue->capacity == queue->currSize){
        Node* deletedNode = queue->removeNode(queue->rear, queue);
        map->removeNodeByKey(deletedNode->key, map);
        free(deletedNode);
        deletedNode = NULL;
    }

    Node* insertedNode = queue->insertNode(key, value, queue);
    if(insertedNode == NULL){
        printf("Error: Failed to allocated memory\n");
        return;
    }
    map->insertNodeInMap(insertedNode, map);
}

void handleGet(Queue* queue, HashMap* map, int key){
    Node* node = map->getNodeFromMap(key, map);
    if(node){
        Node* deletedNode = queue->removeNode(node, queue);
        Node* insertedNode = queue->insertNode(node->key, node->value, queue);
        map->insertNodeInMap(insertedNode, map);
        printf("\n%s\n\n", node->value);
        free(deletedNode);
        deletedNode = NULL;
    } else{
        printf("\nNULL\n\n");
    }
}

int initLru(){
    printf("LRU Initiated\n");
    char line[MAX_INPUT_LENGTH];
    int argsLen = 0;
    char cmd[MAX_ARG_LENGTH];
    int key;
    char val[MAX_ARG_LENGTH];

    int capacity;

    while(argsLen != 2){
        fgets(line, sizeof(line), stdin);
        argsLen = sscanf(line, "%s %d", cmd, &capacity);
        if((strcmp(cmd, "createCache") != 0 ) || argsLen != 2 || capacity < MIN_CAPACITY || capacity > MAX_CAPACITY){
            printf("Invalid Input!\n");
            argsLen = 0;
        }
    } 
    Queue* queue = initQueue(capacity);
    HashMap* map = initMap(capacity);

    if(queue == NULL){
        printf("Error: Failed to allocated memory for map\n");
        return 0;
    }
    if(map == NULL){
        queue->releaseQueueMemory(queue);
        printf("Error: Failed to allocated memory for map\n");
        return 0;
    }

    int ch;
    while(1){
        fgets(line, sizeof(line), stdin);
        int argsLen = sscanf(line, "%s %d %s", cmd, &key, val);
        if(strcmp(cmd, "exit") == 0){
            queue->releaseQueueMemory(queue);
            map->releaseMapMemory(map);
            printf("Releasing memory...\nExiting\n");
            return 0;
        } else if(argsLen < 2 || argsLen > 3){
            printf("Inavlid Input\n");
        } else if(strcmp(cmd, "put") == 0){
            handlePut(queue, map, key, val);
        } else if(strcmp(cmd, "get") == 0){
            handleGet(queue, map, key);
        } else{
            printf("Invalid Commands! Try Again\n");
        }
    }
    return 0;
}