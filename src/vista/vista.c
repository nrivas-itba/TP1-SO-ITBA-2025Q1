//Inspired from
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include "../utils/utils.h"

#define NOT_ENOUGH_SPACE_TO_PRINT_MATRIX "Not enough space for matrix"
#define NOT_ENOUGH_SPACE_TO_PRINT_TABLE "Not enough space for table"

/*
    Check digits of int -> off limit is set to -1
    Returns int or -1.
*/
int numberSize(int number, int size){
    int currentSize=0;
    for(; number>=10;currentSize++){
        number/=10; // 100 -> 10 ...
    }
    if (currentSize >= size){
        return -1;
    }
    return number;
}

/*
    Check if there is enough space in screen to write something
    Returns binary.
*/
int checkWritable(const screen_t screen, char* myMsg){
    int ret = 0;
    if(screen.yWidth >= 1 && screen.xWidth >= sizeof(myMsg)){
            moveCursorScreen(screen,0,0);
            puts(myMsg);
            ret = 1;
    }
    return ret;
}

/*
    Check if there is enough space in screen to print something
    Returns binary.
*/
int checkPrintable(int srcWidth, int srcHeight, int myWidth, int myHeight, const screen_t screen){
    // can print
    int ret = 1;
    if(srcWidth < myWidth || srcHeight < myHeight){
        // cannot print
        ret = 0;
        for(int x=0; x<screen.xWidth;x++){
            for(int y=0; y<screen.yWidth; y++){
                moveCursorScreen(screen, x, y);
                putchar(' ');
            }
        }
        fflush(stderr);
    }
    return ret;
}


/*
    Prints the player stats table
    Returns how many lines where printed.
*/
int printPlayerStats(player_t* players, unsigned int playerCount, const screen_t screen){

    // check width and heigth (max_players) -> ERR_MSG
        // print header
        // print iterating players til max (indicate padding to printf and # of chars i want,data type)
        // print separator
        // return lines printed (minimum of 5)
        // aux arr with indexes -> sort with criteria (complex)

    int xWidth = screen.xWidth-2;
    int yHeight = screen.yWidth-2;

    int myWidth = 25 + MAX_PLAYER_NAME_LEN;

    //fix magic numbers
    if( !checkPrintable(xWidth,yHeight,myWidth,playerCount,screen) ){
        return checkWritable(screen, NOT_ENOUGH_SPACE_TO_PRINT_TABLE);
    }

    // Header
    moveCursorScreen(screen,0,0);
    for (int x = 0; x < myWidth; x++){
        printf("═");
    }
    moveCursorScreen(screen,0,1);
    printf(" %16s %5s %5s %5s %s \n", "Name", "Score", "InvRq", "ValRq", "❤️");
    moveCursorScreen(screen,0,2);
    for (int x = 0; x < myWidth; x++){
        printf("═");
    }

    for (int fila = 0; fila < playerCount; fila++){
        moveCursorScreen(screen,0,3+fila);
        printf(" %16.16s %5d %5d %5d %s \n", players[fila].name, numberSize(players[fila].score, 5), numberSize(players[fila].invalidMovementRequestsCount, 5), numberSize(players[fila].validMovementRequestsCount, 5), (players[fila].canMove ? "💀" : "😀")); 
    }

    // Lowe border
    moveCursorScreen(screen,0,3+playerCount);
    for (int x = 0; x < myWidth; x++){
        printf("═");
    }

    return 4+playerCount;
}

/*
    Prints the board
    Returns how many lines where printed.
*/
int printGame(int gameWidth, int gameHeight, int board[gameWidth][gameHeight], const screen_t screen){
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

    if( !checkPrintable(xWidth,yHeight,gameWidth,gameHeight,screen) ){
        return checkWritable(screen, NOT_ENOUGH_SPACE_TO_PRINT_MATRIX);
    }

    int xMult = xWidth/gameWidth;
    int yMult = yHeight/gameHeight;

    // Upper boarder
    moveCursorScreen(screen,0,0);
    printf("╭");
    for (int x = 0; x < gameWidth*xMult; x++){
        printf("═");
    }
    printf("╮");

    /*
    for (int i = 0; i < gameWidth; i++){
        moveCursorScreen(screen,0,1+i);
        printf("║");    // Left border
        for (int j = 0; j < gameHeight; j++){
            if(matrix[i][j]>0){
                printf(" %d ", matrix[i][j]);
            }
            else{
                printf("%s███%s", playerColors[-matrix[i][j]],WHITE);
            }
        }
        printf("║");  // Right border
    }
    */
   for (int fila = 0; fila < gameHeight; fila++){
        for (int columna = 0; columna < gameWidth; columna++){
            int boardValue = board[fila][columna];
            for(int filaInner = 0; filaInner<yMult; filaInner++){
                if(columna == 0){
                    moveCursorScreen(screen,0,1+fila*yMult+filaInner);
                    printf("║");    // Left border
                }
                for(int columnaInner = 0; columnaInner<xMult;columnaInner++){
                    moveCursorScreen(screen,1+columna*xMult+columnaInner,1+fila*yMult+filaInner);
                    if(boardValue>0){
                        if(columnaInner==xMult/2 && filaInner == yMult/2){
                            printf("%d", boardValue%10);
                        }
                        else{
                            putchar(' ');
                        }
                    }
                    else{
                        printf("%s", playerColors[-boardValue]);
                        printf("█");
                        printf("%s", WHITE);
                    }
                }
                if(columna == gameWidth-1){
                    moveCursorScreen(screen,1+gameWidth*xMult,1+fila*yMult+filaInner);
                    printf("║");    // Right border
                    fflush(stdout);
                }
            }
        }
    }

    // Lower border
    moveCursorScreen(screen,0,1+gameWidth*yMult);
    printf("╰");
    for (int i = 0; i < gameWidth*xMult; i++){
        printf("═");

    }
    printf("╯");
    fflush(stdout);

    return 2+gameHeight*yMult;
}

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv);
    char isGameOver = 0;
    while(!isGameOver){
        sWait(&(game.sync->printNeeded)); //Waint until master wants to print
        screen_t screen = buildScreen(1,14);
        moveCursor(1,screen.yOffset);
        screen.yOffset += printPlayerStats(game.state->playerList, game.state->playerCount, screen);
        screen.yOffset += printGame(game.gameWidth, game.gameHeight, (void*)(game.state->board), screen);
        moveCursor(1,screen.yOffset);
        isGameOver = game.state->isOver;
        sPost(&(game.sync->printDone)); //Tell the master that we have finished printing.

    }
    return 0;
}