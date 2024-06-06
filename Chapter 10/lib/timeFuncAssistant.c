#include "../include/timeFuncAssistant.h"

void showtmStruct(const struct tm * __timeInfo)
{
    if (!__timeInfo) { errExit("showtmStruct(__timeInfo): NULL argument __timeInfo."); }

    printf(
            "[%d.%d.%d %d:%d:%d] %s\n",
            getYears(__timeInfo->tm_year), __timeInfo->tm_mon, __timeInfo->tm_mday,
            __timeInfo->tm_hour, __timeInfo->tm_min, __timeInfo->tm_sec,
            Weeks[__timeInfo->tm_wday - 1]
        );

    printf(
            "No.%d days of %d, time zone: %s.\n", 
            __timeInfo->tm_yday, getYears(__timeInfo->tm_year), __timeInfo->tm_zone
        );
}

char * getCurrentTime(const char * __format)
{
    if (!__format || !(*__format)) 
    {
        fatal("getCurrentTime(__format): NULL or empty format string.\n");
    }

    static char timeStrBuffer[TIME_STRING_BUFF_SIZE] = {0};
    size_t bufferLen = 0UL;
    time_t timeStamp = time(NULL);
    struct tm * locatTimeInfo = localtime(&timeStamp);

    if (!locatTimeInfo) { return NULL; }

    bufferLen = strftime(timeStrBuffer, TIME_STRING_BUFF_SIZE, __format, locatTimeInfo); 

    return (bufferLen == 0) ? NULL : timeStrBuffer;
}