#include <stdarg.h>

//#include "../include/errorFunctions.h"
#include "../include/tlpi_hdr.h"
#include "../include/ename.c.inc"

void printSplitLine(size_t __len, const char __style, const int __fd)
{
    char * splitLine = (char * )calloc(__len, sizeof(char));

    if (!splitLine) { errExit("calloc(__len, sizeof(char))"); }

    memset(splitLine, __style, __len);

    ssize_t writeByte = write(__fd, splitLine, __len);

    if (writeByte == -1) { free(splitLine); close(__fd); fatal("write(__fd, splitLine, __len)"); }

    free(splitLine);
}

#ifdef __GNUC__ 

    __attribute__((__noreturn__))

#endif // __GNUC__

/**
 * @brief 根据 EF_DUMPCORE 环境变量是否存在来选择结束程序的方式。
 * 
 * @param __useExit3 选择使用 exit 还是 _exit
 * 
 * @return no return
*/
static void terminate(Boolean __useExit3)
{
    char * str;

    str = getenv("EF_DUMPCORE");

    if (!str && !(*str))
        abort();
    
    else if (__useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

/**
 * @brief 根据给定的参数输出合适的错误消息，
 *        作为 errorFunctions.h 文件内函数的辅助函数。
 * 
 * @param __useErr          是否发送错误消息类型
 * @param __err             错误码
 * @param __flushStdout     是否刷新标准输出
 * @param __format          格式化字符串
 * @param __ap              可变参数列表
 * 
 * @return no return
*/
static void outputError(
                          Boolean __useErr, int __err, Boolean __flushStdout,
                          const char * __format, va_list __ap
                    )
{
#define BUFFER_SIZE 500

    char buffer[BUFFER_SIZE * 3], userMsg[BUFFER_SIZE], errText[BUFFER_SIZE];

    /**
     * 根据 格式化字符（__format）和 可变参数列表（__ap）提供的数据，
     * 往一个字符串（userMsg）发送规定大小（BUFFER_SIZE）的字节流。
    */
    vsnprintf(userMsg, BUFFER_SIZE, __format, __ap);

    if (__useErr)
    {
        /**
         * 将从错误码（__err）从 错误索引表（ename）以及 strerror() 函数的返回值中获取的两个字符串，
         * 拼合 [%s %s] 后向 errText 发送 BUFFER_SIZE 大小的字节流。
        */
        snprintf(
                    errText, BUFFER_SIZE, "[%s %s]",
                    (__err > 0 && __err <= MAX_ENAME) ? ename[__err] : "?UNKNOW?", strerror(__err)
                );
    }
    else
        snprintf(errText, BUFFER_SIZE, ":"); // 否则就只发送一个 : 字符。

    /**
     * 最后将 errText 和 userMsg 这两个字符串拼接成 "ERROR (errText) (userMsg)" 的形式，
     * 发送给字符串 buffer，形成最后的错误字符串。
    */
    snprintf(buffer, BUFFER_SIZE * 3, "ERROR %s %s\n", errText, userMsg);

    /*用户来选择是否刷新标准输出*/
    if (__flushStdout) { fflush(stdout); }

    /**
     * 最后往标准错误流发送并刷新标准错误流。
    */
    fputs(buffer, stderr);
    fflush(stderr);
}

void errMessage(const char * __format, ...)
{
    va_list argumentList;

    int savedError = errno;

    va_start(argumentList, __format);
    outputError(TRUE, savedError, TRUE, __format, argumentList);

    va_end(argumentList);

    errno = savedError;
}

void errExit(const char * __format, ...)
{
    va_list argumentList;

    va_start(argumentList, __format);

    outputError(TRUE, errno, TRUE, __format, argumentList);

    terminate(FALSE);
}

void err_exit(const char * __format, ...)
{
    va_list argumentList;

    va_start(argumentList, __format);

    outputError(TRUE, errno, FALSE, __format, argumentList);

    terminate(FALSE);
}

void errExitEN(int __errNum, const char * __format, ...)
{
    va_list argumentList;

    va_start(argumentList, __format);

    outputError(TRUE, errno, FALSE, __format, argumentList);

    va_end(argumentList);

    terminate(TRUE);
}

void fatal(const char * __format, ...)
{
    va_list argumentList;

    va_start(argumentList, __format);

    outputError(FALSE, 0, TRUE, __format, argumentList);

    va_end(argumentList);

    terminate(TRUE);
}

void usageErr(const char * __format, ...)
{
    va_list argumentList;

    fflush(stdout);

    fprintf(stderr, "Usage: ");

    va_start(argumentList, __format);

    vfprintf(stderr, __format, argumentList);

    va_end(argumentList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

void cmdLineErr(const char * __format, ...)
{
    va_list argumentList;

    fflush(stderr);

    fprintf(stderr, "Command-line usage error: ");

    va_start(argumentList, __format);

    vfprintf(stderr, __format, argumentList);

    va_end(argumentList);

    fflush(stderr);

    exit(EXIT_FAILURE);
}