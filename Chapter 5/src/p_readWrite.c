#include "../../include/tlpi_hdr.h"

#include <fcntl.h>
#include <sys/stat.h>

/**
 * 本程序学习 pread() 和 pwrite() 函数的使用，
 * 它们可以在不改变文件指针偏移量的情况下读写文件。
 * 这在多线程并法访问文件的情况下能避免数据竞争。
*/

#ifndef BUFFER_SIZE

#define BUFFER_SIZE 4096

#endif // BUFFER_SIZE

int main(int argc, char const *argv[])
{
    system("clear");

    const char filePath[] = {"./data/minecraftLog.log"};

    int logFd           = 0;
    int accessMode      = O_RDWR;
    mode_t accessPower  = S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH;
    ssize_t readBytes   = 0L;
    ssize_t writeBytes  = 0L;
    char buffer[BUFFER_SIZE] = {0};
    char writeContent[BUFFER_SIZE / 2] = {0};

    if ((logFd = open(filePath, accessMode, accessPower)) == -1) { errExit("open()"); }

    struct stat logFileState = getFileState(logFd);

    //showFileState(filePath, &logFileState, STDOUT_FILENO);

    printf("Call pread() to read [%s] but not modify file offset.\n", filePath);
    if ((readBytes = pread(logFd, buffer, 536, logFileState.st_size - 536)) == -1) 
    { 
        errExit("pread(logFd, buffer, 128, logFileState.st_size - 128)"); 
    }

    printf("%s\n", buffer);

    /**
     * lseek(logFd, 0, SEEK_CUR) 
     * 可以使用这样的操作获取当前文件指针的偏移量
    */
    printf("After pread() the [%s]'s offset = %ld\n", filePath, lseek(logFd, 0, SEEK_CUR));

    printf("Press sth, then write to file end: \n");
    fgets(writeContent, BUFFER_SIZE / 2, stdin);

    if((writeBytes = pwrite(logFd, writeContent, strlen(writeContent), logFileState.st_size)) == -1)
    {
        errExit("pwrite(logFd, writeContent, strlen(writeContent), logFileState.st_size)");
    }

    close(logFd);
}