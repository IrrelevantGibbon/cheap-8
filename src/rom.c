#include <stdio.h>

u_int8_t loadRom(char *file_name, u_int8_t *memory)
{
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fread(memory + 0x200, size, 1, file);

    return 0;
}