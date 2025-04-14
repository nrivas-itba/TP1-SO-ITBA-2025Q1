#ifndef GRAPHICS_H
#define GRAPHICS_H

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

#include <stdio.h>
//TODO esto es movimiento ABSOLUTO, tamb seria facil implementar movimiento RELATIVO (creo q este todo no es necesario, creo q no necesitamos movimiento relativo) 
//TODO macros solo deberian generar strings, no ejecutar codigo. Si se quiere ejecutar codigo, meterlo en funcion

#define ENABLE_ALTERNATIVE_SCREEN_BUFFER "\033[?1049h"
#define DISABLE_ALTERNATIVE_SCREEN_BUFFER "\033[?1049l"


typedef struct {
    int xWidth;
    int yHeight;
    int xOffset;
    int yOffset;
} screen_t;

typedef struct {
    void (*signalHandler)(int);
} signalHandler_t;

signalHandler_t setGraphicsSignalHandler();
void deleteGraphicsSignalHandler(signalHandler_t signalNumber);
screen_t buildScreen(int xOffset, int yOffset);
screen_t buildScreenFromScreen(const screen_t* screen, int xOffset, int yOffset); //TODO deberia ser screen_t*
int moveCursorScreen(const screen_t* screen, int x, int y);

void printBorder(const screen_t* screen, const int boardWidth, const int boardHeight); //TODO deberia ser screen_t*
void printBlock(const screen_t* screen, int columna, int fila, int yMult, int xMult, char* str, char* strMiddle); //TODO deberia ser screen_t* //TODO codigo en español
int checkPrintable(screen_t* screen, int width, int tableHeight, char* errStr, size_t errStrLen);

const char* getPlayerColor(unsigned int playerIndex);

#endif