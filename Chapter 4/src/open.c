#include <sys/stat.h>
#include <sys/fcntl.h>

#include "../../include/tlpi_hdr.h"

/**
 * open() 函数的使用方法，将一个文件的内容读出并发送给标准输出。
*/
int main(int argc, char const *argv[])
{
    system("clear");

    int enameFd = open("../include/ename.c.inc", O_RDWR, S_IRUSR | S_IWUSR);

    if (enameFd == -1) { errExit("Opening file: [../include/ename.c.inc]"); }

    char readBuffer[128];

    ssize_t readBytes = 0;
    while ((readBytes = read(enameFd, readBuffer, 128)) > 0)
    {
        fprintf(stdout ,"%s", readBuffer);
    }

    putchar('\n');
    close(enameFd);

    return 0;
}