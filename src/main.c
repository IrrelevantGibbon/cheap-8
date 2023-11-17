#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "cheap_8.h"

 int main(int argc, char *argv[])
{
    srand( time( NULL ) );
    if (argc < 2 || argc > 2) {
        printf("Usage: %s <rom>\n", argv[0]);
        return 1;
    }
    init_cheap_8(argv[1]);
    return 0;
}
