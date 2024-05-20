#include "../../include/tlpi_hdr.h"

#include <fcntl.h>

/**
 * mkstemp() 函数，根据指定的文件路径
 *（例如 ./data/temp/testTempFileXXXXXX，最后 6 个字符必须是 X，在函数调用后它们会被替换，以保证文件名的唯一性）
 * 文件拥有者对 mkstemp() 函数建立的文件拥有读写权限（其他用户则没有任何操作权限），
 * 且打开文件时使用了 O_EXCL 标志，确保调用者以独占的方式使用文件。
*/

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

    int randAmount = getInt(argv[1], GN_ANY_BASE, "getInt(argv[1], GN_ANY_BASE, ...)");
    int delayTimes = getInt(argv[2], GN_ANY_BASE, "getInt(argv[2], GN_ANY_BASE, ...)");
    char ** tempFilePathSet = (char **)calloc(randAmount, sizeof(char *));

    for (size_t sIndex = 0; sIndex < randAmount; ++sIndex)
    {
        tempFilePathSet[sIndex] = (char *)calloc(24, sizeof(char));
        strncpy(tempFilePathSet[sIndex], "./data/temp/testXXXXXX", 23);
        tempFilePathSet[23] = '\0';
    }

    int * fdSet = (int *)calloc(randAmount, sizeof(int));

    for (size_t rIndex = 0; rIndex < randAmount; ++rIndex)
    {
        fdSet[rIndex] = mkstemp(tempFilePathSet[rIndex]);

        printf("Generated temp file name: [%s]\n", tempFilePathSet[rIndex]);

        memset(tempFilePathSet[rIndex] + strlen(tempFilePathSet[rIndex]) - 6, 'X', 6);

        //printf("%d %s\n", strcmp(tempFilePath, "./data/temp/testXXXXXX"), tempFilePath);

        sleep(delayTimes);
    }

    freeFdSetAndFile(fdSet, tempFilePathSet, randAmount);
    free(fdSet);

    for (size_t sIndex = 0; sIndex < randAmount; ++sIndex)
    {
        free(tempFilePathSet[sIndex]);
    }

    return EXIT_SUCCESS;
}

void freeFdSetAndFile(int * __fdSet, char ** __filePathSet, const int __setSize)
{
    for (int index = 0; index < __setSize; ++index)
    {
        unlink(__filePathSet[index]);
        if (close(__fdSet[index]) == -1) { errExit("close(__fdSet[index])"); }
    }
}