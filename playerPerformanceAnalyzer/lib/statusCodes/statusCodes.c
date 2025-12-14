#include "statusCodes.h"


void printStatusCode(StatusCode status){
    switch(status){
        case SUCCESS:
            printf("Operation Executed Successfully\n");
            break;
        case ERROR:
            printf("Error: Unexpected Error Occurred\n");
            break;
        case INVALID_PLAYER_ID:
            printf("Error: Invalid Player ID\n");
            break;
        case INVALID_TEAM_ID:
            printf("Error: Invalid team ID\n");
            break;
        case INVALID_NAME:
            printf("Error: Invalid Name\n");
            break;
        case INVALID_ROLE:
            printf("Error: Invalid Role\n");
            break;
        case ID_EXISTS:
            printf("Error: Player ID Already Exists\n");
            break;
        case TEAM_FULL:
            printf("Error: Team is Full. Cannot Add More Players\n");
            break;
        case INIT:
            printf("Initialization Complete\n");
            break;
        default:
            printf("Unknown Status Code\n");
            break;
    }
}
