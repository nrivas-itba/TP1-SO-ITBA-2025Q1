#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#include <sys/stat.h>        /* For mode constants */

#include "desordenado.h"

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