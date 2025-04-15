// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <string.h> //for mem cpy
#include <stdlib.h> //for qsort
#include "gameGraphics.h"

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
#define TABLE_FORMAT_ROW TABLE_FORMAT("u")
#define NUMBER_OF_NON_FORMAT_CHARACTERS_IN_TABLE_FORMAT 4
#define TABLE_FORMAT_LEN (NUMBER_OF_NON_FORMAT_CHARACTERS_IN_TABLE_FORMAT + MAX_PLAYER_NAME_LEN + SCORE_LEN + VALID_REQUESTS_LEN + INVALID_REQUESTS_LEN + IS_BLOCKED_LEN)
#define NUMBER_FITS(NUMBER, MAX) decimalLen(NUMBER) < (MAX) ? (NUMBER) : -1

//Chars stock █ ▓ ▒
#define HEAD_CHAR "█"
#define HEAD_SPECIAL_CHAR "█"
#define BODY_CHAR "▒"

static int comparePlayer(const player_t* player1, const player_t* player2){
    int ret = player2->score - player1->score;
    if(ret){
        return ret;
    }
    ret = player2->validMovementRequestsCount - player1->validMovementRequestsCount;
    if(ret){
        return ret;
    }
    return player2->invalidMovementRequestsCount - player1->invalidMovementRequestsCount;
}

void sortPlayers(unsigned int playerCount, player_t* playerList){
    for(unsigned int i = 0; i<playerCount; i++){
        playerList[i].pid = i;
    }
    qsort(playerList,playerCount,sizeof(playerList[0]),(int(*)(const void *, const void *))comparePlayer);
}

/*
    Prints the player stats table
    Returns how many lines where printed.
*/
int printPlayerStats(player_t* playerList, unsigned int playerCount, const screen_t* screenOrg){
    player_t sortedPlayerList[playerCount];
    memcpy(sortedPlayerList, playerList, sizeof(sortedPlayerList));
    sortPlayers(playerCount, sortedPlayerList);

    screen_t screen = *screenOrg;
    const int tableHeight = 3+playerCount;
    const int tableWidth = 2+TABLE_FORMAT_LEN;
    
    int ret = checkPrintable(&screen, tableWidth, tableHeight, NOT_ENOUGH_SPACE_TO_PRINT_TABLE, sizeof(NOT_ENOUGH_SPACE_TO_PRINT_TABLE)-1);
    if (ret!=1){
        return -ret;
    }


    moveCursorScreen(&screen,0,0);
    printf(TABLE_FORMAT_HEADER, NAME, SCORE, VALID_REQUESTS, INVALID_REQUESTS, IS_BLOCKED);

    for (int row = 0; row < playerCount; row++){
        moveCursorScreen(&screen, 0, 1+row);
        printf("%s",getPlayerColor(sortedPlayerList[row].pid));
        printf(TABLE_FORMAT_ROW, sortedPlayerList[row].name, NUMBER_FITS(sortedPlayerList[row].score, SCORE_LEN), NUMBER_FITS(sortedPlayerList[row].validMovementRequestsCount, VALID_REQUESTS_LEN), NUMBER_FITS(sortedPlayerList[row].invalidMovementRequestsCount, INVALID_REQUESTS_LEN), (sortedPlayerList[row].isBlocked ? BLOCKED_PLAYER : NON_BLOCKED_PLAYER)); 
        printf("%s",getPlayerColor(-1));
    }

    fflush(stdout);
    return tableHeight;
}

static inline char isThisAPlayerHead(player_t* playerList, int negativePlayerIndex, unsigned short x, unsigned short y){
    return playerList[-negativePlayerIndex].x == x && playerList[-negativePlayerIndex].y == y;
}

/*
    Prints the game
    Returns how many lines where printed.
*/
int printGame(int gameWidth, int gameHeight, int board[gameWidth][gameHeight], player_t* playerList, const screen_t* screenOrg){
    screen_t screen = *screenOrg;
    int xMult = (screen.xWidth-2)/gameWidth;
    int yMult = (screen.yHeight-2)/gameHeight;
    int xRealWidth = gameWidth*xMult + 2;
    int yRealHeight = gameHeight*yMult + 2;
    if(yMult<1 || xMult <1){
        xRealWidth = screen.xWidth+1; //This forces checkPrintableToExit
    }

    int ret = checkPrintable(&screen, xRealWidth, yRealHeight, NOT_ENOUGH_SPACE_TO_PRINT_MATRIX, sizeof(NOT_ENOUGH_SPACE_TO_PRINT_MATRIX)-1);
    if (ret!=1){
        return -ret;
    }

    // Upper boarder
   char numberStr[2] = {0};
   for (int row = 0; row < gameHeight; row++){
        for (int column = 0; column < gameWidth; column++){
            int boardValue = ((int*)board)[row*gameWidth+column];//board[column][row];
            if (boardValue > 0){
                numberStr[0] = '0'+(boardValue%10);
                printBlock(&screen, column, row, yMult, xMult, " ", numberStr);
            }
            else {
                printf("%s", getPlayerColor(-boardValue));
                isThisAPlayerHead(playerList, boardValue, column, row) ?
                    printBlock(&screen, column, row, yMult, xMult, HEAD_CHAR, HEAD_SPECIAL_CHAR):
                    printBlock(&screen, column, row, yMult, xMult, BODY_CHAR, BODY_CHAR); //isThisAPlayerHead(playerList, boardValue, column, row) ? "▓" : "▓"
                printf("%s", getPlayerColor(-1));
            }
        }
    }

    fflush(stdout);
    return yRealHeight;
}