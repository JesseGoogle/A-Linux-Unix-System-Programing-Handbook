#include "../../../include/tlpi_hdr.h"

#include <bits/getopt_core.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

/**
 * tee 命令是从标准输入中读取数据，
 * 直至文件结尾，随后将数据写入标准输出和命令行参数指定的文件。
 * 
 * 默认情况下，若已存在与命令行参数 <output-file-name> 的同名文件，程序会将其覆盖.
 * 如果有 -a 选项，则往这个文件后追加数据。
 * 
 * Usage: tee { -a }  <output-file-name>
*/
#ifndef BUFFER_SIZE

#define BUFFER_SIZE 128

#endif

int main(int argc, char *argv[])
{
    int     outputFd = 0;
    int     fileLoadFlag = 0;
    int     option;
    char    byteBuffer[BUFFER_SIZE];

    /*read() write() 返回的读写字节数*/
    ssize_t readBytes = 0L, writeBytes = 0L;

    Boolean aFlag = FALSE;

    /*读写权限 rw - rw - rw*/
    mode_t  loadAuthority = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
    while ((option = getopt(argc, argv, "-a:")) != -1)
    {
        switch (option)
        {
            case 'a':
                aFlag = TRUE;
                break;
            
            case '?':
                aFlag = FALSE;
                break;

            default:
                break;
        }

        /*
            若存在 -a 选项，则往这个文件后追加数据。
        */
        if (aFlag == TRUE)
        {
            system("clear");

            fprintf(stdout, "[%c]: ", option);

            memset(byteBuffer, 0, BUFFER_SIZE);

            fprintf(stdout, "Please enter some character:\n");

            // 往标准输出发送从标准输入读取的字符流
            while ((readBytes = read(STDIN_FILENO, byteBuffer, BUFFER_SIZE)) < 0) { errExit("read()"); }

            byteBuffer[strlen(byteBuffer) - 1] = '\0';

            fprintf(stdout, "Press content: [%s]\n", byteBuffer);

            // 以末尾追加和可读可写模式打开一个访问权限为 rw-rw-rw 的文件。
            fileLoadFlag = O_APPEND | O_RDWR;
            if ((outputFd = open(argv[2], fileLoadFlag, loadAuthority)) == -1) { errExit("open()"); }

            while ((writeBytes = write(outputFd, byteBuffer, strlen(byteBuffer))) < 0) { errExit("write()"); }

            fprintf(stdout, "[%s %s]: Append data complete.\n", argv[1], argv[2]);

            close (outputFd);
        }
        /*
            若存在 -a 选项，则要清空文件后在写入数据。
        */
        else
        {
            system("clear");
            
            fprintf(stdout, "[no option] ");

            memset(byteBuffer, 0, BUFFER_SIZE);

            fprintf(stdout, "Please enter some character:\n");

            // 往标准输出发送从标准输入读取的字符流
            while ((readBytes = read(STDIN_FILENO, byteBuffer, BUFFER_SIZE)) < 0) { errExit("read()"); }

            byteBuffer[strlen(byteBuffer) - 1] = '\0';

            fprintf(stdout, "Press content: [%s]\n", byteBuffer);

            fileLoadFlag = O_CREAT | O_RDWR | O_TRUNC;
            if ((outputFd = open(argv[1], fileLoadFlag, loadAuthority)) == -1) { errExit("open()"); }

            while ((writeBytes = write(outputFd, byteBuffer, strlen(byteBuffer))) < 0) { errExit("write()"); }

            fprintf(stdout, "[(no option) %s]: Create ot Trunc file and write data complete.\n", argv[1]);

            close (outputFd);
        }
    }

    return EXIT_SUCCESS;
}
