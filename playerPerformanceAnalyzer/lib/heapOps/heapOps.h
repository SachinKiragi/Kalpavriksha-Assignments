#ifndef HEAPOPS_H
#define HEAPOPS_H

#include<stdlib.h>
#include "../macros.h"
#include "../models/models.h"

void heapify(PlayerInfo**, int, int);
PlayerInfo* pop(PlayerInfo**, int*);
void push(PlayerInfo**, int*, PlayerInfo*);
PlayerInfo** initHeap(TeamInfo**, int, int);


#endif