// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "../utils/utils.h"
#include <string.h> //for mem cpy

typedef struct {
    FILE* file;
} connection_t;

unsigned int getMe(game_t* game){
    pid_t pid = getpid();
    for(int i =0; i<MAX_PLAYERS; i++){
        if(game->state->playerList[i].pid == pid){
            return i;
        }
    }
    errExit("I dont exist in the board");
}

char shouldITryToMove(player_t* me){
    static unsigned int invalidMovementRequestsCount = -1;
    static unsigned int validMovementRequestsCount = -1;
    if (!me->isBlocked  && (me->invalidMovementRequestsCount != invalidMovementRequestsCount || me->validMovementRequestsCount != validMovementRequestsCount)){
        invalidMovementRequestsCount = me->invalidMovementRequestsCount;
        validMovementRequestsCount = me->validMovementRequestsCount;
        return 1;
    }
    return 0;
}

void waitToRead(gameSync_t* gameSync){
    sWait(&(gameSync->masterWantsToReadMutex));
    sPost(&(gameSync->masterWantsToReadMutex)); //In the 31/3/2025 we saw that it should have this order

    sWait(&(gameSync->readersCountMutex));
    gameSync->readersCount++;
    if(gameSync->readersCount == 1){
        sWait(&(gameSync->readGameStateMutex)); //First reader locks writes
    }
    sPost(&(gameSync->readersCountMutex));
}

void finishReading(gameSync_t* gameSync){
    sWait(&(gameSync->readersCountMutex));
    gameSync->readersCount--;
    if(gameSync->readersCount == 0){
        sPost(&(gameSync->readGameStateMutex)); //Last readers unlocks writes
    }
    sPost(&(gameSync->readersCountMutex));
}

char getNextMove(connection_t* connection){
    unsigned char random_byte;
    if (fread(&random_byte, 1, 1, connection->file) == 0){
        errExit("fread");
    }
    return random_byte;
}

void playingLoop(game_t* game, int gameWidth, int gameHeight, unsigned int playerCount, unsigned int me, connection_t* connection){
    player_t playerList[playerCount];
    int board[gameWidth][gameHeight];
    char isGameOver = 0;

    while(!isGameOver) {
        memcpy(playerList, game->state->playerList, sizeof(playerList[0])*playerCount);
        memcpy(board,      game->state->board,      sizeof(board[0][0])*gameWidth*gameHeight);
        isGameOver = game->state->isOver;
        finishReading(game->sync);

        if(shouldITryToMove(&(playerList[me]))){
            char nextMove = getNextMove(connection)%9;
            if(nextMove!=8){
                printf("%c", nextMove); // Print a random value
                fflush(stdout);
            }
            else{
                shouldITryToMove(&((player_t){
                    .validMovementRequestsCount=-1
                }));
            }
        }

        if(!isGameOver){
            waitToRead(game->sync);
        }
        else{
            break;
        }
    }
}

connection_t openConnection(){
    connection_t ret = (connection_t){
        .file = fopen("/dev/random", "rb")
    };
    if (ret.file == NULL) {
        errExit("Error opening /dev/random");
    }
    return ret;
}

void closeConnection(connection_t* connection){
    fclose(connection->file);
}

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv); //TODO meter todas estas inicializaciones en un inicializador de view
    unsigned int me = getMe(&game);
    connection_t connection = openConnection();
    
    waitToRead(game.sync);
    playingLoop(&game, game.gameWidth, game.gameHeight, game.state->playerCount, me, &connection);
    
    closeConnection(&connection);
    return 0;
}
