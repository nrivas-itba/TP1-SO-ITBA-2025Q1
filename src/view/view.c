// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "../utils/utils.h"
#include "../utils/ipc.h"
#include "../utils/ari.h"
#include "graphics/graphics.h"
#include "gameGraphics/gameGraphics.h"


void printingLoop(game_t* game, int gameWidth, int gameHeight, unsigned int playerCount) {
    player_t playerList[playerCount];
    int board[gameWidth][gameHeight];
    char isGameOver = 0;
    while (1) {
        memcpy(playerList, game->state->playerList, sizeof(playerList[0]) * playerCount);
        memcpy(board, game->state->board, sizeof(board[0][0]) * gameWidth * gameHeight);
        isGameOver = game->state->isOver;
        sPost(&(game->sync->printDone));
        screen_t screen = buildScreen(0, 0);
        screen = buildScreenFromScreen(&screen, 0, printPlayerStats(playerList, playerCount, &screen));
        screen = buildScreenFromScreen(&screen, 0, printGame(gameWidth, gameHeight, board, playerList, &screen));
        moveCursorScreen(&screen, 0, 0);
        fflush(stdout);

        if (!isGameOver) {
            sWait(&(game->sync->printNeeded));
        }
        else {
            sleep(ARI_SLEEP);
            break;
        }
    }
}


int main(int argc, char* argv[]) {
    game_t game = openGame(argc, argv);
    printf(ENABLE_ALTERNATIVE_SCREEN_BUFFER);
    signalHandler_t signalHandler = setGraphicsSignalHandler();

    sWait(&(game.sync->printNeeded));
    printingLoop(&game, game.gameWidth, game.gameHeight, game.state->playerCount);

    deleteGraphicsSignalHandler(signalHandler);
    printf(DISABLE_ALTERNATIVE_SCREEN_BUFFER);
    return 0;
}