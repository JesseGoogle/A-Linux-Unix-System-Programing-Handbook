#include <stdio.h>
#include <stdlib.h>

#include "../include/getNum.h"


int main(int argc, char const *argv[])
{
    system("clear");

    printf("0xFF = %ld\n", getLong("0xFF", GN_BASE_16, NULL));
    printf("0200 = %d\n", getInt("0200", GN_BASE_8, NULL));

    printf("0x114514 = %d\n", getInt("0x114514", 10, NULL));

    printf("abcdefg = %d\n", getInt("abcdefg", GN_ANY_BASE, NULL));

    getInt(NULL, GN_ANY_BASE, NULL);
    
    return 0;
}
