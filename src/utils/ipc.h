#ifndef IPC_H
#define IPC_H

#include <semaphore.h>
#include <stdio.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)

void* createShm(char* name, size_t size, char isPublic, int* fd);

void* openMem(char* name, size_t size, char readOnly, int* fd);

void closeMem(char* name, size_t size, void* address, int* fd);
void closeFd(int fd);

void semInit(sem_t* sem, unsigned int value);
void semDestroy(sem_t* sem);

void sWait(sem_t* sem);
void sPost(sem_t* sem);

#endif