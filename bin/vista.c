//Inspired from
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include "desordenado.h"

uint64_t main(uint64_t argc, char* argv[]){
    game_t game = openGame(argc, argv);
    while(!game.yyy->m){
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