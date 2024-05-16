#include <sys/stat.h>
#include <sys/fcntl.h>

#include "../../include/tlpi_hdr.h"

/**
 * write() 函数的基本使用，
 * 把一个数组的内容转成字符串后往一个文本文件发送。
*/

/**
 * @brief 要把一个数字转化成字符串后所需要的长度
 * 
 * @param __number 数字
 * 
 * @return 数字转化成字符串后所需要的长度
*/
size_t getByteBufferSize(int __number);

int main(int argc, char const *argv[])
{
    int bufferArray[6] = {114514, 1919810, 332623, 425645, 134534, 4567633}; 

    int arrDataFd = open("./data/arrayData.txt", O_CREAT | O_RDWR | O_TRUNC);

    if (arrDataFd == -1) { errExit("Opening file: [../data/arrayData.dat]"); }

    for (size_t index = 0; index < 6; ++index)
    {
        size_t numStrSize = getByteBufferSize(bufferArray[index]);
        char * byteBuffer = (char *)calloc(numStrSize, sizeof(char));

        snprintf(byteBuffer, numStrSize, "%d", bufferArray[index]);

        byteBuffer[numStrSize - 1] = ' ';

        if (write(arrDataFd, byteBuffer, numStrSize) == -1) { errExit("Write file: [../data/arrayData.dat]"); }



        free(byteBuffer);
    }

    fprintf(stdout, "Write ./data/arrayData.txt complete.\n");

    close(arrDataFd);

    return EXIT_SUCCESS;
}

size_t getByteBufferSize(int __number)
{
    size_t bytesCount = 0UL;
    while ((__number /= 10) > 0) { ++bytesCount; }

    return bytesCount + 2;
}