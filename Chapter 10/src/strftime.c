#include <sys/unistd.h>

#include "../include/timeFuncAssistant.h"

int main(int argc, char const *argv[])
{
    system("clear");

    printf("%s\n", getCurrentTime("%F %T"));
    
    return EXIT_SUCCESS;
}
