#include <fcntl.h>
#include <sys/stat.h>

#include "../../../include/tlpi_hdr.h"

/**
 * 本程序主要三为了验证，使用 dup() 或 dup2() 拷贝文件描述符后，
 * 拷贝的新文件描述符是否继承原文件的偏移量，访问标志位等信息。
*/

/**
 * File: [./data/minecraftLog.log] fd = [3], access flag is:
 * <read-write> <append> <non-block> 
 * file ./data/minecraftLog.log offset = 100
 * -----------------------------------------------------------------
 * File: [./data/minecraftLog.log] fd = [4], access flag is:
 * <read-write> <append> <non-block> 
 * file ./data/minecraftLog.log offset = 100
 * 
 * 程序的结果显而易见：拷贝的新文件描述符会继承原文件的偏移量，访问标志位等信息。
*/

int main(int argc, char const *argv[])
{
    system("clear");

    int fd = open(argv[1], O_RDWR | O_APPEND | O_NDELAY | O_NONBLOCK, S_IWUSR | S_IRUSR);

    if (fd == -1) { errExit("open()"); }

    lseek(fd, 100, SEEK_SET);

    getFileAccessFlag(argv[1], fd, STDOUT_FILENO);

    printf("file %s offset = %ld\n", argv[1], getFileCurrentOffset(fd));

    printSplitLine(65, '-', STDOUT_FILENO); puts("");

    int newFd = dup(fd);

    getFileAccessFlag(argv[1], newFd, STDOUT_FILENO);

    printf("file %s offset = %ld\n", argv[1], getFileCurrentOffset(newFd));

    close(fd);

    return EXIT_SUCCESS;
}
