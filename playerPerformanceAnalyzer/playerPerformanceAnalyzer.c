#include "playerPerformanceAnalyzer.h"

void printPlayerInfo(PlayerInfo* currPlayer){
    printf("| %-4d | %-24s | %-15s | %-12s | %-6d | %-8.2f | %-8.2f | %-6d | %-8.2f | %-8.2f |\n",
        currPlayer->id, 
        currPlayer->name, 
        currPlayer->team, 
        currPlayer->role, 
        currPlayer->totalRuns, 
        currPlayer->battingAverage, 
        currPlayer->strikeRate, 
        currPlayer->wickets, 
        currPlayer->economyRate,
        currPlayer->performanceIndex
    );
}

void printPlayerHeader() {
    printf("----------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-4s | %-24s | %-15s | %-12s | %-6s | %-8s | %-8s | %-6s | %-8s | %-8s |\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "Econ", "PerfIdx");
    printf("----------------------------------------------------------------------------------------------------------------------------------\n");
}

void printPlayerList(PlayerInfo* head){
    PlayerInfo* curr = head;
    while(curr){
        printPlayerInfo(curr);
        curr = curr->next;
    }
}

void printPlayersByRole(PlayerInfo* headPlayersByRole[3]){
    printPlayerHeader();
    printPlayerList(headPlayersByRole[0]);
    printPlayerList(headPlayersByRole[1]);
    printPlayerList(headPlayersByRole[2]);
}

void printTeamInfo(TeamInfo* team){
    printPlayersByRole(team->headPlayersByRole);
    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", team->averageBattingStrikerate);
}

bool idExistsHelper(int id, PlayerInfo* headPlayer){
    PlayerInfo* currPlayer = headPlayer;
    while(currPlayer){
        if(id == currPlayer->id){
            return true;
        }
        currPlayer = currPlayer->next;
    }
    return false;
}

bool idExistsInCurrTeam(int id, PlayerInfo* headPlayersByRole[3]){
    if(idExistsHelper(id, headPlayersByRole[0])){
        return true;
    } else if(idExistsHelper(id, headPlayersByRole[1])){
        return true;
    } else if(idExistsHelper(id, headPlayersByRole[2])){
        return true;
    } else{
        return false;
    }
}

bool idExists(int id, TeamInfo** myTeams, int teamCount){
    for(int i = 0; i < teamCount; i++){
        if(idExistsInCurrTeam(id, myTeams[i]->headPlayersByRole)){
            return true;
        }
    }
    return false;
}

StatusCode isValidPlayer(PlayerInfo* player, TeamInfo** myTeams, int teamCount){
    StatusCode status = INIT;
    if(player == NULL){
        status = ERROR;
    }else if(player->id < MIN_PLAYER_ID || player->id > MAX_PLAYER_ID){
        status = INVALID_PLAYER_ID;
    } else if(strlen(player->name) == 0){
        status = INVALID_NAME;
    } else if(player->role == "\0"){
        status = INVALID_ROLE;
    } else if(idExists(player->id, myTeams, teamCount)){
        status = ID_EXISTS;
    } else{
        status = SUCCESS;
    }
    return status;
}

PlayerInfo* getPlayer(){
    PlayerInfo* player = (PlayerInfo*) calloc (1, sizeof(PlayerInfo));
    if(player == NULL){
        return NULL;
    }
    printf("Player ID: ");
    scanf("%d", &player->id);
    printf("Name: ");
    scanf(" %[^\n]", player->name);
    int roleId;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleId);
    if(roleId == 1){
        strcpy(player->role, "Batsman");
    } else if(roleId == 2){
        strcpy(player->role, "Bowler");
    } else if(roleId == 3){
        strcpy(player->role, "All-rounder");
    }
    printf("Total Runs: ");
    scanf("%d", &player->totalRuns);
    printf("Batting Average: ");
    scanf("%f", &player->battingAverage);
    printf("Strike Rate: ");
    scanf("%f", &player->strikeRate);
    printf("Wickets: ");
    scanf("%d", &player->wickets);
    printf("Economy Rate: ");
    scanf("%f", &player->economyRate);
    player->performanceIndex = calculatePerformaceIndex(player);   
    return player;
}

void handleAddPlayer(TeamInfo** myTeams, int teamCount){
    int teamId;
    printf("Enter team id [1-10]: ");
    scanf("%d", &teamId);
    if(teamId < 0 || teamId > 10){
        printStatusCode(INVALID_TEAM_ID);
        return;
    }
    teamId--;
    if(myTeams[teamId]->totalPlayers == MAX_NUM_OF_PLAYERS){
        printStatusCode(TEAM_FULL);
        return;
    }
    bool valid = false;
    PlayerInfo* player = NULL;
    while(valid == false){
        player = getPlayer();
        StatusCode status = isValidPlayer(player, myTeams, teamCount);
        if(status == SUCCESS){
            valid = true;
        } else{
            printStatusCode(status);
        }
    }
    strcpy(player->team, myTeams[teamId]->name);
    if(addPlayer(player, myTeams, teamCount) == false){
        printStatusCode(ERROR);
        return;
    }
    myTeams[teamId]->averageBattingStrikerate = getAvgBatStrikeRate(myTeams[teamId]);
}

void handleDisplayPlayersByTeam(TeamInfo** myTeams){
    int teamId;
    printf("Enter team id [1-10]: ");
    scanf("%d", &teamId);
    teamId--;
    if(teamId < 0 || teamId > 10){
        printStatusCode(INVALID_TEAM_ID);
        return;
    }
    printTeamInfo(myTeams[teamId]);
}

void handleDisplayTeamsByAvgStrikeRate(TeamInfo** myTeams, int teamCount){
    typedef struct{
        int teamId;
        char name[MAX_NAME_LENGTH];
        int totalPlayers;
        float averageBattingStrikerate;
    } TempTeamInfo;
    TempTeamInfo tempTeams[teamCount];
    for(int i = 0; i < teamCount; i++){
        tempTeams[i].teamId = myTeams[i]->teamId;
        strcpy(tempTeams[i].name, myTeams[i]->name);
        tempTeams[i].totalPlayers = myTeams[i]->totalPlayers;
        tempTeams[i].averageBattingStrikerate = myTeams[i]->averageBattingStrikerate;
    }
    for(int i = 0; i < teamCount; i++){
        for(int j = 0; j < teamCount - i - 1; j++){
            if(tempTeams[j].averageBattingStrikerate < tempTeams[j + 1].averageBattingStrikerate){
                TempTeamInfo temp = tempTeams[j];
                tempTeams[j] = tempTeams[j + 1];
                tempTeams[j + 1] = temp;
            }
        }
    }
    printf("===========================================================\n");
    printf("| %-4s | %-15s | %-12s | %-14s |\n",
           "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("===========================================================\n");
    for(int i = 0; i < teamCount; i++){
        printf("| %-4d | %-15s | %-12.1f | %-14d |\n", tempTeams[i].teamId, tempTeams[i].name, tempTeams[i].averageBattingStrikerate, tempTeams[i].totalPlayers);        
    }
}

void displayTopKPlayers(PlayerInfo* head, int k){
    PlayerInfo* currPlayer = head;
    printPlayerHeader();
    while(k > 0 && currPlayer){
        printPlayerInfo(currPlayer);
        currPlayer = currPlayer->next;
        k--;
    }
}

void handleDisplayTopkPlayersByTeam(TeamInfo** myTeams, int teamCount){
    int teamId;
    int role;
    int k;
    printf("Enter team id [1-10]: ");
    scanf("%d", &teamId);
    printf("EnterRole(1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &role);
    printf("Enter number of players: ");
    scanf("%d", &k);
    teamId--;
    role--;
    if(teamId < 0 || teamId > 100){
        printStatusCode(INVALID_TEAM_ID);
        return;
    }
    if(role < 0 || role > 2){
        printStatusCode(INVALID_ROLE);
        return;
    }
    displayTopKPlayers(myTeams[teamId]->headPlayersByRole[role], k);
}

void displayPlayersByRoleByPI(TeamInfo** myTeams, int teamCount, int role){
    printPlayerHeader();
    PlayerInfo** heap = initHeap(myTeams, teamCount, role);
    int heapSize = teamCount;

    while(heapSize){
        PlayerInfo* currTopPlayer = pop(heap, &heapSize);
        printPlayerInfo(currTopPlayer);
        push(heap, &heapSize, currTopPlayer->next);
    }
}

void handleDisplayPlayersByRoleByPI(TeamInfo** myTeams, int teamCount){
    int role;
    printf("EnterRole(1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &role);
    role--;
    if(role < 0 || role > 2){
        printStatusCode(INVALID_ROLE);
        return;
    }
    displayPlayersByRoleByPI(myTeams, teamCount, role);
}

void releasePlayersMemory(PlayerInfo** headPlyer){
    PlayerInfo** currPlayer = headPlyer;
    PlayerInfo** temp = NULL;
    while(*currPlayer){
        temp = currPlayer;
        *currPlayer = (*currPlayer)->next;
        free(*temp);
        *temp = NULL;
    }
}

void releaseCurrentTeamMemory(TeamInfo* team){
    releasePlayersMemory(&team->headPlayersByRole[0]);
    releasePlayersMemory(&team->headPlayersByRole[1]);
    releasePlayersMemory(&team->headPlayersByRole[2]);
}

void releaseMemoryForTeams(TeamInfo** myTeams, int teamCount){
    for(int i = 0; i < teamCount; i++){
        releaseCurrentTeamMemory(myTeams[i]);
        free(myTeams[i]);
        myTeams[i] = NULL;
    }
}

void printMenu(){
    printf("==============================================================================\n");
    printf("%-79s\n", " ICC ODI Player Performance Analyzer");
    printf("==============================================================================\n");

    printf("%-5s %-60s\n", "1.", "Add Player to Team");
    printf("%-5s %-60s\n", "2.", "Display Players of a Specific Team");
    printf("%-5s %-60s\n", "3.", "Display Teams by Average Batting Strike Rate");
    printf("%-5s %-60s\n", "4.", "Display Top K Players of a Specific Team by Role");
    printf("%-5s %-60s\n", "5.", "Display all Players of Specific Role Across All Teams by Performance Index");
    printf("%-5s %-60s\n", "6.", "Exit");

    printf("==============================================================================\n");
    printf("Enter your choice: ");
}

void initPerformanceAnalyzer(){
    TeamInfo** myTeams = (TeamInfo**) malloc (teamCount * sizeof(TeamInfo*));
    if(myTeams == NULL){
        printStatusCode(ERROR);
        return;
    }
    StatusCode status = INIT;
    status = initmyTeams(myTeams, teamCount);
    if(status == ERROR){
        printStatusCode(status);
        return;
    }
    status = initPlayers(myTeams, teamCount);
    if(status == ERROR){
        printStatusCode(status);
        return;
    }
    initAvgBatStrikeRateByTeams(myTeams, teamCount);

    int ch;
    while(1){
        printMenu();
        scanf("%d", &ch);
        getchar();
        switch(ch){
            case 1: handleAddPlayer(myTeams, teamCount);
            break;
            case 2: handleDisplayPlayersByTeam(myTeams);
            break;
            case 3: handleDisplayTeamsByAvgStrikeRate(myTeams, teamCount);
            break;
            case 4: handleDisplayTopkPlayersByTeam(myTeams, teamCount);
            break; 
            case 5: handleDisplayPlayersByRoleByPI(myTeams, teamCount);
            break;
            case 6: releaseMemoryForTeams(myTeams, teamCount);
                    printf("Memory released! Exiting..\n");
                    return;
            default:
            printf("Invalid choice! Try again!\n");
        }
    }
    return;
}