#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "../utils/utils.h"

#define DEFAULT_WIDTH 10
#define DEFAULT_HEIGHT 10
#define DEFAULT_DELAY 200
#define DEFAULT_TIMEOUT 10
#define DEFAULT_SEED time(NULL)

#define MIN_PLAYERS 1
#define MAX_PLAYERS 9

#define ARI_VIEW_EXIT "View exited (%d)\n"

#define ARI_PLAYER_EXIT "Player %s (%u) exited (%d) with a score of %u / %u / %u\n"

#define ARI_OPTARG "w:h:d:p:v:s:t:"

#define ARI_WRONG_USAGE "Usage: %s [-w width] [-h height] [-d delay] [-s seed] [-v view] [-t timeout] -p player1 player2 ...\n"

#define ARI_NO_VIEW "Info: Delay parameter ignored since there is no view.\n"

#define ARI_NO_PLAYER "Error: At least one player must be specified using -p.\n"

#define ARI_TOO_MANY_PLAYERS "Error: At most %d players can be specified using -p.\n"

#define ARI_SHORT_OF_PLAYERS "Error: Minimal board dimensions: %dx%d. Given %dx%d\n"



#define ARI_WIDTH "width: %d\n"
#define ARI_HEIGHT "height: %d\n"
#define ARI_DELAY "delay: %u\n"
#define ARI_TIMEOUT "timeout: %u\n"
#define ARI_SEED "seed: %u\n"
#define ARI_VIEW "view: %s\n"
#define ARI_NUM_PLAYERS "num_players: %u\n"
#define ARI_PLAYER_NAME_STRING_FORMAT "  %s\n"

int main(int argc, char* argv[]){
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int delay = DEFAULT_DELAY;
    int timeout = DEFAULT_TIMEOUT;
    int seed = DEFAULT_SEED;
    char *view = NULL;
    char *players[9];
    gameState_t gameState;
    int playerCount = 0;

    int opt;
    while ((opt = getopt(argc, argv, ARI_OPTARG)) != -1) {
        switch (opt) {
            case 'w':
                width = atoi(optarg);
                if (width < DEFAULT_WIDTH) width = 10;
                break;
            case 'h':
                height = atoi(optarg);
                if (height < DEFAULT_HEIGHT) height = 10;
                break;
            case 'd':
                delay = atoi(optarg);
                if (delay<0) delay = 0;
                break;
            case 't':
                timeout = atoi(optarg);
                if (timeout<0) timeout = 0;
                break;
            case 's':
                seed = atoi(optarg);
                break;
            case 'v':
                view = optarg;
                break;
            case 'p':
                while (optind - 1 < argc && argv[optind - 1][0] != '-') {
                    if (playerCount < MAX_PLAYERS) 
                        players[playerCount++] = argv[optind - 1];
                        
                    else {
                        fprintf(stderr,ARI_TOO_MANY_PLAYERS, MAX_PLAYERS);
                        exit(EXIT_FAILURE);
                    }
                    optind++;
                }
                optind--; // Ajustar índice para getopt
                break;
            default:
                fprintf(stderr, ARI_WRONG_USAGE, argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    if (playerCount<MIN_PLAYERS){
        fprintf(stderr,ARI_NO_PLAYER);
        exit(EXIT_FAILURE);
    }
        

    // if(???){
    //     fwrite(ARI_NO_VIEW,1,0x36,stderr);
    // }

    // fprintf(stderr,"Error: Minimal board dimensions: %dx%d. Given %dx%d\n",10,10,(ulong)*param_1,
    //         (ulong)param_1[1]);
                        // WARNING: Subroutine does not return
    // exit(1);

    printf("Configuración:\n");
    printf("  Ancho: %d\n", width);
    printf("  Alto: %d\n", height);
    printf("  Delay: %d ms\n", delay);
    printf("  Timeout: %d s\n", timeout);
    printf("  Semilla: %d\n", seed);
    printf("  Vista: %s\n", view ? view : "Sin vista");
    printf("  Jugadores:\n");
    for (int i = 0; i < playerCount; i++) {
        printf("    - %s\n", players[i]);
    }

    return 0;

}