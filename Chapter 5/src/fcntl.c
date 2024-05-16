#include "../../include/tlpi_hdr.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>

/**
 * fcntl() 函数的用途之一是针对一个打开的文件，
 * 获取或修改其访问模式和标志状态（这些值是通过指定 open() 调用的 flag 参数来设置的）。
 * 要获取这些设置，应该将 fcntl() 的 cmd 参数设置为 F_GETFL
*/

/**
 * fcntl(fd, F_GETFL) 的返回值是 fd 所指向的文件访问模式相或的结果。
 * 如（O_RDWR | O_TRUNC），所以需要设计一些宏去解析 fcntl() 函数的返回值，
 * 让它和所有标志位逐个相与，得出该文件的具体访问模式。
*/
#define IS_READ_ONLY(__fd)   ((fcntl(__fd, F_GETFL) & O_RDONLY) != FALSE)
#define IS_WRITE_ONLY(__fd)  ((fcntl(__fd, F_GETFL) & O_WRONLY) != FALSE)
#define IS_READ_WRITE(__fd)  ((fcntl(__fd, F_GETFL) & O_RDWR) != FALSE)
#define IS_APPEND(__fd)      ((fcntl(__fd, F_GETFL) & O_APPEND) != FALSE)

enum FlagSetting { INCREASE = TRUE, REDUCE = FALSE };

/**
 * @brief 修改一个文件的访问模式。
*/
void modifyFileFlag(const int __fd, enum FlagSetting __flagset, const int __Flag);

void getFileAccessMode(const int __fd);

int main(int argc, char const *argv[])
{
    system("clear");

    int     fd          = 0;
    int     loadFlag    = O_RDWR;
    mode_t  accessPower = S_IWUSR | S_IRUSR;
    char    buffer[64]  = {0};

    if ((fd = open("./data/test.txt", loadFlag, accessPower)) == -1) 
    { 
        errExit("open(""./data/test.txt"", loadFlag, accessPower)"); 
    }

    struct stat testStat = getFileState(fd);

    /**
     * 检测文件的访问模式和文件详细属性。
    */
    getFileAccessMode(fd);
    showFileState("./data/test.txt", &testStat, stdout);

    printf("After modifyFileFlag(..., INCREASE, ...)\n");

    modifyFileFlag(fd, INCREASE, O_APPEND);

    getFileAccessMode(fd);
    showFileState("./data/test.txt", &testStat, stdout);

    printf("After modifyFileFlag(..., REDUCE, ...)\n");

    modifyFileFlag(fd, REDUCE, O_APPEND);

    getFileAccessMode(fd);
    showFileState("./data/test.txt", &testStat, stdout);

    if (read(fd, buffer, sizeof(buffer)) < 0) { errExit("read(fd, buffer, 64)"); }

    fprintf(stdout, "%s\n", buffer);

    return EXIT_SUCCESS;
}

void getFileAccessMode(const int __fd)
{
    printf("File: [%d] Access mode is: ", __fd);
    if (IS_READ_WRITE(__fd)) { fputs("<Read-Write> ", stdout); }
    if (IS_READ_ONLY(__fd))  { fputs("<Read-Only> ", stdout); }
    if (IS_APPEND(__fd))     { fputs("<Append> ", stdout); }

    fputc('\n', stdout);
}

void modifyFileFlag(const int __fd, enum FlagSetting __flagset, const int __Flag)
{
    int accessMode = 0;
    if ((accessMode = fcntl(__fd, F_GETFL)) == -1) { errExit("addFileFlag() -> fcntl(..., F_GETFL)"); }

    if (__flagset == INCREASE)
    {
        accessMode |= __Flag;
        if (fcntl(__fd, F_SETFL, accessMode) == -1) 
        { 
            errExit("addFileFlag(..., INCREASE, ...) -> fcntl(__fd, F_SETFL, accessMode)"); 
        }
    }
    else if (__flagset == REDUCE)
    {
        accessMode &= ~__Flag;
        if (fcntl(__fd, F_SETFL, accessMode) == -1) 
        { 
            errExit("addFileFlag(..., REDUCE, ...) -> fcntl(__fd, F_SETFL, accessMode)"); 
        }
    }
    else
    {
        fatal("addFileFlag(..., __flagset, ...): invalid __flagset!\n");
    }
}