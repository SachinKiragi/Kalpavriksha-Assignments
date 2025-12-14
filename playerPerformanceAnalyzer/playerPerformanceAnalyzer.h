#ifndef PLAYER_PERFORMANCE_ANALYZER_H
#define PLAYER_PERFORMANCE_ANALYZER_H

#include "lib/initTeams/initTeams.c"
#include "lib/models/models.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

void printPlayerHeader();
void printPlayerList(PlayerInfo*);
void printPlayersByRole(PlayerInfo* []);
void printTeamInfo(TeamInfo*);

bool idExistsHelper(int, PlayerInfo*);
bool idExistsInCurrTeam(int, PlayerInfo*[3]);
bool idExists(int, TeamInfo**, int);
StatusCode isValidPlayer(PlayerInfo*, TeamInfo**, int);
PlayerInfo* getPlayer();
void handleAddPlayer(TeamInfo**, int);

void handleDisplayPlayersByTeam(TeamInfo**);

void handleDisplayTeamsByAvgStrikeRate(TeamInfo**, int);

void displayTopKPlayers(PlayerInfo*, int);
void handleDisplayTopkPlayersByTeam(TeamInfo**, int);

void displayPlayersByRoleByPI(TeamInfo**, int, int);
void handleDisplayPlayersByRoleByPI(TeamInfo**, int);

void releasePlayersMemory(PlayerInfo**);
void releaseCurrentTeamMemory(TeamInfo*);
void releaseMemoryForTeams(TeamInfo**, int);

void printMenu();
void initPerformanceAnalyzer();

#endif