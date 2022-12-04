#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef unsigned char byte;

byte ROR(byte value, int amount);

int main(int argc, char** argv) {
    struct stat s;
    if (argc != 2 || stat(argv[1], &s) == -1) {
        return -1;
    }

    int size = s.st_size;
    int fd = open(argv[1], O_RDONLY);

    if (size <= 4) {
        return -1;
    }

    int seed;
    read(fd, &seed, sizeof(int));

    size -= sizeof(int);

    byte* buff = malloc(size + 1);
    read(fd, buff, size);

    srand(seed);

    for (int i = 0; i < size; i++) {
        // crypt function:
        // ROL( (byte ^ rand()), rand()&7 )

        byte rand1 = (byte)rand();
        byte rand2 = (byte)rand() & 7;

        buff[i] = ROR(buff[i], rand2);
        buff[i] = buff[i] ^ rand1;
    }

    buff[size] = '\0';

    printf("%s\n", buff);

    return 0;
}

byte ROR(byte value, int amount) {
    // https://stackoverflow.com/questions/33425383/rotate-bits-8-bits-in-c-with-arduino
    // gcc will generate ror instruction with optimizations enabled
    return (value >> amount) | (value << (8 - amount));
}
