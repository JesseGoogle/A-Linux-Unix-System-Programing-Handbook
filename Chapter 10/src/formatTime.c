#include <sys/unistd.h>

#include <time.h>

#include "../include/timeFuncAssistant.h"

int main(int argc, char const *argv[])
{
    time_t curTimeStamp = time(NULL);
    struct tm * localTimeInfo = localtime(&curTimeStamp);

    /**
     * asctime() 函数用于把一个 tm 结构体的数据转换成一个表示时间信息的字符串,
     * 格式和 ctime() 一样
    */
    char * localTimeString = asctime(localTimeInfo);

    printf("%s", localTimeString);
    showtmStruct(localTimeInfo);

    return EXIT_SUCCESS;
}
