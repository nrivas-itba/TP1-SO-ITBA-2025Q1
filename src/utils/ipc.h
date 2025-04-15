#ifndef IPC_H
#define IPC_H

#include <semaphore.h>
#include <stdio.h>
#include <poll.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)

typedef struct {
    int read;
    int write;
} pipefd_t;

/// @brief Create a shared memory segment.
/// @param name 
/// @param size 
/// @param isPublic 
/// @param fd 
/// @return A pointer to the map area.
void* createShm(char* name, size_t size, char isPublic, int* fd);

/// @brief Open the shared memory segment.
/// @param name 
/// @param size 
/// @param readOnly 
/// @param fd 
/// @return A pointer to the map area
void* openMem(char* name, size_t size, char readOnly, int* fd);

/// @brief Close the shared memory segment.
/// @param name 
/// @param size 
/// @param address 
/// @param fd 
void closeMem(char* name, size_t size, void* address, int* fd);

/// @brief Close a file descriptor
/// @param fd 
void closeFd(int fd);

/// @brief Initialize the semaphore.
/// @param sem 
/// @param value 
void semInit(sem_t* sem, unsigned int value);

/// @brief Destroy the semaphore.
/// @param sem 
void semDestroy(sem_t* sem);

/// @brief Wait a semaphore.
/// @param sem 
void sWait(sem_t* sem);

/// @brief Post a semaphore.
/// @param sem 
void sPost(sem_t* sem);

/// @brief Execute a process, passing two integer args.
/// @param process 
/// @param width 
/// @param decimalLenWidth 
/// @param height 
/// @param decimalLenHeight 
void execveWithArgs(char* process, int width, unsigned int decimalLenWidth, int height, unsigned int decimalLenHeight);

/// @brief Duplicate a file descriptor.
/// @param from 
/// @param to 
void dup2Fd(int from, int to);

/// @brief Get the number of ready file descriptors.
/// @param timeout 
/// @param nfd 
/// @param pollFdArr 
/// @param timeStart 
/// @return number of ready file descriptors.
int getNumberOfReadyFd(int timeout, unsigned int nfd, struct pollfd* pollFdArr, time_t* timeStart);

/// @brief Create a set number of pipes.
/// @param nfd The amount of pipes to create.
/// @param pipefd 
void createPipes(unsigned int nfd, pipefd_t* pipefd);

/// @brief Close a set of pipes except one.
/// @param excludedIndex Index of the pipe to omit.
/// @param nfd The amount of pipes.
/// @param pipefd 
void closeForeignPipes(unsigned int excludedIndex, unsigned int nfd, pipefd_t* pipefd);

/// @brief Close write end of a set of pipes.
/// @param nfd The amount of pipes.
/// @param pipefd 
void closeWritePipes(unsigned int nfd, pipefd_t* pipefd);

/// @brief Set up a poll fd arr.
/// @param nfd The amount of poll file descriptors.
/// @param pipefd 
/// @param pollFdArr 
void setUpPollFdArr(unsigned int nfd, pipefd_t* pipefd, struct pollfd* pollFdArr);

/// @brief Read a pipe.
/// @param fd 
/// @param directionPtr 
/// @return `0` if success, `-1` if there were no chars read.
int readPipe(int fd, char* directionPtr);

#endif