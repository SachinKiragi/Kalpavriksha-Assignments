#include "hashMap.h"

int hash(int key, int capacity){
    int bucketIndex = key % capacity;
    return bucketIndex;
}

HashNode* createHashNode(Node* node){
    HashNode* newHashNode = (HashNode*) calloc (1, sizeof(HashNode));
    if(newHashNode){
        newHashNode->key = node->key;
        newHashNode->value = node;
    }
    return newHashNode;
}

HashNode* getHashNodeFromBucketIndex(HashNode* listHead, int key){
    HashNode* currHashNode = listHead;
    while(currHashNode){
        if(currHashNode->key == key){
            return currHashNode;
        }
        currHashNode = currHashNode->next;
    }
    return NULL;
}

HashNode* insertIntoBucketIndex(HashNode* listHead, Node* node){
    HashNode* newHashNode = createHashNode(node);
    if(newHashNode == NULL){
        printf("Error: Failed to allocated memory\n");
        return listHead;
    }
    if(listHead == NULL){
        listHead = newHashNode;
    } else{
        newHashNode->next = listHead;
        listHead = newHashNode;
    }
    return listHead;
}

bool updateHashNode(Node* newNode, HashMap* map){
    int bucketIndex = hash(newNode->key, map->capacity);
    HashNode* currHashNode = map->hashArray[bucketIndex];
    while(currHashNode){
        if(currHashNode->key == newNode->key){
            currHashNode->value = newNode;
            return true;
        }
        currHashNode = currHashNode->next;
    }
    return false;
}

void insertNodeInMap(Node* node, HashMap* map){
    int bucketIndex = hash(node->key, map->capacity);
    if(updateHashNode(node, map)){
        return;
    }
    map->hashArray[bucketIndex] = insertIntoBucketIndex(map->hashArray[bucketIndex], node);
}

bool removeNodeByKey(int key, HashMap* map){
    int bucketIndex = hash(key, map->capacity);
    HashNode* hashNodeToDelete = NULL;
    HashNode* currHashNode = map->hashArray[bucketIndex];
    HashNode* prevHashNode = NULL;
    HashNode* nextHashNode = NULL;

    while(currHashNode){
        if(currHashNode->key == key){
            hashNodeToDelete = currHashNode;
            break;
        }
        prevHashNode = currHashNode;
        currHashNode = currHashNode->next;
    }

    if(currHashNode == NULL){
        return false;
    }
    
    nextHashNode = currHashNode->next;
    if(prevHashNode == NULL){
        map->hashArray[bucketIndex] = map->hashArray[bucketIndex]->next;
    } else{
        prevHashNode->next = nextHashNode;
    }
    free(hashNodeToDelete);
    hashNodeToDelete = NULL;
    return true;
}

HashNode* getHashNode(int key, HashMap* map){
    int bucketIndex = hash(key, map->capacity);
    HashNode* tempHashNode = getHashNodeFromBucketIndex(map->hashArray[bucketIndex], key);
    return tempHashNode;
}

Node* getNodeFromMap(int key, HashMap* map){
    HashNode* tempHashNode = getHashNode(key, map);
    if(tempHashNode == NULL){
        return NULL;
    }
    Node* tempNode = tempHashNode->value;
    return tempNode;
}

void releaseCurrentBucketList(HashNode* listHead){
    HashNode* currHashNode = listHead;
    HashNode* hashNodeToDelete = NULL;

    while(currHashNode){
        hashNodeToDelete = currHashNode;
        currHashNode = currHashNode->next;
        free(hashNodeToDelete);
        hashNodeToDelete = NULL;
    }
}

void releaseMapMemory(HashMap* map){
    for(int i = 0; i < map->capacity; i++){
        releaseCurrentBucketList(map->hashArray[i]);
    }
    free(map);
    map = NULL;
}

HashMap* initMap(int capicity){

    HashMap* map = (HashMap*) calloc (1, sizeof(HashMap));
    if(map == NULL){
        return NULL;
    }
    map->capacity = capicity;
    map->insertNodeInMap = &insertNodeInMap;
    map->removeNodeByKey = &removeNodeByKey;
    map->getNodeFromMap = &getNodeFromMap;
    map->releaseMapMemory = &releaseMapMemory;

    map->hashArray = (HashNode**) malloc (map->capacity * sizeof(HashNode*));
    if(map->hashArray == NULL){
        map->releaseMapMemory(map);
        return NULL;
    }
    for(int i = 0; i < map->capacity; i++){
        map->hashArray[i] = (HashNode*) calloc (1, sizeof(HashNode));
        if(map->hashArray[i] == NULL){
            map->releaseMapMemory(map);
            return NULL;
        }
    }
    return map;
}