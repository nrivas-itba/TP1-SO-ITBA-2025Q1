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
    fprintf(stderr,"%s", "Hola");
    for(int i = 1; i< argc; i++){
        fprintf(stderr,"%d\n", atoi(argv[i]));
    }
    fprintf(stderr,"end\n");



    if (argc != 3){
        errExit("invalid argc");
    }
    uint64_t width  = atoi(argv[1]);
    uint64_t height = atoi(argv[2]);

    GameSync_t* gameSync = openmem("/game_sync", sizeof(*gameSync), 0);

    YYY* yyy = openmem("/game_state",sizeof(*yyy), 1);


    /*for(int i = 0; i<sizeof(*gameSync)/sizeof(unsigned int);i++){
        fprintf(stderr,"%d\n",((int*)gameSync)[i]);
    }*/

    fprintf(stderr,"end3\n");
    while(!yyy->m){
        fprintf(stderr,"Wstart\n");
        if (sem_wait(&(gameSync->A_printNeeded)) == -1){ //Esto deberia esperar a que el master quiera imprimir
            errExit("sem_wait");
        }

        fprintf(stderr,"*imprime*\n");

        if (sem_post(&(gameSync->B_printDone)) == -1){ //Esto deberia decirle al master que ya terminé de imprimir
            errExit("sem_post");
        }
        
        fprintf(stderr,"Wend\n");
    }
    return 0;
};