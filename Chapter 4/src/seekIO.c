#include <sys/stat.h>
#include <sys/fcntl.h>

#include <ctype.h>

#include "../../include/tlpi_hdr.h"

/**
 * 程序演示了 lseek() 与 read() write() 的协同使用。
 * 
 * 该程序的第一个命令行参数为将要打开的文件名称，余下的参数则指定了在文件上执行的 I/O 操作。
 * 
 * seekIO <file-name> {r<length> | R<length> | w<string> | s<offset>} ...
 * 
 *      file-name 要操作的文件名或路径
 *      r<length> 从当前文件指针所在开始，往后读 length 个字节数据，并以文本格式显示。
 *      R<length> 从当前文件指针所在开始，往后读 length 个字节数据，并以 16 进制显示。
 *      w<string> 在当前文件偏移量处，从文件中读写入由 string 指定的字符串。
 *      s<offset> 从文件开头偏移文件指针到 offset 指定的位置。
*/

/**
 * @brief 显示软件的帮助信息。
 * 
 * @param __fileName 文件名或路径
*/
void showHelp(const char * __programName) 
{ 
    system("clear");
    usageErr("[%s] <file-name> {r<length> | R<length> | w<string> | s<offset>} ...\n", __programName); 
}

int main(int argc, char const *argv[])
{
    size_t length = 0UL;        // 读取文件长度
    off_t  offset = 0L;         // 文件偏移量
    int    loadFd = 0;          // 打开文件的文件描述符
    int    argvIndex = 0;       // 程序参数列表索引
    
    char * buffer = NULL;       // 读取缓存区指针，需要动态分配

    /*read() write() 返回的读写字节数*/
    ssize_t readBytes = 0L, writeBytes = 0L;

    /*读写权限 rw - rw - rw*/
    mode_t  loadAuthority = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    /*读写方式 可读可写，若文件不存在则创建之*/
    int     loadFlag      = O_CREAT | O_RDWR;

    if (argc < 3 || (strcmp(argv[1], "--help") == 0)) { showHelp(argv[0]); }

    if ((loadFd = open(argv[1], loadFlag, loadAuthority)) == -1) { errExit("Open"); }

    /**
     * 逐个遍历并解析每一个程序参数
    */
    for (argvIndex = 2; argvIndex < argc; ++argvIndex)
    {
        switch(argv[argvIndex][0])
        {   
            case 'r':   // 从当前文件指针所在开始，往后读 length 个字节数据，并以文本格式显示。
            case 'R':   // 从当前文件指针所在开始，往后读 length 个字节数据，并以 16 进制显示。

                /*
                    假设传入一个 option: “R1024”，
                    那么从第 2 个字符开始，将后面的字符全部转成长整形，即 1024。
                */
                length = getLong(&argv[argvIndex][1], GN_ANY_BASE, argv[argvIndex]);

                // 开辟 length 字节的缓冲区并作返回值检查
                if (!(buffer = (char *)malloc(length))) { errExit("malloc"); }

                // 从文件中读取 length 字节的数据到 buffer，并做返回值检查
                if ((readBytes = read(loadFd, buffer, length)) == -1) { errExit("read"); }

                // 检测是否读到了文件尾部。
                if (!readBytes) { fprintf(stdout, "%s: End-of-file.\n", argv[argvIndex]); }
                else
                {
                    printf("%s: ", argv[argvIndex]);

                    if (argv[argvIndex][0] == 'r')      // 直接按文本输出
                    {
                        printf("%s", buffer);
                    }

                    for (size_t index = 0; index < readBytes; ++index)  // 将数据但个字节，16 进制输出
                    {
                        if (argv[argvIndex][0] == 'R')
                        {
                            printf("0x%02x ", (unsigned int)buffer[index]);
                        }
                    }
                    putchar('\n');
                }

                free(buffer);
                break;
            case 'w':
                if ((writeBytes = write(loadFd, &argv[argvIndex][1], strlen(&argv[argvIndex][1]))) == -1)
                    errExit("write");
                
                fprintf(stdout, "[%s] wrote %ld bytes.\n", argv[argvIndex], writeBytes);
                
                break;
            case 's':
                offset = getLong(&argv[argvIndex][1], GN_ANY_BASE, argv[argvIndex]);

                if (lseek(loadFd, offset, SEEK_SET) == -1) { errExit("lseek"); }

                fprintf(stdout, "[%s] Seek success! offset = %ld\n", argv[argvIndex], offset);

                break;

            default:
                cmdLineErr("Invalid argument: [%s]\n", argv[argvIndex]);

                break;
        }
    }

    close(loadFd);

    return EXIT_SUCCESS;
}
