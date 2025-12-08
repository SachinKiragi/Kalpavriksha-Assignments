#ifndef MODELS_H
#define MODELS_H

#include "../macros.h"
#include "../strategy/playerPerfStrategy.h"

typedef struct PlayerInfo {
    int id;
    char name[MAX_NAME_LENGTH];
    char team[MAX_NAME_LENGTH];
    char role[MAX_ROLE_LENGTH];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    playerPerformanceIndexStrategy performanceIndexStrategy; 
    struct PlayerInfo* next;
} PlayerInfo;

typedef struct{
    int teamId;
    char name[MAX_NAME_LENGTH];
    int totalPlayers;
    float averageBattingStrikerate;
    PlayerInfo* headPlayersByRole[3];
} TeamInfo;

#endif