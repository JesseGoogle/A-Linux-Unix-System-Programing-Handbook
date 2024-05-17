#include "../../include/tlpi_hdr.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BUFFER_SIZE

#define BUFFER_SIZE 4096

#endif // BUFFER_SIZE

int main(int argc, char const *argv[])
{
    const char filePath[] = {"./data/minecraftLog.log"};

    int logFd           = 0;
    int accessMode      = O_APPEND | O_WRONLY | O_CREAT;
    mode_t accessPower  = S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP;
    ssize_t readBytes   = 0L;
    ssize_t writeBytes  = 0L;
    char buffer[BUFFER_SIZE] = {0};

    if ((logFd = open(filePath, accessMode, accessPower)) == -1) { errExit("open()"); }

    struct stat logState = getFileState(logFd); // 获取文件详细属性

    int stdoutCopyFd = 0;

    /*复制标准输出到一个新的文件描述符*/
    if ((stdoutCopyFd = dup(STDOUT_FILENO)) == -1) 
    { 
        close(logFd); 
        errExit("dup(STDOUT_FILENO)"); 
    }

    /**
     * 使用 dup2() 函数，
     * 将 STDOUT_FILENO 即标准输出重定向到 logFd 所代表的文件中。
    */
    if (dup2(logFd, STDOUT_FILENO) == -1)
    {
        close(logFd); 
        close(stdoutCopyFd);
        errExit("dup2(logFd, STDOUT_FILENO)"); 
    }

    /**
     * 此时会把文件详细属性输出到 logFd 所代表的文件中，
     * 但是并没有输出到终端。
    */
    showFileState(filePath, &logState, stdoutCopyFd);
    //showFileState(filePath, &logState, logFd);

    /**
     * 恢复标准输出到原来的状态，注释掉这块代码时，内容就输出到终端而非文件了。
    */
    if (dup2(stdoutCopyFd, STDOUT_FILENO) == -1)
    {
        errExit("dup2(logFd, STDOUT_FILENO)"); 
    }

    close(logFd); 
    close(stdoutCopyFd);

    return EXIT_SUCCESS;
}
