#ifndef GAME_GRAPHICS_H
#define GAME_GRAPHICS_H
#include "../../utils/utils.h"
#include "../graphics/graphics.h"

/// @brief Prints the game.
/// @param gameWidth 
/// @param gameHeight 
/// @param board 
/// @param playerList 
/// @param screenOrg 
/// @return How many lines were printed.
int printGame(int gameWidth, int gameHeight, int board[gameWidth][gameHeight], player_t* playerList, const screen_t* screen);

/// @brief Print the game board stats in the screen.
/// @param playerList 
/// @param playerCount 
/// @param screenOrg Pointer to the screen.
/// @return How many lines were printed.
int printPlayerStats(player_t* playerList, unsigned int playerCount, const screen_t* screen);

#endif