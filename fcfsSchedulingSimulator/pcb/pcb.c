#include "pcb.h"

PcbNode* createPcbNode(int pid, char* name, int cpuBurst, int ioStart, int ioDuration){
    PcbNode* newPcbNode = (PcbNode*) calloc (1, sizeof(PcbNode));
    if(newPcbNode == NULL){
        return newPcbNode;
    }
    newPcbNode->pid = pid;
    strcpy(newPcbNode->name, name);
    newPcbNode->cpuBurst = cpuBurst;
    newPcbNode->tempCpuBurst = cpuBurst;
    newPcbNode->ioStart = ioStart;
    newPcbNode->tempIoStart = ioStart;
    newPcbNode->ioDuration = ioDuration;
    newPcbNode->tempIoDuration = ioDuration;
    newPcbNode->killTime = 1e9;
    return newPcbNode;
}

void printStatus(PcbNode* curr){
    switch (curr->status){
        case OK:
            printf("%-14s", "OK");
            break;
        case KILLED:
            printf("KILLED at %-4d", curr->killTime);
            break;
        default:
            printf("Error: Unexpected Error Occured\n");
    }
}

void printNode(PcbNode* curr){
    int waitingTime = curr->turnAroundTime - curr->cpuBurst;
    if(waitingTime < 0) waitingTime = 0;

    printf("| %-3d | %-7s | %-3d | %-3d | ",
           curr->pid,
           curr->name,
           curr->cpuBurst,
           curr->ioDuration);

    printStatus(curr);

    printf(" | %-10d | %-7d |\n",
           curr->turnAroundTime,
           waitingTime);
}