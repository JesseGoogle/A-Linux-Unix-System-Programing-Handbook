#include <sys/unistd.h>

#include <time.h>

#include "../../include/tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    while (TRUE)
    {
        system("clear");

        time_t currentStamp = time(NULL);

        /**
         * ctime() 函数通过传入一个时间戳, 通常由 time() 函数获取,
         * 解析后将型如
         *  
         *      "Thu Jun  6 16:31:52 2024\n\0"
         * 
         * 这样的字符串存储到一个静态分配的字符串中, 每一次调用都会覆盖它
        */
        const char * curTimeString = ctime(&currentStamp);

        printf("Current time = %s", curTimeString);

        sleep(1);
    }
    
    return EXIT_SUCCESS;
}