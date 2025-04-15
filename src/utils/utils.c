// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>

#include "utils.h"
#include "ipc.h"


game_t openGame(int argc, char* argv[]) {
    if (argc != 3) {
        errExit("invalid argc");
    }

    game_t ret = (game_t){
        .gameWidth = atoi(argv[1]),
        .gameHeight = atoi(argv[2]),
        .sync = openMem(GAME_SYNC, sizeof(*ret.sync), 0, &(ret.syncFd)),
        .state = openMem(GAME_STATE,sizeof(*ret.state) + (ret.gameWidth * ret.gameHeight) * sizeof((ret.state->board)[0]), 1, &(ret.stateFd))
    };

    if (ret.gameWidth <= 0 || ret.gameHeight <= 0) {
        errExit("Board dimensions cant be less or equal to 0");
    }

    if (ret.gameHeight != ret.state->height || ret.gameWidth != ret.state->width) {
        errExit("Board dimensions dont match");
    }

    return ret;
}

void closeGame(game_t* game) {
    closeMem(0, sizeof(*game->sync), game->sync, &(game->syncFd));
    closeMem(0, sizeof(*game->state) + (game->gameWidth * game->gameHeight) * sizeof((game->state->board)[0]), game->state, &(game->stateFd));
}

unsigned int decimalLen(int number) {
    unsigned int len = 0;
    if (number < 0) {
        number *= -1;
        len++;
    }
    do {
        len++;
    } while (number /= 10);
    return len;
}