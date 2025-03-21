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
    if (argc != 3){
        errExit("invalid argc");
    }
    uint64_t gameWidth  = atoi(argv[1]);
    uint64_t gameHeight = atoi(argv[2]);

    game_t game = openGame(gameWidth, gameHeight);

    while(!game.yyy->m){
        fprintf(stderr,"Wstart\n");
        if (sem_wait(&(game.sync->A_printNeeded)) == -1){ //Esto deberia esperar a que el master quiera imprimir
            errExit("sem_wait");
        }

        fprintf(stderr,"*imprime*\n");

        if (sem_post(&(game.sync->B_printDone)) == -1){ //Esto deberia decirle al master que ya terminé de imprimir
            errExit("sem_post");
        }
        
        fprintf(stderr,"Wend\n");
    }
    return 0;
};