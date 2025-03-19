#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = fopen("/dev/random", "rb");
    if (fp == NULL) {
        perror("Error opening /dev/random");
        return EXIT_FAILURE;
    }

    unsigned char random_byte;
    for(int i = 0; i<3;i++) { // Read and print 10 random bytes
        fread(&random_byte, 1, 1, fp);
        printf("%c", random_byte%8); // Print each byte as hexadecimal
    }

    printf("\n");
    fclose(fp);
    return EXIT_SUCCESS;
}
