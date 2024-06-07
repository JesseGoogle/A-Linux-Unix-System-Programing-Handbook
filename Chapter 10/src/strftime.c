#define _XOPEN_SOURCE

#include <sys/unistd.h>

#include "../include/timeFuncAssistant.h"

#include <locale.h>

#define TS_BUFFER_SIZE 1024

char * getProgramName(char * __programPath);

int main(int argc, char * argv[])
{
    system("clear");

    struct tm timeMessage = {0};
    char timeStringBuff[TS_BUFFER_SIZE] = {0};
    char * outputFormat = NULL;


    if (argc < 3 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("[%s] <input-date-time> <in-format> [out-format]\n", getProgramName(argv[0]));
    }

    /**
     * 设置本地化环境, LC_ALL 宏意味着影响所有本地化类别
    */
    if (!setlocale(LC_ALL, "")) { errExit("setlocale()"); }

    if (!strptime(argv[1], argv[2], &timeMessage)) { fatal("strptime()"); }

    timeMessage.tm_isdst = -1;

    printf("Current time stamp (seconds since Epoch) = %ld\n", (long int)mktime(&timeMessage));

    outputFormat = (argc > 3) ? argv[3] : "%H:%M:%S:%A, %d %B %Y %Z";

    if (!strftime(timeStringBuff, TS_BUFFER_SIZE, outputFormat, &timeMessage))
    {
        fatal("strftime() return 0.\n");
    }

    printf("strftime() yields: %s\n", timeStringBuff);
    
    return EXIT_SUCCESS;
}

char * getProgramName(char * __programPath)
{
    if (!__programPath || !(*__programPath)) 
    { 
        fatal("getProgramName(): NULL or empty programPath.\n"); 
    }

    size_t pathLen = strlen(__programPath);
    char * pathEnd = __programPath + pathLen;

    while ((*pathEnd) != '/') { --pathEnd; }

    return (pathEnd + 1);
}