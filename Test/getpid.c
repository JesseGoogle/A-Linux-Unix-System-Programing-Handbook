#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t myPID = getpid();

    printf("This process ID = %ld\n", (long int)myPID);

    return 0;
}
