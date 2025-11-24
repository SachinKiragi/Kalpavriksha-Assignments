#ifndef STATUSCODES_H
#define STATUSCODES_H

#include<stdio.h>

typedef enum{
    INIT,
    INVALID_TEAM_ID,
    INVALID_PLAYER_ID,
    INVALID_NAME,
    INVALID_ROLE,
    ID_EXISTS,
    TEAM_FULL,
    SUCCESS,
    ERROR
} StatusCode;

void printStatusCode(StatusCode);

#endif