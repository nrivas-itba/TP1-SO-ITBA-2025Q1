//Inspired from
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include "desordenado.h"

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv);
    while(1){
        if(game.yyy->m){ //TODO pienso q solo deberiamos leer esta variable mientras tenemos permitido acceder al tablero, es decir: Durante "printNeeded"... Pero si muevo el if a desp de "printNeeded": No termina nunca
            return 0;
        }
        fprintf(stderr,"Wstart\n");
        if (sem_wait(&(game.sync->A_printNeeded)) == -1){ //Waint until master wants to print
            errExit("sem_wait");
        }

        fprintf(stderr,"*imprime*\n");

        if (sem_post(&(game.sync->B_printDone)) == -1){ //Tell the master that we have finished printing.
            errExit("sem_post");
        }
        fprintf(stderr,"Wend\n");
    }
    return 0;
};