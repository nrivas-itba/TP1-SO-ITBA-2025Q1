//Inspired from
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include "../utils/utils.h"

void print_game(int gameWidth, int gameHeight, int matrix[gameWidth][gameHeight]){
    static const char* playerColors[]= {
        RED,
        RED,
        GREEN,
        BLUE,
        CYAN,
        MAGENTA,
        YELLOW,
        WHITE,
        GRAY
    };
    // Upper boarder
    printf("╭");
    for (int i = 0; i < gameWidth; i++)
        printf("═══");
    printf("╮\n");

    for (int i = 0; i < gameWidth; i++){
        printf("║");    // Left border
        for (int j = 0; j < gameHeight; j++){
            if(matrix[i][j]>0){
                printf(" %d ", matrix[i][j]);
            }
            else{
                printf("%s███%s", playerColors[-matrix[i][j]],WHITE);
            }
        }
    //printf("%d ", matrix[i][j] <= 0 ? matrix[i][j] : 'a'-'0');
        printf("║\n");  // Right border
    }

    // Lower border
    printf("╰");
    for (int i = 0; i < gameWidth; i++)
        printf("═══");
    printf("╯\n");
}

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv);
    while(1){
        if(game.state->isOver){ //TODO pienso q solo deberiamos leer esta variable mientras tenemos permitido acceder al tablero, es decir: Durante "printNeeded"... Pero si muevo el if a desp de "printNeeded": No termina nunca
            return 0;
        }
        fprintf(stderr,"Wstart\n");
        if (sem_wait(&(game.sync->printNeeded)) == -1){ //Waint until master wants to print
            errExit("sem_wait");
        }

        print_game(game.gameWidth, game.gameHeight, (void*)(game.state->board));

        if (sem_post(&(game.sync->printDone)) == -1){ //Tell the master that we have finished printing.
            errExit("sem_post");
        }
        fprintf(stderr,"Wend\n");
    }
    return 0;
};