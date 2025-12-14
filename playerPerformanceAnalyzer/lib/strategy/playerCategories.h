#ifndef PLAYER_CATEGORIES
#define PLAYER_CATEGORIES

typedef struct PlayerInfo PlayerInfo;

float batsmanPerformanceIndexStrategy(PlayerInfo*);
float bowlerPerformanceIndexStrategy(PlayerInfo*);
float allRounderPerformanceIndexStrategy(PlayerInfo*);

#endif