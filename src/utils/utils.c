// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
#include <sys/ioctl.h>
//TODO Ninguno de estos define esta verificado que sea necesario. Por favor revisar si se puede sacar alguno


#include "utils.h"

#define MODE_RW0_R00_R00 S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH
#define MODE_RW0_RW0_RW0 MODE_RW0_R00_R00 | S_IWGRP | S_IWOTH

void* createShm(char* name, size_t size, char isPublic, int* fd){
    mode_t prevMode;
    if (isPublic){
        prevMode = umask((mode_t)0); //This systeam call never fails
    }
    *fd = shm_open(name, O_RDWR | O_CREAT, isPublic ? MODE_RW0_RW0_RW0 : MODE_RW0_R00_R00);
    if (*fd == -1){
        errExit("shm_open");
    }
    
    if (-1 == ftruncate(*fd, size)){
        errExit("ftruncate");
    }

    void* ret = mmap(NULL,
                size,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                *fd,
                0);
    
    if (ret == MAP_FAILED){
        errExit("mmap");
    }    
    if (isPublic){
        umask(prevMode);
    }
    return ret;
}

void closeMem(char* name, size_t size, void* address, int* fd){
  if (munmap(address, size) == -1) {
    errExit("munmap");
  }
  
  if (close(*fd) == -1) {
    errExit("close");
  }

  if(name){
    if (shm_unlink(name) == -1){
        errExit("shm_unlink");
    }
  }
  return;
}

void closeFd(int fd)
{
    if (close(fd) == -1){
        errExit("close");
    }
    
}

void semInit(sem_t* sem, unsigned int value){
  if (sem_init(sem,!0,value) == -1) {
    errExit("sem_init");
  }
}

void semDestroy(sem_t* sem){
  if (sem_destroy(sem) == -1) {
    errExit("sem_destroy");
  }
}

void* openMem(char* name, size_t size, char readOnly, int* fd){
    *fd = shm_open(name, readOnly ? O_RDONLY : O_RDWR, 0);
    if (*fd == -1){
        errExit("shm_open");
    }

    void* ret = mmap(NULL,
                size,
                PROT_READ | (readOnly ? 0 : PROT_WRITE),
                MAP_SHARED,
                *fd,
                0);
    if (ret == MAP_FAILED){
        errExit("mmap");
    }
    return ret;
}

game_t openGame(int argc, char* argv[]){
    if (argc != 3){
        errExit("invalid argc");
    }

    game_t ret = (game_t){
        .gameWidth  = atoi(argv[1]),
        .gameHeight = atoi(argv[2]),
        .sync = openMem(GAME_SYNC, sizeof(*ret.sync), 0, &(ret.syncFd)),
        .state = openMem(GAME_STATE,sizeof(*ret.state) + (ret.gameWidth * ret.gameHeight) * sizeof((ret.state->board)[0]), 1, &(ret.stateFd))
    };
    
    if (ret.gameWidth <= 0 || ret.gameHeight <= 0){
        errExit("Board dimensions cant be less or equal to 0");
    }

    if (ret.gameHeight != ret.state->height || ret.gameWidth != ret.state->width){
        errExit("Board dimensions dont match");
    }

    return ret;
}

void closeGame(game_t* game){
    closeMem(0, sizeof(*game->sync), game->sync, &(game->syncFd));
    closeMem(0, sizeof(*game->state) + (game->gameWidth * game->gameHeight) * sizeof((game->state->board)[0]), game->state, &(game->stateFd));
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

unsigned int decimalLen(int number){
    unsigned int len = 0;
    if(number<0){
        number *= -1;
        len++;
    }
    do {
        len++;
    } while(number /= 10);
    return len;
}