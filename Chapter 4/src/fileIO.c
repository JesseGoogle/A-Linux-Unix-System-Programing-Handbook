#include <sys/stat.h>
#include <fcntl.h>

#include "../../include/tlpi_hdr.h"

/**
 * 允许使用 -D 选项去重载该定义，例如：
 * 
 * gcc ..... -D BUFFER_SIZE=2048
*/
#ifndef BUFFER_SIZE

#define BUFFER_SIZE 1024

#endif  // BUFFER_SIZE

int main(int argc, char const *argv[])
{
    system("clear");

    /*
        输入/输出 文件描述符，
        文件打开标志位，指定文件的打开方式
    */
    int inputFd, outputFd, openFlags;

    /*文件权限*/
    mode_t filePerms;

    /*读取字节数*/
    ssize_t numRead;
    char buffer[BUFFER_SIZE];   // 缓冲区所在

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s [source-file] [new-file]\n", argv[0]);
    }
    
    /*用只读模式 O_RDONLY 打开 source-file（不会创建新文件）*/
    inputFd = open(argv[1], O_RDONLY);

    /*若返回文件描述符为 -1,证明打开失败，报错*/
    if (inputFd == -1) { errExit("Opening file: [%s]", argv[1]); }

    /*设置 open 标志位，会创建或清空一个可以读写的文件*/
    openFlags = (O_CREAT | O_WRONLY | O_TRUNC);

    /*文件对于 自己，群组，其他用户 皆可访问，即 (rw-rw-rw)*/
    filePerms = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    outputFd = open(argv[2], openFlags, filePerms);
    
    if (outputFd == -1) { errExit("Opening file: [%s]", argv[2]); }

    /**
     * 每一次，从 inputFd 所指向的文件中，
     * 读取 BUFFER_SIZE 字节的内容到 buffer 中，返回此次读取的字节数。
    */
    while ((numRead = read(inputFd, buffer, BUFFER_SIZE)) > 0)
    {
        /**
         * 把 buffer 中 BUFFER_SIZE 字节的数据写入 outputFd 所指向的文件中，
         * 并将返回值和 numRead 进行比对。
        */
        if (write(outputFd, buffer, numRead) != numRead)
        {
            /*
                若 write() 的返回值和 read() 的返回值不相等，
                则说明不能将整个 buffer 写入文件。
            */
            fatal("Couldn't write whole buffer");
        }
    }

    if (numRead == -1) { errExit("read"); }

    if (close(outputFd) == -1) { errExit("Close output"); }
    if (close(inputFd) == -1) { errExit("Close input"); }

    return EXIT_SUCCESS;
}
