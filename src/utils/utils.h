#ifndef UTILS_H
#define UTILS_H
#include <semaphore.h>

#define MAX_PLAYERS 9

#define MAX_PLAYER_NAME_LEN 16

#define GAME_SYNC "/game_sync"
#define GAME_STATE "/game_state"

typedef struct {
    sem_t printNeeded; // Se usa para indicarle a la vista que hay cambios por imprimir
    sem_t printDone; // Se usa para indicarle al master que la vista terminó de imprimir
    sem_t masterWantsToReadMutex; // Mutex para evitar inanición del master al acceder al estado
    sem_t readGameStateMutex; // Mutex para el estado del juego
    sem_t readersCountMutex; // Mutex para la siguiente variable
    unsigned int readersCount; // Cantidad de jugadores leyendo el estado
} gameSync_t;

typedef struct {
    char name[MAX_PLAYER_NAME_LEN]; // Nombre del jugador
    unsigned int score; // Puntaje
    unsigned int invalidMovementRequestsCount; // Cantidad de solicitudes de movimientos inválidas realizadas
    unsigned int validMovementRequestsCount; // Cantidad de solicitudes de movimientos válidas realizadas
    unsigned short x, y; // Coordenadas x e y en el tablero
    pid_t pid; // Identificador de proceso
    char isBlocked; // Indica si el jugador tiene movimientos válidos disponibles
} player_t;

typedef struct {
    unsigned short width; // Ancho del tablero
    unsigned short height; // Alto del tablero
    unsigned int playerCount; // Cantidad de jugadores
    player_t playerList[MAX_PLAYERS]; // Lista de jugadores
    char isOver; // Indica si el juego se ha terminado
    int board[]; // Puntero al comienzo del tablero. fila-0, fila-1, ..., fila-n-1
} gameState_t;

typedef struct {
    int gameWidth;
    int gameHeight;
    int syncFd;
    int stateFd;
    gameSync_t* sync;
    gameState_t* state;
} game_t;

/// @brief Make the common intializations necessary before starting a game.
/// @param argc 
/// @param argv 
/// @return A game.
game_t openGame(int argc, char* argv[]);

/// @brief Close the game
/// @param game 
void closeGame(game_t* game);

/// @brief Get the len of a decimal number.
/// @param number 
/// @return The number of characters that the number would occupy in a string.
unsigned int decimalLen(int number);

#endif