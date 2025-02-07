#include "../include/tlpi_hdr.h"

static int idata = 111;

int main(int argc, char const *argv[])
{
    int inStack = 222;

    pid_t childPID;

    switch ((childPID = fork())) 
    {
        case -1:
            errExit("main()->fork()");

        case 0:
            idata     *= 2;
            inStack   *= 2;

            break;

        default:
            sleep(3);
            break;
    }

    printf(
        "PID = %ld %s, idata = %d, inStack = %d.\n",
        (long)getpid(), (childPID == 0) ? "(child)" : "(parent)",
        idata, inStack
    );

    return EXIT_SUCCESS;
}
