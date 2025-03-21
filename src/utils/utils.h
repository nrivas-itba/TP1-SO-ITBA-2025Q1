#ifndef UTILS_H
#define UTILS_H

#include <semaphore.h>


#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)

#define color(R,G,B) "\033[38;2;" R ";"G";"B"m"


#define BLACK color(,,)
#define RED color("255", , )
#define GREEN color(, "255", )
#define BLUE color(, , "255")
#define CYAN color(, "255", "255")
#define MAGENTA color("255", , "255")
#define YELLOW color("255", "255", )
#define WHITE color("255", "255", "255")
#define GRAY color("127", "127", "127")


typedef struct {
    sem_t printNeeded; // Se usa para indicarle a la vista que hay cambios por imprimir
    sem_t printDone; // Se usa para indicarle al master que la vista terminó de imprimir
    sem_t C; // Mutex para evitar inanición del master al acceder al estado
    sem_t readGameStateMutex; // Mutex para el estado del juego
    sem_t readersCountMutex; // Mutex para la siguiente variable
    unsigned int readersCount; // Cantidad de jugadores leyendo el estado
} gameSync_t;

typedef struct {
char name[16]; // Nombre del jugador
unsigned int score; // Puntaje
unsigned int invalidMovementRequestsCount; // Cantidad de solicitudes de movimientos inválidas realizadas
unsigned int validMovementRequestsCount; // Cantidad de solicitudes de movimientos válidas realizadas
unsigned short x, y; // Coordenadas x e y en el tablero
pid_t pid; // Identificador de proceso
char hasValidMovements; // Indica si el jugador tiene movimientos válidos disponibles
} player_t;

typedef struct {
unsigned short width; // Ancho del tablero
unsigned short height; // Alto del tablero
unsigned int playerCount; // Cantidad de jugadores
player_t playerList[9]; // Lista de jugadores
char isOver; // Indica si el juego se ha terminado
int board[]; // Puntero al comienzo del tablero. fila-0, fila-1, ..., fila-n-1
} gameState_t;

typedef struct {
    int gameWidth;
    int gameHeight;
    gameSync_t* sync;
    gameState_t* state;
} game_t;

game_t openGame(int argc, char* argv[]); //TODO no me gusta que una funcion reciva argc y argv

#endif