#include "../include/timeFuncAssistant.h"
#include <sys/times.h>

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

void displayProcessTime(const char * __msg)
{
    /*
        tms 结构体描述了一个进程的时间相关信息, 
        结构体内容如下:
    */

#if FALSE
    /* 结构体描述了进程和该进程的子进程所用的 CPU 时间 */
    struct tms
    {
        clock_t tms_utime;		/* 用户态所用的 CPU 时间  */
        clock_t tms_stime;		/* 内核态 (一般是系统调用) 所用的 CPU 时间  */

        clock_t tms_cutime;		/* User CPU time of dead children.  */
        clock_t tms_cstime;		/* System CPU time of dead children.  */
    };
#endif

    struct tms processTime = {0};
    clock_t clockTime = 0L;
    static long int clockTicks = 0L;

    if (__msg != NULL) { printf("%s\n", __msg); }
    
    if (clockTicks == 0) {

        /*
            调用 sysconf 传入 _SC_CLK_TCK 宏来获得每秒包含的时钟计时单元数 (系统的时间频率),
            除以 clock_t 转换为秒
        */
        clockTicks = sysconf(_SC_CLK_TCK);
        if (clockTicks == -1) { errExit("sysconf(_SC_CLK_TCK)"); }
    }

    /*
        clock() 返回当前进程调用所使用的总 CPU 时间 (用户 + 内核)
    */
    clockTime = clock();
    if (clockTime == -1) { errExit("clock()"); }

    printf(
            "        clock() returns: %ld clock-per-sec (%.4f secs)\n",
            (long int)clockTime, (double)clockTime / CLOCKS_PER_SEC
        );

    if (times(&processTime) == -1) {
        errExit("times(&processTime)");
    }

    printf(
            "        times() yields: User CPU = %.4f secs; System CPU = %.4f secs\n",
            (double) processTime.tms_utime / clockTicks,
            (double) processTime.tms_stime / clockTicks
        );
}