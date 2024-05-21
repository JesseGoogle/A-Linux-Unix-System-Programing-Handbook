#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include "../../../include/tlpi_hdr.h"

void checkArgumentList(int argc, char const *argv[]);
int correctAccessMode(Boolean * __xFlagOpen, const char * __argv3);

/**
 * @brief 按照长度随即生成一个字符串，ASCII 码在 33 - 126 之间的字符
 * 
 * @param __nBytes          生成字符串的长度
 * @param __generatedStr    生成的字符串
 * 
 * @return 生成的字符串
*/
char * randString(const int __nBytes, char * __generatedStr);

ssize_t writeToFile(const int __fd, const ssize_t __nBytes, const Boolean __xFlagOp, const char * __buff);

int main(int argc, char const *argv[])
{
    if (system("clear") != 0) { return -1; }

    checkArgumentList(argc, argv);

    const char * xFlag = argv[3];
    ssize_t nBytes     = getInt(argv[2], GN_ANY_BASE, "getInt(argv[2], GN_ANY_BASE, ...)");
    Boolean xFalgOpen  = FALSE;

    int filedesc            = 0;
    int accessMode          = correctAccessMode(&xFalgOpen, xFlag);
    mode_t accessPower      = S_IRUSR | S_IWUSR;
    ssize_t writtenBytes    = 0L;

    char * buffer = randString(nBytes, buffer);

    //printf("%s\n", buffer);

    if ((filedesc = open(argv[1], accessMode, accessPower)) == -1) 
    { 
        errExit("open(%s, accessMode, accessPower)", argv[1]); 
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

int correctAccessMode(Boolean * __xFlagOpen, const char * __argv3)
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

ssize_t writeToFile(const int __fd, const ssize_t __nBytes, const Boolean __xFlagOp, const char * __buff)
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
                    errExit("writeToFile(TRUE) -> write(__fd, __buff[index], 1)");
                }
            }

            if (writtenBytes < __nBytes)
            {
                fatal("Written fewer bytes than __nBytes!");
            }

            break;

        case FALSE:
            for (int index = 0; index < __nBytes; ++index)
            {
                if ((writtenBytes += write(__fd, &__buff[index], 1)) == -1)
                {
                    errExit("writeToFile(TRUE) -> write(__fd, __buff[index], 1)");
                }
            }

            if (writtenBytes < __nBytes)
            {
                fatal("Written fewer bytes than __nBytes!");
            }

            break;
    }

    return writtenBytes;
}