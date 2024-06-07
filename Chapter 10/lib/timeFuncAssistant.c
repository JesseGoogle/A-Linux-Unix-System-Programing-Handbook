#include "../include/timeFuncAssistant.h"

void showtmStruct(const struct tm * __timeInfo)
{
    if (!__timeInfo) { errExit("showtmStruct(__timeInfo): NULL argument __timeInfo."); }

    printf(
            "[%d.%d.%d %d:%d:%d] %s %s\n",
            getYears(__timeInfo->tm_year), __timeInfo->tm_mon, __timeInfo->tm_mday,
            __timeInfo->tm_hour, __timeInfo->tm_min, __timeInfo->tm_sec,
            Month[__timeInfo->tm_mon], Weeks[__timeInfo->tm_wday - 1]
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

    /**
         * strftime() 函数的功能非常丰富, 常用的几个有:
         * 
         * %a       星期的缩写 (如 Thu)
         * %A       星期的全写 (如 Thursday)
         * %b %h    月份名称的缩写
         * %B       月份全称
         * %c       日期和时间 (如 Tue Feb 1 21:39:46 2023)
         * %d       一个月的第 n 天, 两个字符
         * %D       美国日期格式  (06/ 07/ 24)
         * %F       ISO 日期格式 (2024-06-07)
         * %H       小时 (24 小时表示法 17)
         * %I       小时 (12 小时表示法 05)
         * %j       一年中的某一天 [1 - 366]
         * %m       十进制月 (2 位, 01 到 12)
         * %M       分
         * %p / %P  (am/pm 或 AM/PM)
         * %R       24 小时制的时间 (17:27)
         * %S       秒 (00 至 60)
         * %T       时间 (21: 39: 46)
         * %Y       年份, 四位字符 (2024)
    */
    bufferLen = strftime(timeStrBuffer, TIME_STRING_BUFF_SIZE, __format, locatTimeInfo); 

    return (bufferLen == 0) ? NULL : timeStrBuffer;
}