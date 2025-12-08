#include "playerCategories.h"

float batsmanPerformanceIndexStrategy(PlayerInfo* player){
    float batAvg = player->battingAverage;
    float strikeRate = player->strikeRate;
    float perfIndex = (float)(batAvg * strikeRate) / 100.0;
    return perfIndex;
}

float bowlerPerformanceIndexStrategy(PlayerInfo* player){
    int wickets = player->wickets;
    float ecoRate = player->economyRate;
    float perfIndex = (wickets * 2) + (100 - ecoRate);
    return perfIndex;
}

float allRounderPerformanceIndexStrategy(PlayerInfo* player){
    float batAvg = player->battingAverage;
    float strikeRate = player->strikeRate;
    int wickets = player->wickets;
    float perfIndex = ((batAvg * strikeRate) / 100.0) + (wickets * 2);
    return perfIndex;
}