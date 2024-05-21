#include "../../include/tlpi_hdr.h"

#include <fcntl.h>

/**
 * mkstemp() 函数，根据指定的文件路径
 *（例如 ./data/temp/testTempFileXXXXXX，最后 6 个字符必须是 X，在函数调用后它们会被替换，以保证文件名的唯一性）
 * 文件拥有者对 mkstemp() 函数建立的文件拥有读写权限（其他用户则没有任何操作权限），
 * 且打开文件时使用了 O_EXCL 标志，确保调用者以独占的方式使用文件。
*/

#define FILE_PATH_LENGTH 24

/**
 * @brief 逐个关闭文件描述符数组中的所有文件，
 *        并根据文件路径销毁所有临时文件。
 * 
 * @param __fdSet           文件描述符数组
 * @param __filePathSet     文件路径字符串数组
 * @param __setSize         数组长度和文件路径字符串数组长度
 * 
 * @return no return
*/
void freeFdSetAndFile(int * __fdSet, char ** __filePathSet, const int __setSize);

int main(int argc, char const *argv[])
{
    if (system("clear") != 0) { return -1; }

    if (argc < 3 || (strcmp(argv[1], "--help") == 0)) 
    { 
        usageErr("[%s] <rand-amount> <delay-time>\n", argv[0]); 
    }

    /*要生成临时文件的总数*/
    int randAmount = getInt(argv[1], GN_ANY_BASE, "getInt(argv[1], GN_ANY_BASE, ...)");

    /*每生成一个临时文件后要挂起进程的时间*/
    int delayTimes = getInt(argv[2], GN_ANY_BASE, "getInt(argv[2], GN_ANY_BASE, ...)");

    /*临时文件模板路径*/
    const char templateFilePath[] = "./data/temp/testXXXXXX";

    /*临时文件路径字符串数组，动态分配*/
    char ** tempFilePathSet = (char **)calloc(randAmount, sizeof(char *));

    if (!tempFilePathSet) { errExit("(char **)calloc(randAmount, sizeof(char *))"); }

    for (size_t sIndex = 0; sIndex < randAmount; ++sIndex)
    {
        tempFilePathSet[sIndex] = (char *)calloc(FILE_PATH_LENGTH, sizeof(char));

        if (!tempFilePathSet[sIndex]) { errExit("(char *)calloc(FILE_PATH_LENGTH, sizeof(char))"); }

        strncpy(tempFilePathSet[sIndex], templateFilePath, FILE_PATH_LENGTH - 1);
    }

    /*文件描述符数组，动态分配*/
    int * fdSet = (int *)calloc(randAmount, sizeof(int));

    for (size_t rIndex = 0; rIndex < randAmount; ++rIndex)
    {
        /*生成唯一临时文件，返回描述符*/
        if ((fdSet[rIndex] = mkstemp(tempFilePathSet[rIndex])) == -1)
        { 
            errExit("mkstemp(tempFilePathSet[rIndex])"); 
        }

        printf("Generated temp file name: [%s]\n", tempFilePathSet[rIndex]);

        sleep(delayTimes);
    }

    freeFdSetAndFile(fdSet, tempFilePathSet, randAmount);

    puts("Free fdset.");
    free(fdSet);

    puts("Free tempFilePathSet.");
    for (size_t sIndex = 0; sIndex < randAmount; ++sIndex)
    {
        free(tempFilePathSet[sIndex]);
    }

    puts("Done.");

    return EXIT_SUCCESS;
}

void freeFdSetAndFile(int * __fdSet, char ** __filePathSet, const int __setSize)
{
    for (int index = 0; index < __setSize; ++index)
    {
        printf("Close and delete file: [%s], fd = %d\n", __filePathSet[index], __fdSet[index]);
        unlink(__filePathSet[index]);
        if (close(__fdSet[index]) == -1) { errExit("close(__fdSet[index])"); }

        sleep(1);
    }
}