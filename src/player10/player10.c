// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../utils/utils.h"
#include "../utils/ipc.h"
#include "../utils/gameLogic.h"
unsigned int getMe(game_t* game) {
    pid_t pid = getpid();
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->state->playerList[i].pid == pid) {
            return i;
        }
    }
    errExit("I dont exist in the board");
}

char shouldITryToMove(player_t* me) {
    static unsigned int invalidMovementRequestsCount = -1;
    static unsigned int validMovementRequestsCount = -1;
    if (!me->isBlocked && (me->invalidMovementRequestsCount != invalidMovementRequestsCount || me->validMovementRequestsCount != validMovementRequestsCount)) {
        invalidMovementRequestsCount = me->invalidMovementRequestsCount;
        validMovementRequestsCount = me->validMovementRequestsCount;
        return 1;
    }
    return 0;
}

void waitToRead(gameSync_t* gameSync) {
    sWait(&(gameSync->masterWantsToReadMutex));
    sPost(&(gameSync->masterWantsToReadMutex)); //In the 31/3/2025 class we saw that it should have this order

    sWait(&(gameSync->readersCountMutex));
    gameSync->readersCount++;
    if (gameSync->readersCount == 1) {
        sWait(&(gameSync->readGameStateMutex));
    }
    sPost(&(gameSync->readersCountMutex));
}

void finishReading(gameSync_t* gameSync) {
    sWait(&(gameSync->readersCountMutex));
    gameSync->readersCount--;
    if (gameSync->readersCount == 0) {
        sPost(&(gameSync->readGameStateMutex));
    }
    sPost(&(gameSync->readersCountMutex));
}

char getMove(int me, gameState_t* state) {
    static int direction = 0;
    coords_t coords;
    if(!hasAnyValidDirection(state,me)){
        return 8;
    }
    if (!isDirectionValid(state,me,direction,&coords)){
        direction = (direction+1) % 8;
    }
    return direction;
    
}

void playingLoop(game_t* game, int gameWidth, int gameHeight, unsigned int playerCount, unsigned int me) {
    player_t playerList[playerCount];
    char isGameOver = 0;
    char state[sizeof(gameState_t)+ sizeof(int)*gameWidth*gameHeight];

    while (!isGameOver) {
        memcpy(playerList, game->state->playerList, sizeof(playerList[0]) * playerCount);
        memcpy(state, game->state, sizeof(gameState_t)+ sizeof(int)*gameWidth*gameHeight);
        isGameOver = game->state->isOver;
        finishReading(game->sync);

        if (shouldITryToMove(&(playerList[me]))) {
            char nextMove = getMove(me, (void*)state) % 9;
            if (nextMove != 8) {
                printf("%c", nextMove);
                fflush(stdout);
            }
            else {
                shouldITryToMove(&((player_t) {
                    .validMovementRequestsCount = -1
                }));
            }
        }

        if (!isGameOver) {
            waitToRead(game->sync);
        }
        else {
            break;
        }
    }
}


int main(int argc, char* argv[]) {
    game_t game = openGame(argc, argv);
    unsigned int me = getMe(&game);

    waitToRead(game.sync);
    playingLoop(&game, game.gameWidth, game.gameHeight, game.state->playerCount, me);

    closeGame(&game);
    return 0;
}
