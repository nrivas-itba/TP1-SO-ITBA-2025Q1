// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ipc.h"
#include <fcntl.h>
#include <sys/stat.h>        /* For mode constants */
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>

#include "ari.h"

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

void execveWithArgs(char* process, int width, unsigned int decimalLenWidth, int height, unsigned int decimalLenHeight){
    char arg1[decimalLenWidth+1];
    char arg2[decimalLenHeight+1];
    char* argv[] = {process, arg1, arg2,(char*)0};
  
    snprintf(arg1,decimalLenWidth+1,ARI_SNPRINTF,width); //This may be a little overkill, another aproach would be to use itoa. But ChompChamps original uses snprintf.
    snprintf(arg2,decimalLenHeight+1,ARI_SNPRINTF,height);
    char* envp[] = {(char*)0};
    if (execve(process,argv,envp) == -1) { // TODO
      errExit(ARI_EXECVE);
    }
  }


void dup2Fd(int from, int to){
    if(dup2(from,to) == -1){
        errExit("dup2");
    }
}

int pollWrapper(struct pollfd* pollFdArr, unsigned int nfd, int timeout){
    int ret = poll(pollFdArr, nfd, timeout);
    if (ret == -1) {
      errExit("poll");
    }
    return ret;
}

int getNumberOfReadyFd(int timeout, unsigned int nfd, struct pollfd* pollFdArr, time_t *timeStart) {
  int timeUntilTimeout = (timeout - difftime(time(NULL), *timeStart)) * 1000;
  if (timeUntilTimeout < 1) {
    return 0;
  }
  return pollWrapper(pollFdArr, nfd, timeUntilTimeout);
}