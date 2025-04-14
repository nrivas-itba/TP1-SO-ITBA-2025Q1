// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "gameLogic.h"
#include <time.h>
#include <unistd.h>
#include "../../utils/utils.h"
#include "../../utils/ipc.h"

typedef struct {
    unsigned int x;
    unsigned int y;
} coords_t;

void askViewToPrint(int delay,gameSync_t* gameSync) {
  sPost(&(gameSync->printNeeded));
  sWait(&(gameSync->printDone)); //ChompChamps (6b398ab0f1be541975002579f26f509f) does not check sem errors, but we check them.
  usleep(delay * 1000);
}

void lockGameStateReads(gameSync_t* gameSync) {
  sWait(&(gameSync->masterWantsToReadMutex));
  sWait(&(gameSync->readGameStateMutex));
  sPost(&(gameSync->masterWantsToReadMutex));
  return;
}

void unlockGameStateReads(gameSync_t* gameSync) {
  sPost(&(gameSync->readGameStateMutex));
  return;
}

void endGame(gameState_t* gameState,gameSync_t* gameSync) {
  lockGameStateReads(gameSync);
  gameState->isOver = 1;
  unlockGameStateReads(gameSync);
  return;
}

char canAnyPlayerMove(gameState_t* gameState){
  for(unsigned int i = 0; i < gameState->playerCount; i++){
    if (!gameState->playerList[i].isBlocked){
      return 1;
    }
  }
  return 0;
}

void killPlayer(unsigned int nextPlayerIndex, gameState_t* gameState, gameSync_t* gameSync, struct pollfd* pollFdArr){
    if(gameSync){
      lockGameStateReads(gameSync); //TODO esta feito
    }
    gameState->playerList[nextPlayerIndex].isBlocked = 1;
    if(gameSync){
      unlockGameStateReads(gameSync);
    }
    pollFdArr[nextPlayerIndex].events = 0;
    pollFdArr[nextPlayerIndex].revents = 0; //TODO no se si es necesario
    pollFdArr[nextPlayerIndex].fd = -1; //TODO no se si esta bien, o si hacer el bitwsie negativo
}

char hasAnyValidDirection(gameState_t* gameState, unsigned int playerIndex);
//The ChompChamps implementation of this round-rovin like function was recursive and used many static variables. It was not realistic to try to recreate it.
int getNextMove(unsigned int* nextPlayerIndex, int timeout, gameState_t* gameState, gameSync_t* gameSync,  struct pollfd* pollFdArr, char* directionPtr, time_t timeStart){
    static int numberOfReadyPlayers = 0; //TODO analizar si se puede / deberia sacar (hacer no estatica)
    while(1){ //TODO probably some condition should be put here instead of having "ifs" with returns
        for(unsigned int i = 0; numberOfReadyPlayers > 0 && i < gameState->playerCount; i++){ //TODO this for is very wierd
            // if(!hasAnyValidDirection(gameState,*nextPlayerIndex)){
            //     killPlayer(*nextPlayerIndex, gameState, gameSync,pollFdArr);
            // }
            if(pollFdArr[*nextPlayerIndex].revents!=0){
                numberOfReadyPlayers--;
                if(!(pollFdArr[*nextPlayerIndex].revents & POLLIN) || readPipe(pollFdArr[*nextPlayerIndex].fd,directionPtr) == -1){ //TODO this is the wierdest if ever, it abuses of "||" lazyness to make function calls
                    killPlayer(*nextPlayerIndex, gameState, gameSync,pollFdArr);
                    return -1;
                }
                return 0;
            }
            *nextPlayerIndex=((*nextPlayerIndex)+1)%gameState->playerCount;
        }
        numberOfReadyPlayers = getNumberOfReadyFd(timeout, gameState->playerCount, pollFdArr, &timeStart);
        if (numberOfReadyPlayers == 0) {
            return -1;
        }
    }
}

static inline int getDx(char direction){
    return (direction == 1 || direction == 2 || direction == 3)
         - (direction == 5 || direction == 6 || direction == 7);
}
static inline int getDy(char direction){
    return (direction == 3 || direction == 4 || direction == 5)
         - (direction == 1 || direction == 0 || direction == 7);
}

static inline char isWithin(int minInclusive, int value, int maxExclusive){
    return minInclusive <= value && value < maxExclusive;
}

static inline char isDirectionInsideOfBoard(coords_t* coords, unsigned short width, unsigned short height){
  return isWithin(0, coords->x, width) && isWithin(0, coords->y, height);
}


static inline char isNewPositionEmpty(coords_t* coords, unsigned short width, int* board){
    return isWithin(MIN_BOARD_VALUE, board[width * (coords->y) + coords->x], MAX_BOARD_VALUE+1);
}

char isDirectionValid(gameState_t* gameState, unsigned int playerIndex, char direction, coords_t* coords) {
    *coords = (coords_t){
        .x = gameState->playerList[playerIndex].x + getDx(direction),
        .y = gameState->playerList[playerIndex].y + getDy(direction)
    };
    return isDirectionInsideOfBoard(coords, gameState->width, gameState->height)
           && isNewPositionEmpty(coords, gameState->width, gameState->board);
}

char hasAnyValidDirection(gameState_t* gameState, unsigned int playerIndex){
    coords_t coords;
    for(char i = 0; i<=MAX_DIRECTION; i++){
        if(isDirectionValid(gameState, playerIndex, i, &coords)){
            return 1;
        }
    }
    return 0;
}

void updateAllPlayers(gameState_t* gameState, struct pollfd* pollFdArr){
    for(unsigned int i = 0; i < gameState->playerCount; i++){
        if(!gameState->playerList[i].isBlocked && !hasAnyValidDirection(gameState,i)){
          killPlayer(i,gameState,0,pollFdArr);
        }
    }
}

void applyCoords(gameState_t* gameState, unsigned int playerIndex, coords_t* coords){
    player_t* player = &(gameState->playerList[playerIndex]);
    player->score += gameState->board[gameState->width * coords->y + coords->x]; //TODO quizas hacer una variable player
    gameState->board[gameState->width * coords->y + coords->x] = -playerIndex;
    player->x = coords->x;
    player->y = coords->y;
    player->validMovementRequestsCount++;
}

void processMove(gameState_t* gameState, gameSync_t* gameSync, unsigned int playerIndex, char direction, struct pollfd* pollFdArr, time_t* timeStart) {
  coords_t coords;
  if (isDirectionValid(gameState, playerIndex, direction, &coords)) {
    lockGameStateReads(gameSync);
    applyCoords(gameState, playerIndex, &coords);
    updateAllPlayers(gameState, pollFdArr);
    gameState->isOver = !canAnyPlayerMove(gameState);
    unlockGameStateReads(gameSync);
    *timeStart = time(NULL);
  }
  else {
    lockGameStateReads(gameSync);
    gameState->playerList[playerIndex].invalidMovementRequestsCount++;
    // gameState->playerList[playerIndex].isBlocked = !hasAnyValidDirection(gameState, playerIndex);
    // gameState->isOver = !canAnyPlayerMove(gameState);
    unlockGameStateReads(gameSync);
  }
}

void game(gameConfig_t* gameConfig, pipefd_t* pipefd) {
  struct pollfd pollFdArr[MAX_PLAYERS];

  setUpPollFdArr(gameConfig->state->playerCount, pipefd, pollFdArr);

  unsigned int nextPlayerIndex = 0;
  char direction;
  time_t timeStart = time(NULL);
  while(1) { //TODO reordenando, se puede hacer un while (gameState->isOver)
    if (gameConfig->view) {
      askViewToPrint(gameConfig->delay, gameConfig->sync);
    }
    if (gameConfig->state->isOver){
      break;
    }
    if (getNextMove(&nextPlayerIndex, gameConfig->timeout, gameConfig->state, gameConfig->sync, pollFdArr, &direction, timeStart) == -1) {
      endGame(gameConfig->state, gameConfig->sync);
    }
    else {
      processMove(gameConfig->state, gameConfig->sync, nextPlayerIndex, direction, pollFdArr, &timeStart);
    }
    nextPlayerIndex = (nextPlayerIndex + 1) % gameConfig->state->playerCount;
  }
}