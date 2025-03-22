#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "../utils/utils.h"

player_t* getMe(game_t* game){
    pid_t pid = getpid();
    for(int i =0; i<MAX_PLAYERS; i++){
        if(game->state->playerList[i].pid == pid){
            return &(game->state->playerList[i]);
        }
    }
    errExit("I dont exist in the board");
}

char shouldITryToMove(player_t* me){
    static int invalidMovementRequestsCount = -1;
    static int validMovementRequestsCount = -1;
    if (!me->notHasAnyValidMovements && (me->invalidMovementRequestsCount != invalidMovementRequestsCount || me->validMovementRequestsCount != validMovementRequestsCount)){
        invalidMovementRequestsCount = me->invalidMovementRequestsCount;
        validMovementRequestsCount = me->validMovementRequestsCount;
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv);
    player_t* me = getMe(&game);

    FILE *fp = fopen("/dev/random", "rb");
    if (fp == NULL) {
        errExit("Error opening /dev/random");
    }


    unsigned char random_byte;
    while(!game.state->isOver) { // Read and print 10 random bytes //TODO we should not read this var while we dont have read permissions
        
        /////////////Readers entrance
        sWait(&(game.sync->C));
        sWait(&(game.sync->readersCountMutex));
        game.sync->readersCount++;
        if(game.sync->readersCount == 1){
            sWait(&(game.sync->readGameStateMutex)); //First reader locks writes
        }
        
        sPost(&(game.sync->C));
        sPost(&(game.sync->readersCountMutex));
        /////////////Readers entrance

        /////////////Read
        if(shouldITryToMove(me)){
            fread(&random_byte, 1, 1, fp);
            printf("%c", random_byte%8); // Print each byte as hexadecimal
            fflush(stdout);
        }

        /////////////Readers exit
        sWait(&(game.sync->readersCountMutex));
        game.sync->readersCount--;
        if(game.sync->readersCount == 0){
            sPost(&(game.sync->readGameStateMutex)); //Last readers unlocks writes
        }
        sPost(&(game.sync->readersCountMutex));
        /////////////Readers exit
    }

    printf("\n");
    fclose(fp);
    return EXIT_SUCCESS;
}
