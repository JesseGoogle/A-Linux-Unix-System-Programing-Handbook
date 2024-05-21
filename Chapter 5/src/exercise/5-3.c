#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include "../../../include/tlpi_hdr.h"

/**
 * 本程序设计目的在于展示何以 O_APPEND 标志打开文件来保障操作原子性是必要的，
 * 程序会接受 3 个参数：
 *      
 *         atomic_append <file-name> <n-bytes> [x]
 * 
 * 程序打开所指定的文件（若没有则创建），然后每次调用 write() 写入一字节的方式，
 * 向文件尾追加 <n-bytes> 字节的数据，但若存在第三个命令行参数 x，那么就不使用 
 * O_APPEND 标志位打开文件，取而代之的是，用 lseek(fd, 0, SEEK_END) 来定位文件尾部。
 * 
 * 最后使用以下两条命令运行程序：
 * 
 *      [1] atomic_append test2.txt 1000000 x & atomic_append test2.txt 1000000 x
 * 
 *      [2] atomic_append test.txt 1000000 & atomic_append test.txt 1000000
 * 
 * 两条通过并行的方式往同一个文件写入 2000000 字节，运行完成后使用 ls -l 命令查看文件字节数，
 * 并解释命令 [1] 执行后文件大小不符合预期的原因。
*/

/**
 * @brief 检查命令行参赛是否合法
*/
void checkArgumentList(int argc, char const *argv[]);

/**
 * @brief 通过是否有参数 [x] 来确定文件的访问模式
 * 
 * @param __xFlagOpen 参数 [x] 是否存在
 * @param __argv3     参数 argv[3]
 * 
 * @return 文件的访问模式
*/
int confirmAccessMode(Boolean * __xFlagOpen, const char * __argv3);

/**
 * @brief 按照长度随即生成一个由 [a-z A-Z] 组成的字符串，
 *        通过 "抛硬币" 来决定一次字符的生成的大小写。
 * 
 * @param __nBytes          生成字符串的长度
 * @param __generatedStr    生成的字符串
 * 
 * @return 生成的字符串
*/
char * randString(const int __nBytes, char * __generatedStr);

/**
 * @brief 往文件写入指定的字节数，但应要求，需要逐字节写入。
 * 
 * @param __fd          文件描述符
 * @param __nBytes      要写入的总字符数
 * @param __xFlagOp     是否使用 O_APPEND 标志位
 * @param __buff        要写入文件的数据所在
 * 
 * @return 一共写入的字节数
*/
ssize_t writeToFile(const int __fd, const ssize_t __nBytes, const Boolean __xFlagOp, char * __buff);

int main(int argc, char const *argv[])
{
    if (system("clear") != 0) { return -1; }

    checkArgumentList(argc, argv);

    const char * xFlag = argv[3];
    ssize_t nBytes     = getInt(argv[2], GN_ANY_BASE, "getInt(argv[2], GN_ANY_BASE, ...)");
    Boolean xFalgOpen  = FALSE;

    int filedesc            = 0;
    int accessMode          = confirmAccessMode(&xFalgOpen, xFlag);
    mode_t accessPower      = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    ssize_t writtenBytes    = 0L;

    char * buffer = randString(nBytes, buffer);

    if ((filedesc = open(argv[1], accessMode, accessPower)) == -1) 
    { 
        errExit("main() -> open(%s, accessMode, accessPower)", argv[1]); 
    }

    writtenBytes = writeToFile(filedesc, nBytes, xFalgOpen, buffer);

    printSplitLine(50, '-', STDOUT_FILENO); puts("");

    printf(
            "Written %ld Bytes to file [%s], fd = %d\n", 
            writtenBytes, argv[1], filedesc
        );

    printSplitLine(50, '-', STDOUT_FILENO); puts("");

    free(buffer);
    close(filedesc);

    return EXIT_SUCCESS;
}

void checkArgumentList(int argc, char const *argv[])
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0) 
    { 
        usageErr("%s <file-name> <n-bytes> [x]\n", argv[0]);
    }
    else if (argc == 4 && strcmp(argv[3], "x") != 0)
    {
        fprintf(stderr, "Only have [x] flags, not [%s]!\n", argv[3]);
        usageErr("%s <file-name> <n-bytes> [x]\n", argv[0]);
    }
    else if (argc > 4) { usageErr("%s <file-name> <n-bytes> [x]\n", argv[0]); }
}

int confirmAccessMode(Boolean * __xFlagOpen, const char * __argv3)
{
    if (!__argv3 || !(*__argv3)) 
    { 
        printf("xFlagOpen = %s\n", (*__xFlagOpen) ? "TRUE" : "FALSE");
        printf("File access mode = O_RDWR | O_CREAT | O_APPEND\n"); 
        return O_RDWR | O_CREAT | O_APPEND; 
    }

    if (strcmp(__argv3, "x") == 0) { *__xFlagOpen = TRUE; }
    else { *__xFlagOpen = FALSE; }

    printf("xFlagOpen = %s\n", (*__xFlagOpen) ? "TRUE" : "FALSE");

    switch (*__xFlagOpen)
    {   
        /**
         * 若第二个命令行参数中有 x 标志位，就返回没有 APPEND 的访问模式
        */
        case TRUE:
            printf("File access mode = O_RDWR | O_CREAT\n");
            return O_RDWR | O_CREAT;

        /**
         * 反之则加上 O_APPEND 标志再返回
        */
        case FALSE:
            printf("File access mode = O_RDWR | O_CREAT | O_APPEND\n");
            return O_RDWR | O_CREAT | O_APPEND;
    }
}

char * randString(const int __nBytes, char * __generatedStr)
{
    srand(time(NULL));
    int upperOrLower = -1;
    __generatedStr = (char *)calloc(__nBytes + 1, sizeof(char));

    if (!__generatedStr) { errExit("randString() -> (char *)calloc(__nBytes, sizeof(char))"); }

    for (int index = 0; index < __nBytes; ++index)
    {
        upperOrLower = rand() % 2;

        if (upperOrLower == TRUE) // UPPER
        {
            __generatedStr[index] = (char)(rand() % 26 + 'A');
        }
        else if (upperOrLower == FALSE) // LOWER
        {
            __generatedStr[index] = (char)(rand() % 26 + 'a');
        }
    }

    __generatedStr[__nBytes] = '\0';

    return __generatedStr;
}

ssize_t writeToFile(const int __fd, const ssize_t __nBytes, const Boolean __xFlagOp, char * __buff)
{
    ssize_t writtenBytes = 0L;

    switch (__xFlagOp)
    {
        case TRUE:
            for (int index = 0; index < __nBytes; ++index)
            {
                if (lseek(__fd, 0, SEEK_END) == -1) { errExit("lseek(__fd, 0, SEEK_END"); }

                if ((writtenBytes += write(__fd, &__buff[index], 1)) == -1)
                {
                    free(__buff);
                    close(__fd);
                    errExit("writeToFile(TRUE) -> write(__fd, __buff[index], 1)");
                }
            }

            if (writtenBytes < __nBytes)
            {
                free(__buff);
                close(__fd);
                fatal("Written fewer bytes than __nBytes!");
            }

            break;

        case FALSE:
            for (int index = 0; index < __nBytes; ++index)
            {
                if ((writtenBytes += write(__fd, &__buff[index], 1)) == -1)
                {
                    free(__buff);
                    close(__fd);
                    errExit("writeToFile(TRUE) -> write(__fd, __buff[index], 1)");
                }
            }

            if (writtenBytes < __nBytes)
            {
                free(__buff);
                close(__fd);
                fatal("Written fewer bytes than __nBytes!");
            }

            break;
    }

    return writtenBytes;
}