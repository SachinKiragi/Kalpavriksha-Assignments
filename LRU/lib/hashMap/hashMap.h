#ifndef HASHMAP_H
#define HASHMAP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../macros.h"
#include "../models/models.h"


int hash(int key, int capacity);

HashNode* createHashNode(Node* node);

HashNode* getHashNodeFromBucketIndex(HashNode* listHead, int key);
HashNode* getHashNode(int key, HashMap* map);

HashNode* insertIntoBucketIndex(HashNode* listHead, Node* node);
void insertNodeInMap(Node* node, HashMap* map);

void removeNodeByKey(int key, HashMap* map);
void modifyMap(Node* oldNode, Node* newNode, HashMap* map);

Node* getNodeFromMap(int key, HashMap* map);

void releaseMapMemory(HashMap* map);
HashMap* initMap(int capicity);

#endif