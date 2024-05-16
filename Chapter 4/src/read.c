#include <sys/stat.h>
#include <sys/fcntl.h>

#include "../../include/tlpi_hdr.h"

/**
 * read() 函数的基本使用，
 * 从标准输入（目前是键盘）读取字节流最后发送到标准输出。
*/

#define BUFFER_SIZE 64

int main(int argc, char const *argv[])
{
    char buffer[BUFFER_SIZE + 1];

    /**
     * 从标准输入中（默认是键盘，但随时可以重定向）读取数据到 buffer。
    */
    if (read(STDIN_FILENO, buffer, BUFFER_SIZE) == -1) { errExit("read"); }


    buffer[BUFFER_SIZE] = '\0'; // 由于读取的数据不一定是字符流，所以需要手动往缓冲区末尾补上空字符。

    fprintf(stdout, "The input data was: %s\n", buffer);

    return EXIT_SUCCESS;
}
