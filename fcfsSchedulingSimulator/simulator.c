#include "simulator.h"

void clock(){
    Sleep(10);
    timer++;
    printf("Timer: %d Sec.\n", timer);
}

void updateTurnArroundTime(PcbHashMap* pcbMap, ProcessQueues* pq){
    Queue* readyQue = pq->readyQue;
    Queue* terminatedQue = pq->terminatedQue;
    int size = readyQue->size;
    for(int i = 0; i < size; i++){
        int currPid = readyQue->getFrontFromQueue(readyQue);
        PcbNode* currPcbNode = pcbMap->getPcbFromMap(currPid, pcbMap);
        readyQue->removeFromQueue(readyQue);
        if(currPcbNode->killTime == timer){
            currPcbNode->tempCpuBurst = 0;
            currPcbNode->status = KILLED;
            terminatedQue->insertInQueue(currPid, terminatedQue);
        } else{
            currPcbNode->turnAroundTime++;
            readyQue->insertInQueue(currPid, readyQue);
        }
    }
}

void handleReadyQueue(PcbHashMap* pcbMap, ProcessQueues* pq){
    Queue* readyQue = pq->readyQue;
    Queue* waitingQue = pq->waitingQue;
    Queue* terminatedQue = pq->terminatedQue;
    if(readyQue->size <= 0){
        return;
    }
    int currProcessId = readyQue->getFrontFromQueue(readyQue);

    PcbNode* currPcbNode = pcbMap->getPcbFromMap(currProcessId, pcbMap);
    currPcbNode->tempCpuBurst--;
    
    updateTurnArroundTime(pcbMap, pq);
    
    if(currPcbNode->tempCpuBurst == 0 && currProcessId == readyQue->getFrontFromQueue(readyQue)){
        currPcbNode->status = OK;
        terminatedQue->insertInQueue(currProcessId, terminatedQue);
        readyQue->removeFromQueue(readyQue);
    } else if(currPcbNode->tempIoStart > 0){
        currPcbNode->tempIoStart--;
        if(currPcbNode->tempIoStart == 0){
            currPcbNode->turnAroundTime--;
            currPcbNode->tempIoDuration++;
            waitingQue->insertInQueue(currProcessId, waitingQue);
            readyQue->removeFromQueue(readyQue);
        }
    }
}

void handleWaitingQueue(PcbHashMap* pcbMap, ProcessQueues* pq){
    Queue* readyQue = pq->readyQue;
    Queue* waitingQue = pq->waitingQue;
    Queue* terminatedQue = pq->terminatedQue;
    int currPid = 0;
    PcbNode* currPcbNode = NULL;
    for(int i = 0; i < waitingQue->size; i++){
        currPid = waitingQue->getFrontFromQueue(waitingQue);
        if(currPid < 0) continue;
        waitingQue->removeFromQueue(waitingQue);
        currPcbNode = pcbMap->getPcbFromMap(currPid, pcbMap);
        currPcbNode->tempIoDuration--;
        if(currPcbNode->killTime == timer){
            continue;
        }
        currPcbNode->turnAroundTime++;
        if(currPcbNode->tempIoDuration == 0){
            readyQue->insertInQueue(currPid, readyQue);
        } else{
            waitingQue->insertInQueue(currPid, waitingQue);
        }
    }
}

void startSimulation(PcbHashMap* pcbMap, ProcessQueues* pq){
    Queue* readyQue = pq->readyQue;
    Queue* waitingQue = pq->waitingQue;
    Queue* terminatedQue = pq->terminatedQue;
    while(readyQue->head || waitingQue->head){
        clock();
        handleReadyQueue(pcbMap, pq);
        handleWaitingQueue(pcbMap, pq);
    }
}

ProcessQueues* initProcessQueues(){
    ProcessQueues* pq = (ProcessQueues*) calloc (1, sizeof(ProcessQueues));
    pq->readyQue = initQueue();
    pq->waitingQue = initQueue();
    pq->terminatedQue = initQueue();
    return pq;
}

void freeProcessQueuesMemory(ProcessQueues* pq){
    pq->readyQue->freeQueueMemory(pq->readyQue);
    pq->waitingQue->freeQueueMemory(pq->waitingQue);
    pq->terminatedQue->freeQueueMemory(pq->terminatedQue);
    free(pq);
    pq = NULL;
}

int initFcfsScheduler(){
    PcbHashMap* pcbMap = initMap();
    ProcessQueues* pq = initProcessQueues();
    
    if(pcbMap == NULL || pq == NULL || pq->readyQue == NULL || pq->waitingQue == NULL || pq->terminatedQue == NULL){
        printf("Error: Memory Allocation Failed\n");
        return 0;
    }

    char line[MAX_INPUT_LEN];
    char arg1[MAX_NAME_LEN];
    int arg2, arg3, arg4, arg5;
    int argsLen = 0;

    while(1){
        fgets(line, MAX_INPUT_LEN, stdin);
        argsLen = sscanf(line, "%s %d %d %d %d %d", arg1, &arg2, &arg3, &arg4, &arg5);
        if(argsLen == -1){
            break;
        }
        if(argsLen == 5){
            PcbNode* currPcb = createPcbNode(arg2, arg1, arg3, arg4, arg5);
            pcbMap->insertPcbNodeInMap(currPcb, pcbMap);
            pq->readyQue->insertInQueue(arg2, pq->readyQue);
        } else if(argsLen == 3){
            if(strcmp(arg1, "KILL") != 0){
                printf("Invalid Command\n");
                continue;
            }
            PcbNode* tempPcbNode =  pcbMap->getPcbFromMap(arg2, pcbMap);
            if(tempPcbNode == NULL){
                printf("Invalid Process Id\n");
            } else{
                tempPcbNode->killTime = arg3;
            }
        } else{
            printf("Invalid Input!\n");
        }
    }

    printf("Simulation Begins\n");
    startSimulation(pcbMap, pq);
    printf("Simulation Ends\n");

    pcbMap->printMap(pcbMap);

    freeProcessQueuesMemory(pq);
    pcbMap->freeMapMemory(pcbMap);
    printf("\nMemory Released Successfully\n");
    return 0;
}