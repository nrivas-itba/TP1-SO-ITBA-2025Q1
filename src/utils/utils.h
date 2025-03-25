#ifndef UTILS_H
#define UTILS_H

/*
https://en.wikipedia.org/wiki/ANSI_escape_code
Code	Abbr	Name	Effect
CSI n A	CUU	Cursor Up	Moves the cursor n (default 1) cells in the given direction. If the cursor is already at the edge of the screen, this has no effect.
CSI n B	CUD	Cursor Down
CSI n C	CUF	Cursor Forward
CSI n D	CUB	Cursor Back
CSI n E	CNL	Cursor Next Line	Moves cursor to beginning of the line n (default 1) lines down. (not ANSI.SYS)
CSI n F	CPL	Cursor Previous Line	Moves cursor to beginning of the line n (default 1) lines up. (not ANSI.SYS)
CSI n G	CHA	Cursor Horizontal Absolute	Moves the cursor to column n (default 1). (not ANSI.SYS)
CSI n ; m H	CUP	Cursor Position	Moves the cursor to row n, column m. The values are 1-based, and default to 1 (top left corner) if omitted. A sequence such as CSI ;5H is a synonym for CSI 1;5H as well as CSI 17;H is the same as CSI 17H and CSI 17;1H
CSI n J	ED	Erase in Display	Clears part of the screen. If n is 0 (or missing), clear from cursor to end of screen. If n is 1, clear from cursor to beginning of the screen. If n is 2, clear entire screen (and moves cursor to upper left on DOS ANSI.SYS). If n is 3, clear entire screen and delete all lines saved in the scrollback buffer (this feature was added for xterm and is supported by other terminal applications).
CSI n K	EL	Erase in Line	Erases part of the line. If n is 0 (or missing), clear from cursor to the end of the line. If n is 1, clear from cursor to beginning of the line. If n is 2, clear entire line. Cursor position does not change.
CSI n S	SU	Scroll Up	Scroll whole page up by n (default 1) lines. New lines are added at the bottom. (not ANSI.SYS)
CSI n T	SD	Scroll Down	Scroll whole page down by n (default 1) lines. New lines are added at the top. (not ANSI.SYS)
CSI n ; m f	HVP	Horizontal Vertical Position	Same as CUP, but counts as a format effector function (like CR or LF) rather than an editor function (like CUD or CNL). This can lead to different handling in certain terminal modes.[5]: Annex A 
CSI n m	SGR	Select Graphic Rendition	Sets colors and style of the characters following this code
CSI 5i		AUX Port On	Enable aux serial port usually for local serial printer
CSI 4i		AUX Port Off	Disable aux serial port usually for local serial printer

CSI 6n	DSR	Device Status Report	Reports the cursor position (CPR) by transmitting ESC[n;mR, where n is the row and m is the column.
*/


#include <semaphore.h>


#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)

#define color(R,G,B) "\033[38;2;" R ";" G ";" B "m"


#define BLACK color(,,)
#define BLUE color(, , "255")
#define GREEN color(, "255", )
#define CYAN color(, "255", "255")
#define RED color("255", , )
#define MAGENTA color("255", , "255")
#define YELLOW color("255", "255", )
#define WHITE color("255", "255", "255")
#define GRAY color("127", "127", "127")

#define moveCursor(x,y) printf("\033[%d;%dH",y,x); //TODO esto es movimiento ABSOLUTO, tamb seria facil implementar movimiento RELATIVO (creo q este todo no es necesario, creo q no necesitamos movimiento relativo) //TODO macros solo deberian generar strings, no ejecutar codigo. Si se quiere ejecutar codigo, meterlo en funcion

#define MAX_PLAYERS 9

typedef struct {
    sem_t printNeeded; // Se usa para indicarle a la vista que hay cambios por imprimir
    sem_t printDone; // Se usa para indicarle al master que la vista terminó de imprimir
    sem_t masterWantsToReadMutex; // Mutex para evitar inanición del master al acceder al estado
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
char notHasAnyValidMovements; // Indica si el jugador tiene movimientos válidos disponibles
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

typedef struct {
    int xWidth;
    int yWidth;
    int xOffset;
    int yOffset;
} screen_t;

game_t openGame(int argc, char* argv[]); //TODO no me gusta que una funcion reciva argc y argv

void sWait(sem_t* sem);
void sPost(sem_t* sem);

screen_t buildScreen(int xOffset, int yOffset);
int moveCursorScreen(screen_t screen, int x, int y);

#endif