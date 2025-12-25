#ifndef SIMULATOR_H
#define SIMULATOR_H

#include<stdio.h>
#include <windows.h>

#include "customPcbMap/customPcbMap.c"
#include "customQueue/customQueue.c"

#define MAX_NAME_LEN 100
#define MAX_INPUT_LEN 200

int timer = 0;

typedef struct ProcessQueues{
    Queue* readyQue;
    Queue* waitingQue;
    Queue* terminatedQue;
} ProcessQueues;

void clock();
void updateTurnArroundTime(PcbHashMap*, ProcessQueues*);
void handleReadyQueue(PcbHashMap*, ProcessQueues*);
void handleWaitingQueue(PcbHashMap*, ProcessQueues*);
void startSimulation(PcbHashMap*, ProcessQueues*);
ProcessQueues* initProcessQueues();
int initFcfsScheduler();

#endif