#include "../include/timeFuncAssistant.h"
#include <limits.h>

int main(int argc, char const *argv[])
{
    system("clear");

    char str[1000000] = {0};
    displayProcessTime("At program start: ");

    write(STDOUT_FILENO, str, 1000000);
    write(STDOUT_FILENO, str, 1000000);
    write(STDOUT_FILENO, str, 1000000);
    write(STDOUT_FILENO, str, 1000000);
    write(STDOUT_FILENO, str, 1000000);

    displayProcessTime("At program end: ");

    return EXIT_SUCCESS;
}
