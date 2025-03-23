//Inspired from
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include "../utils/utils.h"

#define NOT_ENOUGH_SPACE_TO_PRINT_MATRIX "Not enough space"

void print_game(int gameWidth, int gameHeight, int matrix[gameWidth][gameHeight], screen_t screen){
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
    
    if(screen.xWidth < gameWidth+2 || screen.yWidth < gameHeight+2){
        for(int x=0; x<screen.xWidth;x++){
            for(int y=0; y<screen.yWidth; y++){
                moveCursorScreen(screen, x, y);
                putchar(' ');
            }
        }
        if(screen.yWidth && screen.xWidth >= sizeof(NOT_ENOUGH_SPACE_TO_PRINT_MATRIX)){
            moveCursorScreen(screen,0,0);
            puts(NOT_ENOUGH_SPACE_TO_PRINT_MATRIX);
        }
        fflush(stderr);
        return;
    }

    // Upper boarder
    moveCursorScreen(screen,0,0);
    printf("╭");
    for (int i = 0; i < gameWidth; i++)
        printf("═══");
    printf("╮");


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

    moveCursorScreen(screen,0,1+gameWidth);
    // Lower border
    printf("╰");
    for (int i = 0; i < gameWidth; i++)
        printf("═══");
    printf("╯");
    fflush(stdout);
}

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv);
    while(1){
        if(game.state->isOver){ //TODO pienso q solo deberiamos leer esta variable mientras tenemos permitido acceder al tablero, es decir: Durante "printNeeded"... Pero si muevo el if a desp de "printNeeded": No termina nunca
            return 0;
        }
        sWait(&(game.sync->printNeeded)); //Waint until master wants to print

        moveCursor(0,13);
        //printf("ChompChamps!\n");
        print_game(game.gameWidth, game.gameHeight, (void*)(game.state->board), buildScreen(5,14));

        sPost(&(game.sync->printDone)); //Tell the master that we have finished printing.
    }
    return 0;
}