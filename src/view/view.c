// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include "../utils/utils.h"
#include "graphics/graphics.h"
#define STRINGIZE_WRAPPED(x) #x
#define STRINGIZE(x) STRINGIZE_WRAPPED(x)
#define MIN_MAX_FORMAT(x) STRINGIZE(x) "." STRINGIZE(x)
#define NOT_ENOUGH_SPACE_TO_PRINT_MATRIX "Not enough space for matrix"
#define NOT_ENOUGH_SPACE_TO_PRINT_TABLE "Not enough space for table"
#define NAME "Name"
#define SCORE "Score"
#define SCORE_LEN 5
#define VALID_REQUESTS "ValRq"
#define VALID_REQUESTS_LEN 5
#define INVALID_REQUESTS "InvRq"
#define INVALID_REQUESTS_LEN 5
#define IS_BLOCKED "Health"
#define IS_BLOCKED_LEN 6
#define BLOCKED_PLAYER "💀"
#define NON_BLOCKED_PLAYER "😀"
#define TABLE_FORMAT(TYPE_ARG) "%-"MIN_MAX_FORMAT(MAX_PLAYER_NAME_LEN)"s %-"STRINGIZE(SCORE_LEN) TYPE_ARG" %-"STRINGIZE(VALID_REQUESTS_LEN) TYPE_ARG" %-"STRINGIZE(INVALID_REQUESTS_LEN) TYPE_ARG " %-"MIN_MAX_FORMAT(IS_BLOCKED_LEN)"s\n"
#define TABLE_FORMAT_HEADER TABLE_FORMAT("s")
#define TABLE_FORMAT_ROW TABLE_FORMAT("d")
#define NUMBER_OF_NON_FORMAT_CHARACTERS_IN_TABLE_FORMAT 4
#define TABLE_FORMAT_LEN NUMBER_OF_NON_FORMAT_CHARACTERS_IN_TABLE_FORMAT + MAX_PLAYER_NAME_LEN + SCORE_LEN + VALID_REQUESTS_LEN + INVALID_REQUESTS_LEN + IS_BLOCKED_LEN
#define NUMBER_FITS(NUMBER, MAX) decimalLen(NUMBER) < MAX ? NUMBER : -1


/*
    Prints the player stats table
    Returns how many lines where printed.
*/
int printPlayerStats(player_t* players, unsigned int playerCount, const screen_t screen){

    // TODO aux arr with indexes -> sort with criteria (complex)

    int xWidth = screen.xWidth-2;
    int yHeight = screen.yWidth-2;

    const int tableHeight = 4+playerCount;

    //fix magic numbers
    // if( !checkPrintable(xWidth,yHeight,myWidth,playerCount,screen) ){
    //     return checkWritable(screen, NOT_ENOUGH_SPACE_TO_PRINT_TABLE);
    // }

    //TODO codigo repetido
    if(xWidth < TABLE_FORMAT_LEN || yHeight <  tableHeight){
        int ret = 0;
        if(screen.yWidth >= 1 && screen.xWidth >= sizeof(NOT_ENOUGH_SPACE_TO_PRINT_MATRIX)){
            moveCursorScreen(screen,0,0);
            puts(NOT_ENOUGH_SPACE_TO_PRINT_MATRIX);
            ret = 1;
        }
        fflush(stderr);
        return ret;
    }

    puts(TABLE_FORMAT(""));

    // Header
    moveCursorScreen(screen,0,0);
    for (int x = 0; x < TABLE_FORMAT_LEN; x++){
        printf("═");
    }
    moveCursorScreen(screen,0,1);
    printf(TABLE_FORMAT_HEADER, NAME, SCORE, VALID_REQUESTS, INVALID_REQUESTS, IS_BLOCKED);
    moveCursorScreen(screen,0,2);
    for (int x = 0; x < TABLE_FORMAT_LEN; x++){
        printf("═");
    }

    for (int fila = 0; fila < playerCount; fila++){
        moveCursorScreen(screen,0,3+fila);
        printf(TABLE_FORMAT_ROW, players[fila].name, NUMBER_FITS(players[fila].score, SCORE_LEN), NUMBER_FITS(players[fila].validMovementRequestsCount, VALID_REQUESTS_LEN), NUMBER_FITS(players[fila].invalidMovementRequestsCount, INVALID_REQUESTS_LEN), (players[fila].isBlocked ? BLOCKED_PLAYER : NON_BLOCKED_PLAYER)); 
    }

    // Lowe border
    moveCursorScreen(screen,0,3+playerCount);
    for (int x = 0; x < TABLE_FORMAT_LEN; x++){
        printf("═");
    }

    return tableHeight;
}

/*
    Prints the board
    Returns how many lines where printed.
*/
int printGame(int gameWidth, int gameHeight, int board[gameHeight][gameWidth], const screen_t screen){
    static const char* playerColors[]= {
        RED,
        GREEN,
        BLUE,
        CYAN,
        MAGENTA,
        YELLOW,
        WHITE,
        GRAY
        BLACK,
    };
    
    int xWidth = screen.xWidth-2;
    int yHeight = screen.yWidth-2;
    if(xWidth < gameWidth || yHeight < gameHeight){
        int ret = 0;
        if(screen.yWidth >= 1 && screen.xWidth >= sizeof(NOT_ENOUGH_SPACE_TO_PRINT_MATRIX)){
            moveCursorScreen(screen,0,0);
            puts(NOT_ENOUGH_SPACE_TO_PRINT_MATRIX);
            ret = 1;
        }
        fflush(stderr);
        return ret;
    }
    int xMult = xWidth/gameWidth;
    int yMult = yHeight/gameHeight;

    // Upper boarder
    printBoard(screen, gameWidth*xMult+2, gameHeight*yMult+2);

   screen_t screenInsider = modifyScreen(screen,1,1); //TODO nombre de variable no representativo
   char numberStr[2] = {0};
   for (int fila = 0; fila < gameHeight; fila++){
        for (int columna = 0; columna < gameWidth; columna++){
            int boardValue = board[fila][columna];
            if (boardValue > 0){
                numberStr[0] = '0'+(boardValue%10);
                printBlock(screenInsider, columna, fila, yMult, xMult, " ", numberStr);
            }
            else {
                printf("%s", playerColors[-boardValue]);
                printBlock(screenInsider, columna, fila, yMult, xMult, "█", "█");
                printf("%s", WHITE);
            }
        }
    }

    fflush(stdout);
    return 2+gameHeight*yMult;
}

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv); //TODO meter todas estas inicializaciones en un inicializador de view
    printf(ENABLE_ALTERNATIVE_SCREEN_BUFFER);
    signalHandler_t signalHandler = setGraphicsSignalHandler();
    char isGameOver = 0;
    while(!isGameOver){
        sWait(&(game.sync->printNeeded)); //Waint until master wants to print
        screen_t screen = buildScreen(1,14);
        moveCursorScreen(screen,0,0);
        screen = modifyScreen(screen, 0, printPlayerStats(game.state->playerList, game.state->playerCount, screen));
        screen = modifyScreen(screen, 0, printGame(game.gameWidth, game.gameHeight, (void*)(game.state->board), screen));
        moveCursorScreen(screen,0,0);
        isGameOver = game.state->isOver;
        sPost(&(game.sync->printDone)); //Tell the master that we have finished printing.

    }
    deleteGraphicsSignalHandler(signalHandler);
    printf(DISABLE_ALTERNATIVE_SCREEN_BUFFER);
    return 0;
}