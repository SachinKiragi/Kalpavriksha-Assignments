#include "initTeams.h"
#include<stdlib.h>

StatusCode initmyTeams(TeamInfo** myTeams, int teamCount){
    for(int i = 0; i < teamCount; i++){
        myTeams[i] = (TeamInfo*) calloc (1, sizeof(TeamInfo));
        if(myTeams[i] == NULL){
            return ERROR;
        }
        strcpy(myTeams[i]->name, teams[i]);
        myTeams[i]->teamId = i+1;
    }  
     return SUCCESS;
}

float calculatePerformaceIndexForBatsman(PlayerInfo* player){
    float batAvg = player->battingAverage;
    float strikeRate = player->strikeRate;
    float perfIndex = (float)(batAvg * strikeRate) / 100.0;
    return perfIndex;
}

float calculatePerformaceIndexForBowler(PlayerInfo* player){
    int wickets = player->wickets;
    float ecoRate = player->economyRate;
    float perfIndex = (wickets * 2) + (100 - ecoRate);
    return perfIndex;
}

float calculatePerformaceIndexForAllRounder(PlayerInfo* player){
    float batAvg = player->battingAverage;
    float strikeRate = player->strikeRate;
    int wickets = player->wickets;
    float perfIndex = ((batAvg * strikeRate) / 100.0) + (wickets * 2);
    return perfIndex;
}

float calculatePerformaceIndex(PlayerInfo* player){
    float perfIndex = 0;
    if(strcmp(player->role, "Batsman") == 0){
        perfIndex = calculatePerformaceIndexForBatsman(player);
    } else if(strcmp(player->role, "Bowler") == 0){
        perfIndex = calculatePerformaceIndexForBowler(player);
    } else{
        perfIndex = calculatePerformaceIndexForAllRounder(player);
    }
}

PlayerInfo* createPlayer(Player player){
    PlayerInfo* currPlayer = (PlayerInfo*)malloc(sizeof(PlayerInfo));
    if(currPlayer == NULL){
        return NULL;
    }
    currPlayer->id = player.id;
    strcpy(currPlayer->name, player.name);
    strcpy(currPlayer->team, player.team);
    strcpy(currPlayer->role, player.role);
    currPlayer->totalRuns = player.totalRuns;
    currPlayer->battingAverage = player.battingAverage;
    currPlayer->strikeRate = player.strikeRate;
    currPlayer->wickets = player.wickets;
    currPlayer->economyRate = player.economyRate;
    currPlayer->performanceIndex = calculatePerformaceIndex(currPlayer);
    return currPlayer;
}

int getRoleId(char *role){
    if(strcmp(role, "Batsman") == 0){
        return 0;
    } else if(strcmp(role, "Bowler") == 0){
        return 1;
    } else{
        return 2;
    } 
}

PlayerInfo* insertPlayer(PlayerInfo* head, PlayerInfo* player){
    if(head == NULL){
        head = player;
        return head;
    }
    PlayerInfo* prev = NULL;
    PlayerInfo* curr = head;

    while(curr && curr->performanceIndex > player->performanceIndex){
        prev = curr;
        curr = curr->next;
    }

    if(curr == head){
        player->next = head;
        head = player;
    } else {
        prev->next = player;
        player->next = curr;
    }
    return head;
}

int getTeamIndex(char* teamName, TeamInfo** myTeams, int teamCount){
    for(int i = 0; i < teamCount; i++){
        if(strcmp(teamName, myTeams[i]->name) == 0){
            return i;
        }
    }
    return -1;
}

bool addPlayer(PlayerInfo* player, TeamInfo** myTeams, int teamCount){
    int teamIndex = getTeamIndex(player->team, myTeams, teamCount);
    int tempRoleId = getRoleId(player->role);
    myTeams[teamIndex]->headPlayersByRole[tempRoleId] = insertPlayer(myTeams[teamIndex]->headPlayersByRole[tempRoleId], player);
    myTeams[teamIndex]->totalPlayers++;
    return true;
}

StatusCode initCurrentPlayer(Player player, TeamInfo** myTeams, int teamCount){
    PlayerInfo* currPlayer = createPlayer(player);
    if(currPlayer == NULL){
        return ERROR;
    }
    if(addPlayer(currPlayer, myTeams, teamCount)){
        return SUCCESS;
    } else{
        return ERROR;
    }
}

StatusCode initPlayers(TeamInfo** myTeams, int teamCount){
    for(int i = 0; i < playerCount; i++){
        if(initCurrentPlayer(players[i], myTeams, teamCount) == ERROR){
            return ERROR;
        }
    }
    return SUCCESS;
}

float getAverageStrikeRate(PlayerInfo* head){
    if(head == NULL){
        return 0;
    }
    int noOfPlayers = 0;
    PlayerInfo* currPlayer = head;
    float cummulativeStrikeRate = 0;
    while(currPlayer){
        noOfPlayers++;
        cummulativeStrikeRate += currPlayer->strikeRate;
        currPlayer = currPlayer->next;
    }
    float avgStrikeRate = cummulativeStrikeRate / noOfPlayers;
    return avgStrikeRate;
}

float getAvgBatStrikeRate(TeamInfo* currTeam){
    PlayerInfo* batsmen = currTeam->headPlayersByRole[getRoleId("Batsman")];
    PlayerInfo* allRounder = currTeam->headPlayersByRole[getRoleId("All-Rounder")];
    float totalStrike = 0;
    int totalPlayers = 0;
    PlayerInfo* currPlayer = batsmen;
    while(currPlayer){
        totalStrike += currPlayer->strikeRate;
        totalPlayers++;
        currPlayer = currPlayer->next;
    }
    currPlayer = allRounder;
    while(currPlayer){
        totalStrike += currPlayer->strikeRate;
        totalPlayers++;
        currPlayer = currPlayer->next;
    }
    if(totalPlayers == 0){
        return 0;
    }
    return totalStrike / totalPlayers;
}

void initAvgBatStrikeRateByTeams(TeamInfo** myteams, int teamCount){
    for(int i = 0; i < teamCount; i++){
        myteams[i]->averageBattingStrikerate  = getAvgBatStrikeRate(myteams[i]);
    }
}