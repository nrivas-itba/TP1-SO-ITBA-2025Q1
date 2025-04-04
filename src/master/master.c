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


#define DEFAULT_WIDTH 10
#define DEFAULT_HEIGHT 10
#define DEFAULT_DELAY 200
#define DEFAULT_TIMEOUT 10
#define DEFAULT_SEED time(NULL)

#define MIN_PLAYERS 1

#define ARI_VIEW_EXIT "View exited (%d)\n" //TODO Sin uso
#define ARI_PLAYER_EXIT "Player %s (%u) exited (%d) with a score of %u / %u / %u\n" //TODO Sin uso

#define ARI_OPTARG "w:h:d:p:v:s:t:"
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

typedef struct {
    int delay;
    unsigned int seed; //TODO este dato viene de atoi, q retorna int. Pero lo usa srand, q toma unsigned int :(
    int timeout;
    char* view;
    char* playerPaths[9];
    gameState_t* state;
    gameSync_t* sync;
    size_t gameStateSize;
    int stateFd;
    int syncFd;
} gameConfig_t;

void validateArgs(gameConfig_t* gameConfig, int playerCount, int width, int height){
    if (playerCount < MIN_PLAYERS){
        fprintf(stderr,ARI_NO_PLAYER);
        exit(EXIT_FAILURE);
    }

    if (width < DEFAULT_WIDTH || height < DEFAULT_HEIGHT){
        fprintf(stderr, ARI_WRONG_BOARD_DIMENSIONS, DEFAULT_WIDTH, DEFAULT_HEIGHT, width, height);
        exit(EXIT_FAILURE);
    }

    if (gameConfig->view == NULL){
        fprintf(stderr, ARI_NO_VIEW);
    }

    return;
}

void createPlayer(char* name, player_t* player){
    //memset(dest,0,sizeof(dest[0])*MAX_PLAYERS); The chomp champs original implementation uses memset to initialize players to 0. But it is not neccesary since strncpy guarantees to write "count" characters, filling with 0 if source is shorter than len.
    strncpy(player->name, __xpg_basename(name), MAX_PLAYER_NAME_LEN);
    player->name[MAX_PLAYER_NAME_LEN-1] = 0; //strncpy does not guarantee a null terminated string
    player->score = 0;
    player->invalidMovementRequestsCount = 0;
    player->validMovementRequestsCount = 0;
    player->canMove = 1;
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
    gameState->board[i] = (rand() % 9) + 1;
  }
}

void copyPlayers(player_t dest[MAX_PLAYERS],player_t org[MAX_PLAYERS]){
    for(int i = 0; i<MAX_PLAYERS; i++){
        dest[i] = org[i]; //TODO inefficient, it would be ideal to directly write the players in the shared memory.
    }
}

void configureGame(int argc, char* argv[], gameConfig_t* gameConfig){

    int opt;
    int playerCount = 0;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    player_t playerList[MAX_PLAYERS] = {0};

    while ((opt = getopt(argc, argv, ARI_OPTARG)) != -1) {
        switch (opt) {
            case 'w':
                gameConfig->state->width = atoi(optarg);
                break;
            case 'h':
                gameConfig->state->height = atoi(optarg);
                break;
            case 'd':
                gameConfig->delay = atoi(optarg);
                if (gameConfig->delay<0) {
                    gameConfig->delay=0;
                }
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

    gameConfig->gameStateSize=sizeof(gameConfig->state)+sizeof(int)*width*height;
    gameConfig->state = createShm(GAME_STATE, gameConfig->gameStateSize, 1, &gameConfig->stateFd);

    gameConfig->sync = createShm(GAME_SYNC, sizeof(gameConfig->sync), 0, &gameConfig->syncFd);

    gameConfig->state->width       = width;
    gameConfig->state->height      = height;
    gameConfig->state->playerCount = playerCount;
    gameConfig->state->isOver      = 0;

    copyPlayers(gameConfig->state->playerList, playerList);

    initializeRandomBoard(gameConfig->state,gameConfig->seed);

    return;
}

void printArgs(gameConfig_t* gameConfig){
    printf(ARI_WIDTH, gameConfig->state->width);
    printf(ARI_HEIGHT, gameConfig->state->height);
    printf(ARI_DELAY, gameConfig->delay);
    printf(ARI_TIMEOUT, gameConfig->timeout);
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
  char arg1[decimalLenWidth];
  char arg2[decimalLenHeight];
  char* argv[] = {process, arg1, arg2,(char*)0};
  
  snprintf(arg1,decimalLenWidth,"%d",width);
  snprintf(arg2,decimalLenHeight,"%d",height);
  char* envp[] = {(char*)0};
  if (execve(process,argv,envp) == -1) {
    errExit("execve");
  }
}

pid_t forkToView(char* view, unsigned int width, unsigned int height) {
  pid_t pid = fork();
  if (pid == -1) {
    errExit("fork");
  }
  if (pid == 0) {
    if (setuid(1000) == -1) {
      errExit("setuid");
    }
    execveWithArgs(view, width, decimalLen(width), height, decimalLen(height));
  }
  return pid;
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
        .playerPaths = {0},
    };
    
    system(ARI_CLEAR); //I dont like this, I would use CSI 2 J, But the profesor's bynary uses system()

    configureGame(argc, argv, &gameConfig);
    
    printArgs(&gameConfig);

    spawnPlayers(gameConfig.state);

    if (gameConfig.view){
        forkToView(gameConfig.view, gameConfig.state->width, gameConfig.state->height);
    }

    return 0;
}