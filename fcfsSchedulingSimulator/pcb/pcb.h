#ifndef PCB_H
#define PCB_H

#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define MAX_NAME_LEN 100

typedef struct PcbNode PcbNode;

typedef enum ProcessStatus{
    OK,
    KILLED
} ProcessStatus;

typedef struct PcbNode{
    int pid;
    char name[MAX_NAME_LEN];
    int cpuBurst;
    int tempCpuBurst;
    int ioStart;
    int tempIoStart;
    int ioDuration;
    int tempIoDuration;
    int turnAroundTime;
    int killTime;
    ProcessStatus status;
} PcbNode;

PcbNode* createPcbNode(int, char*, int, int, int);
void printStatus(PcbNode*);
void printNode(PcbNode*);

#endif