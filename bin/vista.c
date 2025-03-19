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

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)


typedef struct {
    sem_t A_printNeeded; // Se usa para indicarle a la vista que hay cambios por imprimir
    sem_t B_printDone; // Se usa para indicarle al master que la vista terminó de imprimir
    sem_t C; // Mutex para evitar inanición del master al acceder al estado
    sem_t D; // Mutex para el estado del juego
    sem_t E; // Mutex para la siguiente variable
    unsigned int F; // Cantidad de jugadores leyendo el estado
} GameSync_t;



typedef struct {
char a[16]; // Nombre del jugador
unsigned int b; // Puntaje
unsigned int c; // Cantidad de solicitudes de movimientos inválidas realizadas
unsigned int d; // Cantidad de solicitudes de movimientos válidas realizadas
unsigned short e, f; // Coordenadas x e y en el tablero
pid_t g; // Identificador de proceso
char h; // Indica si el jugador tiene movimientos válidos disponibles
} XXX;
typedef struct {
unsigned short i; // Ancho del tablero
unsigned short j; // Alto del tablero
unsigned int k; // Cantidad de jugadores
XXX l[9]; // Lista de jugadores
char m; // Indica si el juego se ha terminado
int n[]; // Puntero al comienzo del tablero. fila-0, fila-1, ..., fila-n-1
} YYY;




void* openmem(char* name, uint64_t size){
    int fd = shm_open(name, O_RDWR, 0);
    if (fd == -1){
        errExit("shm_open");
    }

    void* ret = mmap(NULL, size, PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0);
    if (ret == MAP_FAILED){
        errExit("mmap");
    }
    return ret;
}

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

    GameSync_t* gameSync = openmem("/game_sync", sizeof(*gameSync));

    YYY* yyy = openmem("/game_state",sizeof(*yyy));


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