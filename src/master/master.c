// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "../utils/utils.h"
#include <libgen.h> //For __xpg_basename
#include <math.h> //For sin and cos
#include <unistd.h> //For fork
#include <sys/wait.h> //For waitpid
#include <poll.h> //for poll

#define DEFAULT_WIDTH 10
#define DEFAULT_HEIGHT 10
#define DEFAULT_DELAY 200
#define DEFAULT_TIMEOUT 10
#define DEFAULT_SEED time(NULL)

#define MAX_BOARD_VALUE 9
#define MIN_BOARD_VALUE 1

#define MAX_DIRECTION 7

#define MIN_PLAYERS 1

#define ARI_VIEW_EXIT "View exited (%d)\n"
#define ARI_PLAYER_EXIT "Player %s (%u) exited (%d) with a score of %u / %u / %u\n"

#define ARI_OPTARG "w:h:d:p:v:s:t:"
#define ARI_OPTARG_MOD "w:h:v:d:p:s:t:"
#define ARI_WRONG_USAGE "Usage: %s [-w width] [-h height] [-d delay] [-s seed] [-v view] [-t timeout] -p player1 player2 ...\n"
#define ARI_NO_VIEW "Info: Delay parameter ignored since there is no view.\n"
#define ARI_NO_PLAYER "Error: At least one player must be specified using -p.\n"
#define ARI_TOO_MANY_PLAYERS "Error: At most %d players can be specified using -p.\n"
#define ARI_WRONG_BOARD_DIMENSIONS "Error: Minimal board dimensions: %dx%d. Given %dx%d\n"

#define ARI_CLEAR "clear"
#define ARI_WIDTH "width: %d\n"
#define ARI_HEIGHT "height: %d\n"
#define ARI_DELAY "delay: %u\n"
#define ARI_TIMEOUT "timeout: %u\n"
#define ARI_SEED "seed: %u\n"
#define ARI_VIEW "view: %s\n"
#define ARI_NUM_PLAYERS "num_players: %u\n"
#define ARI_PLAYER_NAME_STRING_FORMAT "  %s\n"

#define ARI_PIPE "pipe"
#define ARI_EXECVE "execve"
#define ARI_FORK "fork"
#define ARI_SETUID "setuid"
#define ARI_WAITPID "waitpid"
#define ARI_WAIT "wait"

#define ARI_SNPRINTF "%d"

typedef struct {
    int delay;
    unsigned int seed;
    int timeout;
    char* view;
    char* playerPaths[MAX_PLAYERS];
    gameState_t* state;
    gameSync_t* sync;
    int stateFd;
    int syncFd;
} gameConfig_t;

typedef struct {
    int read;
    int write;
} pipefd_t;

typedef struct {
    unsigned int x;
    unsigned int y;
} coords_t;

void validateArgs(gameConfig_t* gameConfig, int playerCount, int width, int height){
    if (playerCount < MIN_PLAYERS){
        fprintf(stderr,ARI_NO_PLAYER);
        exit(EXIT_FAILURE);
    }

    if (width < DEFAULT_WIDTH || height < DEFAULT_HEIGHT){
        fprintf(stderr, ARI_WRONG_BOARD_DIMENSIONS, DEFAULT_WIDTH, DEFAULT_HEIGHT, width, height);
        exit(EXIT_FAILURE);
    }

    return;
}

void createPlayer(char* name, player_t* player){
    //memset(dest,0,sizeof(dest[0])*MAX_PLAYERS); The chomp champs original implementation uses memset to initialize players to 0. But it is not neccesary since strncpy guarantees to write 'count' characters, filling with 0 if source is shorter than len.
    strncpy(player->name, basename(name), MAX_PLAYER_NAME_LEN);
    player->name[MAX_PLAYER_NAME_LEN-1] = 0; //strncpy does not guarantee a null terminated string
    player->score = 0;
    player->invalidMovementRequestsCount = 0;
    player->validMovementRequestsCount = 0;
    player->isBlocked = 0;
}

void initializeGameSync(gameSync_t* gameSync){
  semInit(&(gameSync->printNeeded), 0);
  semInit(&(gameSync->printDone), 0);
  semInit(&(gameSync->masterWantsToReadMutex), 1);
  semInit(&(gameSync->readGameStateMutex), 1);
  semInit(&(gameSync->readersCountMutex), 1);
  gameSync->readersCount = 0;
}


void initializeRandomBoard(gameState_t* gameState, unsigned int seed) {
  srand(seed);
  for(unsigned int i = 0; i < gameState->height * gameState->width; i++){
    gameState->board[i] = (rand() % MAX_BOARD_VALUE) + MIN_BOARD_VALUE;
  }
}

void copyPlayers(player_t dest[MAX_PLAYERS],player_t org[MAX_PLAYERS]){
    for(int i = 0; i<MAX_PLAYERS; i++){
        dest[i] = org[i]; //OBS: inefficient, it would be ideal to directly write the players in the shared memory.
    }
}

void configureGame(int argc, char* argv[], gameConfig_t* gameConfig){
    int opt;
    int playerCount = 0;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    char isDelayDeclared = 0;
    player_t playerList[MAX_PLAYERS] = {0};

    while ((opt = getopt(argc, argv, ARI_OPTARG_MOD)) != -1) {
        switch (opt) {
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'd':
                gameConfig->delay = atoi(optarg);
                if (gameConfig->delay<0) {
                    gameConfig->delay=0;
                }
                isDelayDeclared = 1;
                break;
            case 't':
                gameConfig->timeout = atoi(optarg);
                if (gameConfig->timeout<0) {
                    gameConfig->timeout=0;
                }
                break;
            case 's':
                gameConfig->seed = atoi(optarg);
                break;
            case 'v':
                gameConfig->view = optarg;
                break;
            case 'p':
                while (optind - 1 < argc && argv[optind - 1][0] != '-') {
                    if (playerCount < MAX_PLAYERS) {
                        createPlayer(argv[optind - 1], &playerList[playerCount]);
                        
                        gameConfig->playerPaths[playerCount] = argv[optind - 1];
                        playerCount++;
                    }
                    else {
                        fprintf(stderr,ARI_TOO_MANY_PLAYERS, MAX_PLAYERS);
                        exit(EXIT_FAILURE);
                    }
                    optind++;
                }
                optind--; // Fix index for getopt
                break;
            default:
                fprintf(stderr, ARI_WRONG_USAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    validateArgs(gameConfig, playerCount, width, height);

    if (isDelayDeclared && gameConfig->view == NULL){
      fprintf(stderr, ARI_NO_VIEW); 
    }

    gameConfig->sync = createShm(GAME_SYNC, //BUG the player crashes on shm_open
        sizeof(*(gameConfig->sync)),
        1,
        &gameConfig->syncFd
    );

    gameConfig->state = createShm(GAME_STATE,
        sizeof(*(gameConfig->state))+sizeof(gameConfig->state->board[0])*width*height,
        0,
        &gameConfig->stateFd
    );

    gameConfig->state->width       = width;
    gameConfig->state->height      = height;
    gameConfig->state->playerCount = playerCount;
    gameConfig->state->isOver      = 0;

    copyPlayers(gameConfig->state->playerList, playerList);

    initializeRandomBoard(gameConfig->state,gameConfig->seed);
    initializeGameSync(gameConfig->sync);
}

void printArgs(gameConfig_t* gameConfig){
    printf(ARI_WIDTH, gameConfig->state->width);
    printf(ARI_HEIGHT, gameConfig->state->height);
    printf(ARI_DELAY, (unsigned int)gameConfig->delay);
    printf(ARI_TIMEOUT, (unsigned int)gameConfig->timeout);
    printf(ARI_SEED, gameConfig->seed);
    printf(ARI_VIEW, gameConfig->view ? gameConfig->view : "-");
    printf(ARI_NUM_PLAYERS, gameConfig->state->playerCount);
    for (int i = 0; i < gameConfig->state->playerCount; i++) {
        printf(ARI_PLAYER_NAME_STRING_FORMAT, gameConfig->playerPaths[i]);
    }
    sleep(2);
}

static inline double calculatePosition(double size, char isX, unsigned int i, unsigned int playerCount){
    if (playerCount == 1){
        return size/2;
    }
    double angle = (i * M_PI * 2) / playerCount;
    return (isX ? cos(angle) : sin(angle)) * (size / 3) + size / 2 + 0.5;
}

void spawnPlayers(gameState_t* gameState){
  for (unsigned int i = 0; i < gameState->playerCount; i++) {
      gameState->playerList[i].x = calculatePosition(gameState->width, 1, i, gameState->playerCount);
      gameState->playerList[i].y = calculatePosition(gameState->height, 0, i, gameState->playerCount);
      gameState->board[gameState->width * gameState->playerList[i].y + gameState->playerList[i].x] = -i;
    }
}

void execveWithArgs(char* process, int width, unsigned int decimalLenWidth, int height, unsigned int decimalLenHeight){
  char arg1[decimalLenWidth+1];
  char arg2[decimalLenHeight+1];
  char* argv[] = {process, arg1, arg2,(char*)0};

  snprintf(arg1,decimalLenWidth+1,ARI_SNPRINTF,width); //This may be a little overkill, another aproach would be to use itoa. But ChompChamps original uses snprintf.
  snprintf(arg2,decimalLenHeight+1,ARI_SNPRINTF,height);
  char* envp[] = {(char*)0};
  if (execve(process,argv,envp) == -1) { // TODO
    errExit(ARI_EXECVE);
  }
}

pid_t forkToView(char* view, unsigned int width, unsigned int height) {
  pid_t pid = fork();
  if (pid == -1) {
    errExit(ARI_FORK);
  }
  if (pid == 0) {
    if (setuid(1000) == -1) {
      errExit(ARI_SETUID);
    }
    execveWithArgs(view, width, decimalLen(width), height, decimalLen(height));
  }
  return pid;
}

void createPipes(unsigned int playerCount, pipefd_t* pipefd) {
  for(unsigned int i = 0; i<playerCount; i++){
    if(pipe((int*)&(pipefd[i])) == -1){
        errExit(ARI_PIPE);
    }
  }
}

void closeForeignPipes(unsigned int playerId, unsigned int playerCount, pipefd_t* pipefd) {
  for (unsigned int i = 0; i < playerCount; i++) {
    if (playerId != i) {
      closeFd(pipefd[i].read); 
      closeFd(pipefd[i].write); 
    }
  }
}

void spawnPlayerProcesses(gameState_t* gameState, char** playerPaths, pipefd_t* pipefd){
  for(unsigned int i = 0; i < gameState->playerCount; i++){
    pid_t pid = fork();
    if (pid == -1){
        errExit(ARI_FORK);
    }
    if (pid == 0) {
      if (setuid(1000) == -1) {
        errExit(ARI_SETUID);
      }
      closeForeignPipes(i, gameState->playerCount, pipefd);
      closeFd(pipefd[i].read);
      dup2(pipefd[i].write,1); //TODO err exit
      closeFd(pipefd[i].write); //We validate this, even though ChompChamps (6b398ab0f1be541975002579f26f509f) doesn't
      execveWithArgs(playerPaths[i], gameState->width, decimalLen(gameState->width), gameState->height, decimalLen(gameState->height));
    }
    else {
      gameState->playerList[i].pid = pid;
    }
  }
}

void closeWritePipes(unsigned int playerCount,pipefd_t* pipefd) {
  for (unsigned int i = 0; i < playerCount; i++) {
    closeFd(pipefd[i].write); 
  }
}

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

int getNumberOfReadyPlayers(int timeout, unsigned int playerCount, struct pollfd* pollFdArr, time_t *timeStart) { //TODO es probable que se pueda simplificar. Ademas, solo lee un pipe, deberia tener un nombre generico no particular de player
  int ret;
  int timeUntilTimeout = (timeout - difftime(time(NULL), *timeStart)) * 1000;
  if (timeUntilTimeout < 1) {
    ret = 0;
  }
  else {
    ret = poll(pollFdArr, playerCount, timeUntilTimeout);
    if (ret == -1) {
      errExit("poll");
    }
  }
  return ret;
}

int readPlayer(int fd,char *directionPtr) { //TODO es probable que se pueda simplificar. Ademas, solo lee un pipe, deberia tener un nombre generico no particular de player
  int numberOfReadedChars;
  numberOfReadedChars = read(fd,directionPtr,1);
  if (numberOfReadedChars == -1) {
    perror("read");
    exit(1);
  }
  if (numberOfReadedChars == 0) {
    return -1;
  }
  return 0;
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
                if(!(pollFdArr[*nextPlayerIndex].revents & POLLIN) || readPlayer(pollFdArr[*nextPlayerIndex].fd,directionPtr) == -1){ //TODO this is the wierdest if ever, it abuses of "||" lazyness to make function calls
                    killPlayer(*nextPlayerIndex, gameState, gameSync,pollFdArr);
                    return -1;
                }
                return 0;
            }
            *nextPlayerIndex=((*nextPlayerIndex)+1)%gameState->playerCount;
        }
        numberOfReadyPlayers = getNumberOfReadyPlayers(timeout, gameState->playerCount, pollFdArr, &timeStart);
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

void setUpPollFdArr(unsigned int playerCount, pipefd_t* pipefd, struct pollfd* pollFdArr){
  for(unsigned int i = 0; i<playerCount; i++){
      pollFdArr[i] = (struct pollfd){
          .fd = pipefd[i].read,
          .events = POLLIN,
          .revents = 0 //Important because we read this value before calling poll
      };
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

void waitForView(pid_t view) {
  int statLoc;
  if (waitpid(view, &statLoc, 0) == -1) {
    errExit(ARI_WAITPID);
  }
  printf(ARI_VIEW_EXIT, statLoc); //OBS Original chomp champs does not use WEXITSTATUS -- Asked by email and was mentioned it wasn't necessary
  return;
}

static inline unsigned int findPlayer(player_t* playerList, unsigned int playerCount, pid_t pid){
    unsigned int i;
    for(i=0; i<playerCount; i++){
        if (playerList[i].pid == pid){
            break;
        }
    }
    return i;
}

void waitAllPlayers(player_t* playerList, unsigned int playerCount){
  for(unsigned int i = 0; i<playerCount; i++){
    int statLoc;
    pid_t pid = wait(&statLoc);
    if (pid == -1) {
      errExit(ARI_WAIT);
    }
    unsigned int playerNumber = findPlayer(playerList, playerCount, pid);
    printf(ARI_PLAYER_EXIT,
        playerList[playerNumber].name,
        playerNumber,
        WEXITSTATUS(statLoc),
        playerList[playerNumber].score,
        playerList[playerNumber].validMovementRequestsCount,
        playerList[playerNumber].invalidMovementRequestsCount
    );
  }
}

void unInitializeGameSync(gameSync_t* gameSync) {
  semDestroy(&(gameSync->printNeeded));
  semDestroy(&(gameSync->printDone));
  semDestroy(&(gameSync->masterWantsToReadMutex));
  semDestroy(&(gameSync->readGameStateMutex));
  semDestroy(&(gameSync->readersCountMutex));
}

void unconfigureGame(gameConfig_t* gameConfig){
    unInitializeGameSync(gameConfig->sync);
    closeMem(GAME_STATE,
        sizeof(*(gameConfig->state)) + sizeof(gameConfig->state->board[0]) * gameConfig->state->width * gameConfig->state->height,
        gameConfig->state,
        &gameConfig->stateFd
    );
    closeMem(GAME_SYNC,
        sizeof(*(gameConfig->sync)),
        gameConfig->sync,
        &gameConfig->syncFd
    );
}

int main(int argc, char* argv[]){
    // printf("%d\n\n",sizeof(gameConfig_t));
    // char* temp[sizeof(gameState_t) + (width * height)*sizeof(int)];
    // gameConfig_t gameConfig = (gameState_t*)((void*)temp);

    gameConfig_t gameConfig = {
        .delay = DEFAULT_DELAY,
        .seed = DEFAULT_SEED,
        .timeout = DEFAULT_TIMEOUT,
        .view = NULL,
        .playerPaths = {0}
    };
    
    system(ARI_CLEAR); //I dont like this, I would use CSI 2 J, But the profesor's bynary uses system()

    configureGame(argc, argv, &gameConfig);
    
    printArgs(&gameConfig);

    spawnPlayers(gameConfig.state);

    pid_t viewPid = 0;
    if (gameConfig.view){
        viewPid = forkToView(gameConfig.view, gameConfig.state->width, gameConfig.state->height);
    }

    pipefd_t pipefd[MAX_PLAYERS]; //TODO quizas no es necesario guardar el read end aca, y se puede guardar directo en poll fd arr (aunque quizas seria mala modularizacion? ( TODO discutir))

    createPipes(gameConfig.state->playerCount, pipefd);



    spawnPlayerProcesses(gameConfig.state, gameConfig.playerPaths, pipefd);

    closeWritePipes(gameConfig.state->playerCount, pipefd); //I dont know why original chomp champs separates this and does not make it inside spawn players.

    game(&gameConfig, pipefd);

    if (viewPid > 0){
        waitForView(viewPid);
    }

    waitAllPlayers(gameConfig.state->playerList, gameConfig.state->playerCount);

    unconfigureGame(&gameConfig);

    return 0;
}