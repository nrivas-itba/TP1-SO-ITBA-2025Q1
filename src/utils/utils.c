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
//TODO Ninguno de estos define esta verificado que sea necesario. Por favor revisar si se puede sacar alguno


#include "utils.h"

void* openmem(char* name, uint64_t size, char readOnly){
    int fd = shm_open(name, O_RDWR, 0);
    if (fd == -1){
        errExit("shm_open");
    }

    void* ret = mmap(NULL,
                size,
                PROT_READ | (readOnly ? 0 : PROT_WRITE),
                MAP_SHARED,
                fd,
                0);
    if (ret == MAP_FAILED){
        errExit("mmap");
    }
    return ret;
}

game_t openGame(int argc, char* argv[]){
    game_t ret;
    if (argc != 3){
        errExit("invalid argc");
    }

    ret.gameWidth  = atoi(argv[1]);
    ret.gameHeight = atoi(argv[2]);
    
    if (ret.gameWidth < 0 || ret.gameWidth < 0){
        errExit("invalid argc");
    }

    ret.sync = openmem("/game_sync", sizeof(*ret.sync), 0);
    ret.state = openmem("/game_state",sizeof(*ret.state) + (ret.gameWidth * ret.gameHeight) * sizeof((ret.state->board)[0]), 1);
    return ret;
}

void sWait(sem_t* sem){
    if (sem_wait(sem) == -1){ //Waint until master wants to print
        errExit("sem_wait");
    }
}
void sPost(sem_t* sem){
    if (sem_post(sem) == -1){ //Tell the master that we have finished printing.
        errExit("sem_post");
    }
}