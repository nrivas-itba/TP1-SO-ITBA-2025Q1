//Inspired from
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include "../utils/utils.h"

#define NOT_ENOUGH_SPACE_TO_PRINT_MATRIX "Not enough space for matrix"
#define NOT_ENOUGH_SPACE_TO_PRINT_TABLE "Not enough space for table"

/*
    Prints the player stats table
    Returns how many lines where printed.
*/
int printPlayerStats(player_t* players,  const screen_t screen){

    // check width and heigth (max_players) -> ERR_MSG
        // print header
        // print iterating players til max
        // print separator
        // return lines printed (minimum of 5)

    
    
    return 0;
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
    if(xWidth < gameWidth || yHeight < gameHeight){
        int ret = 0;
        for(int x=0; x<screen.xWidth;x++){
            for(int y=0; y<screen.yWidth; y++){
                moveCursorScreen(screen, x, y);
                putchar(' ');
            }
        }
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
                    printf("║");    // Left border
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
    return 2+gameWidth*yMult;
}

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv);
    while(1){
        if(game.state->isOver){ //TODO pienso q solo deberiamos leer esta variable mientras tenemos permitido acceder al tablero, es decir: Durante "printNeeded"... Pero si muevo el if a desp de "printNeeded": No termina nunca
            return 0;
        }
        sWait(&(game.sync->printNeeded)); //Waint until master wants to print

        moveCursor(1,14);
        screen_t screen = buildScreen(1,14);
        screen.yOffset += printPlayerStats(game.state->playerList, screen);
        screen.yOffset += printGame(game.gameWidth, game.gameHeight, (void*)(game.state->board), screen);

        sPost(&(game.sync->printDone)); //Tell the master that we have finished printing.
    }
    return 0;
}