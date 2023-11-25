#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "cheap_8.h"

 int main(int argc, char *argv[])
{
    srand( time( NULL ) );
    if (argc < 3 || argc > 3) {
        printf("Usage: <chip_type> %s <rom>\n>", argv[0]);
        return 1;
    }
    init_cheap_8(atoi(argv[1]), argv[2]);
    return 0;
}
