#include <sys/time.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define UTC_TIME "UTC Time: 1970.1.1"

void showTimeval(const struct timeval * __timeval);

int main(int argc, char const *argv[])
{
    system("clear");

    struct timeval tv = {0};

    /**
     * gettimeofday() 函数可以获取从
     * 通用协调时间 (UTC) 到程序调用这一刻所经过的秒数, 
     * 存储在 timeval 结构体的 tv_sec 字段
     * 
     * timeval 结构体还有一个附加的 tv_usec 字段, 可以把整个结果精确到微秒级别
    */
    gettimeofday(&tv, NULL);

    showTimeval(&tv);

    /**
     * 使用 time 函数 (C 标准库函数) 也能获取从通用协调时间 (UTC) 到程序调用这一刻所经过的秒数,
     * 一些 time() 函数的实现其实是在内部调用了 gettimeofday()
    */
    time_t seconds = time(NULL);

    printf(
            "It is past [%ld seconds] than: %s.\n",
            seconds, UTC_TIME
        );

    return EXIT_SUCCESS;
}

void showTimeval(const struct timeval * __timeval)
{
    if (!__timeval) { return; }

    printf(
            "It is past [%ld seconds and %ld microseconds] than: %s.\n",
            __timeval->tv_sec, __timeval->tv_usec, UTC_TIME
        );
}
