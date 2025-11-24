#ifndef HASHMAP_H
#define HASHMAP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include "../macros.h"
#include "../models/models.h"


int hash(int key, int capacity);

HashNode* createHashNode(Node* node);

HashNode* getHashNodeFromBucketIndex(HashNode* listHead, int key);

HashNode* insertIntoBucketIndex(HashNode* listHead, Node* node);
bool updateHashNode(Node* newNode, HashMap* map);
void insertNodeInMap(Node* node, HashMap* map);

void removeNodeByKey(int key, HashMap* map);

HashNode* getHashNode(int key, HashMap* map);
Node* getNodeFromMap(int key, HashMap* map);

void releaseMapMemory(HashMap* map);
HashMap* initMap(int capicity);

#endif