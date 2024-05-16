#include <gnu/libc-version.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("GLibC Version = %s\n", gnu_get_libc_version());
    return 0;
}
