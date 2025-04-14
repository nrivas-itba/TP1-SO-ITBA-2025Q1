#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "gameLogic.h"
#include "../../utils/utils.h"
#include "../../utils/ipc.h"

#define MAX_BOARD_VALUE 9
#define MIN_BOARD_VALUE 1

#define MAX_DIRECTION 7

#define MIN_PLAYERS 1

typedef struct {
    int delay;
    unsigned int seed;
    int timeout;
    char* view;
    char* playerPaths[MAX_PLAYERS];
    gameState_t* state;
    gameSync_t* sync;
    int stateFd;
    int syncFd;
} gameConfig_t;

void game(gameConfig_t* gameConfig, pipefd_t* pipefd);

#endif