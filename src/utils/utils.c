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
    if (isPublic){ //TODO i dont understand why this is neccesary
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

void unlinkMem(char* name, size_t size, void* address, int* fd){
  if (munmap(address, size) == -1) {
    errExit("munmap");
  }
  
  if (close(*fd) == -1) {
    errExit("close");
  }

  if (shm_unlink(name) == -1){
    errExit("shm_unlink");
  }
  return;
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

void* openmem(char* name, size_t size, char readOnly){
    int fd = shm_open(name, readOnly ? O_RDONLY : O_RDWR, 0);
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

    ret.sync = openmem(GAME_SYNC, sizeof(*ret.sync), 0);
    ret.state = openmem(GAME_STATE,sizeof(*ret.state) + (ret.gameWidth * ret.gameHeight) * sizeof((ret.state->board)[0]), 1);
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

screen_t buildScreen(int xOffset,int yOffset){
        screen_t ret;
        struct winsize w;
        // Query terminal size
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
            errExit("ioctl, is the stdOut connected to a console?");
        }
        ret.xWidth = w.ws_col-xOffset;
        ret.yWidth = w.ws_row-yOffset;
        ret.xOffset = xOffset;
        ret.yOffset = yOffset;
        return ret;
}

int moveCursorScreen(screen_t screen, int x, int y){
    return moveCursor(screen.xOffset+x, screen.yOffset+y);
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