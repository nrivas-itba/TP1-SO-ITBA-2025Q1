#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/utils.h"

int main(int argc, char* argv[]){
    game_t game = openGame(argc, argv);

    FILE *fp = fopen("/dev/random", "rb");
    if (fp == NULL) {
        errExit("Error opening /dev/random");
    }

    unsigned char random_byte;
    while(!game.state->isOver) { // Read and print 10 random bytes //TODO we should not read this var while we dont have read permissions
        fread(&random_byte, 1, 1, fp);
        printf("%c", random_byte%8); // Print each byte as hexadecimal
        fprintf(stderr, "Player: %d\n", random_byte%8); // Print each byte as hexadecimal
    }

    printf("\n");
    fclose(fp);
    return EXIT_SUCCESS;
}
