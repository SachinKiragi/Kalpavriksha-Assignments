#ifndef TEAMINIT_H
#define TEAMINIT_H

#include "../data/Players_data.h"
#include "../heapOps/heapOps.c"
#include "../statusCodes/statusCodes.c"
#include "../strategy/playerCategories.c"

#include<stdbool.h>
#include<string.h>

StatusCode initmyTeams(TeamInfo**, int);
float calculatePerformaceIndex(PlayerInfo*);
PlayerInfo* createPlayer(Player);
int getRoleId(char*);
PlayerInfo* insertPlayer(PlayerInfo*, PlayerInfo*);
int getTeamIndex(char*, TeamInfo**, int);
bool addPlayer(PlayerInfo*, TeamInfo**, int);
StatusCode initCurrentPlayer(Player, TeamInfo**, int);
StatusCode initPlayers(TeamInfo**, int);
float getAverageStrikeRate(PlayerInfo*);
float getAvgBatStrikeRate(TeamInfo*);
void initAvgBatStrikeRateByTeams(TeamInfo**, int);

#endif