#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "../utils/utils.h"

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
    int seed;
    int timeout;
    char* view;
    char* playerPaths[9];
    gameState_t* gameState;
    size_t gameStateSize;
    int gameStateFd;
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
    strncpy(player->name, name, MAX_PLAYER_NAME_LEN);
    player->score = 0;
    player->invalidMovementRequestsCount = 0;
    player->validMovementRequestsCount = 0;
    player->canMove = 1;
    return;
}

void processArgs(int argc, char* argv[], gameConfig_t* gameConfig){

    int opt;
    int playerCount = 0;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    player_t playerList[MAX_PLAYERS] = {0};

    while ((opt = getopt(argc, argv, ARI_OPTARG)) != -1) {
        switch (opt) {
            case 'w':
                gameConfig->gameState->width = atoi(optarg);
                break;
            case 'h':
                gameConfig->gameState->height = atoi(optarg);
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

    gameConfig->gameStateSize=sizeof(gameState_t)+sizeof(int)*width*height;
    gameConfig->gameState = createShm(GAME_STATE, gameConfig->gameStateSize, 1, &gameConfig->gameStateFd);

    return;
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
    
    processArgs(argc, argv, &gameConfig);

    printf("Configuración:\n");
    printf("  Ancho: %d\n", gameConfig.gameState->width);
    printf("  Alto: %d\n", gameConfig.gameState->height);
    printf("  Delay: %d ms\n", gameConfig.delay);
    printf("  Timeout: %d s\n", gameConfig.timeout);
    printf("  Semilla: %d\n", gameConfig.seed);
    printf("  Vista: %s\n", gameConfig.view ? gameConfig.view : "Sin vista");
    printf("  Jugadores:\n");
    for (int i = 0; i < gameConfig.gameState->playerCount; i++) {
        printf("    - %s\n", gameConfig.playerPaths[i]);
    }

    return 0;

}