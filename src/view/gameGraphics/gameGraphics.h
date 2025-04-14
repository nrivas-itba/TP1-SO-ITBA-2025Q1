#ifndef GAME_GRAPHICS_H
#define GAME_GRAPHICS_H
#include "../../utils/utils.h"
#include "../graphics/graphics.h"

int printGame(int gameWidth, int gameHeight, int board[gameWidth][gameHeight], player_t* playerList, const screen_t* screenOrg);
int printPlayerStats(player_t* playerList, unsigned int playerCount, const screen_t* screenOrg);

#endif