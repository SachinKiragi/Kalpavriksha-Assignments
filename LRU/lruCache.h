#ifndef LRU_H
#define LRU_H

#include "lib/customQueue/customQueue.c"
#include "lib/hashMap/hashMap.c"

void handlePut(Queue* queue, HashMap* hashMap, int key, char* value);
void handleGet(Queue* queue, HashMap* hashMap, int key);
int initLru();

#endif