

#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h> // Provides the definition of struct winsize
#include <unistd.h>    // For functions like ioctl()
#include "../../utils/utils.h"
#include "graphics.h"
#include <signal.h>
void signalHandlerFunction(int signalNumber){
  printf(ERRASE_IN_DISPLAY);
}
signalHandler_t setGraphicsSignalHandler(){
  signalHandler_t handler = (signalHandler_t){
    .signalHandler = signal(SIGWINCH,signalHandlerFunction)
  };
  if (handler.signalHandler == (void (*)(int)) SIG_ERR){
    errExit("signal");
  }
  return handler;
}
void deleteGraphicsSignalHandler(signalHandler_t signalHandler){
  if(signal(SIGWINCH, signalHandler.signalHandler) == (void (*)(int)) SIG_ERR){
    errExit("signal");
  }
}


screen_t buildScreen(int xOffset,int yOffset){
    struct winsize w;
    // Query terminal size
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        errExit("ioctl, is the stdOut connected to a console?");
    }
    return (screen_t){
        .xWidth = w.ws_col-xOffset,
        .yWidth = w.ws_row-yOffset,
        .xOffset = xOffset,
        .yOffset = yOffset
    };
}

screen_t modifyScreen(screen_t screen, int xOffset, int yOffset){
    return buildScreen(screen.xOffset + xOffset, screen.yOffset + yOffset);
}

int moveCursorScreen(screen_t screen, int x, int y){
    return printf(MOVE_CURSOR_FORMAT, screen.yOffset+y, screen.xOffset+x);
}

static inline void printCorners(screen_t screen, const int boardWidth, const int boardHeight){
    moveCursorScreen(screen, 0, 0);
    printf("╭");
    moveCursorScreen(screen, boardWidth-1, 0);
    printf("╮");
    moveCursorScreen(screen, 0, boardHeight-1);
    printf("╰");
    moveCursorScreen(screen, boardWidth-1, boardHeight-1);
    printf("╯");
    return;
}

void printBoard(screen_t screen, const int boardWidth, const int boardHeight){
    for (int i = 1; i < boardWidth-1; i++){
        moveCursorScreen(screen, i, 0);
        printf("═");
        moveCursorScreen(screen, i, boardHeight-1);
        printf("═");
    }

    for (int j = 1; j < boardHeight-1; j++){
        moveCursorScreen(screen, 0, j);
        printf("║");
        moveCursorScreen(screen, boardWidth-1, j);
        printf("║");
    }   
    printCorners(screen, boardWidth, boardHeight);
}

void printBlock(screen_t screen, int columna, int fila, int yMult, int xMult, char* str, char* strMiddle){
    for(int filaInner = 0; filaInner<yMult; filaInner++){
        for(int columnaInner = 0; columnaInner<xMult;columnaInner++){
            moveCursorScreen(screen,columna*xMult+columnaInner,fila*yMult+filaInner);
            if(columnaInner==xMult/2 && filaInner == yMult/2){
                printf("%s",strMiddle);
            }
            else{
                printf("%s",str);
            }
        }
    }
}

