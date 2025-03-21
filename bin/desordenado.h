#ifndef DESORDENADO_H
#define DESORDENADO_H

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
} player_t;
typedef struct {
unsigned short i; // Ancho del tablero
unsigned short j; // Alto del tablero
unsigned int k; // Cantidad de jugadores
player_t l[9]; // Lista de jugadores
char m; // Indica si el juego se ha terminado
int n[]; // Puntero al comienzo del tablero. fila-0, fila-1, ..., fila-n-1
} YYY;


typedef struct {
    GameSync_t* sync;
    YYY* yyy;
} game_t;

game_t openGame(uint64_t gameWidth, uint64_t gameHeight);

#endif