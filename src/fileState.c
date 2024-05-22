#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

#include "../include/fileState.h"

/**
 * fcntl(fd, F_GETFL) 的返回值是 fd 所指向的文件访问模式相或的结果。
 * 如（O_RDWR | O_TRUNC），所以需要设计一些宏去解析 fcntl() 函数的返回值，
 * 让它和所有标志位逐个相与，得出该文件的具体访问模式。
*/
#define IS_READ_ONLY(__fd)   ((fcntl(__fd, F_GETFL) & O_RDONLY) != FALSE) // 只读
#define IS_WRITE_ONLY(__fd)  ((fcntl(__fd, F_GETFL) & O_WRONLY) != FALSE) // 只写
#define IS_READ_WRITE(__fd)  ((fcntl(__fd, F_GETFL) & O_RDWR)   != FALSE) // 可读可写
#define IS_APPEND(__fd)      ((fcntl(__fd, F_GETFL) & O_APPEND) != FALSE) // 文件尾追加
#define IS_CREATE(__fd)      ((fcntl(__fd, F_GETFL) & O_CREAT)  != FALSE) // 文件不存在则创建
#define IS_EXCL(__fd)        ((fcntl(__fd, F_GETFL) & O_EXCL)   != FALSE) // 结合 O_CREAT 使用，专门用于创建文件
#define IS_NOCTTY(__fd)      ((fcntl(__fd, F_GETFL) & O_NOCTTY) != FALSE) // 不要让所指向的终端设备成为控制终端
#define IS_TRUNC(__fd)       ((fcntl(__fd, F_GETFL) & O_TRUNC)  != FALSE) // 打开文件前先截断为 0
#define IS_ASYNC(__fd)       ((fcntl(__fd, F_GETFL) & O_ASYNC)  != FALSE) // 当 I/O 操作可行时，产生 signal 通知进程
#define IS_NONBLOCK(__fd)    ((fcntl(__fd, F_GETFL) & O_NONBLOCK) != FALSE) // 以非阻塞的方式打开
#define IS_SYNC(__fd)        ((fcntl(__fd, F_GETFL) & O_SYNC)   != FALSE)   // 以同步方式写入文件
//#define IS_DSYNC(__fd)       ((fcntl(__fd, F_GETFL) & __O_DSYNC) != FALSE)
//#define IS_NOFOLLOW(__fd)    ((fcntl(__fd, F_GETFL) & __O_NOFOLLOW) != FALSE)
//#define IS_DIRECT(__fd)      ((fcntl(__fd, F_GETFL) & __O_DIRECT) != FALSE)

struct stat getFileState(const int __fd)
{
    struct stat srcState;

    if (fstat(__fd, &srcState) == -1)
    {
        close(__fd);
        errExit("getFileState() -> fstat(__srcFd, ...)");
    }

    return srcState;
}

void showFileState(const char * __fileName, const struct stat * __fileState, const int __fd)
{
    if (!__fileState || !__fileState->st_nlink)
    {
        fatal("showFileState(..., NULL, ...): Empty stat struct.\n");
    }

    ssize_t writeByte = 0L;
    size_t  maxStrLen = 0UL;
    char stateStrings[FILE_STATE_STRING_AMOUNT][FILE_STATE_STRING_LENGTH];
    memset(*stateStrings, 0, FILE_STATE_STRING_AMOUNT * FILE_STATE_STRING_LENGTH);

    snprintf(stateStrings[0], FILE_STATE_STRING_LENGTH, "\nFile Path: %s\n", __fileName);
    snprintf(stateStrings[1], FILE_STATE_STRING_LENGTH, "File Device ID = %lu\n", __fileState->st_dev);
    snprintf(stateStrings[2], FILE_STATE_STRING_LENGTH, "Inode No. %lu\n", __fileState->st_ino);
    snprintf(stateStrings[3], FILE_STATE_STRING_LENGTH, "Nlink count = %lu\n", __fileState->st_nlink);
    snprintf(stateStrings[4], FILE_STATE_STRING_LENGTH, "File access mode: %u\n", __fileState->st_mode);
    snprintf(
                stateStrings[5], FILE_STATE_STRING_LENGTH, 
                "User ID: [%u] Group ID: [%u] Device type ID: [%lu]\n",
                __fileState->st_uid, __fileState->st_gid, __fileState->st_rdev
        );

    snprintf(
                stateStrings[6], FILE_STATE_STRING_LENGTH,
                "File reality size = %ld Bytes File block size = %ld Bytes Use %ld blocks\n",
                __fileState->st_size, __fileState->st_blksize, __fileState->st_blocks
        );

    snprintf(
                stateStrings[7], FILE_STATE_STRING_LENGTH,
                "Last access time: %sLast modify time: %sLast state change time: %s",
                ctime(&__fileState->st_atime), ctime(&__fileState->st_mtime), ctime(&__fileState->st_ctime)
        );

    maxStrLen = strlen(stateStrings[0]);
    
    for (size_t index = 1; index < FILE_STATE_STRING_AMOUNT - 1; ++index) 
    {
        size_t tempLen = strlen(stateStrings[index]);
        if (tempLen > maxStrLen) { maxStrLen = tempLen; }
    }

    printSplitLine(maxStrLen, '-', __fd);

    for (size_t index = 0; index < FILE_STATE_STRING_AMOUNT; ++index)
    {
        writeByte = write(__fd, stateStrings[index], strlen(stateStrings[index]));
        if (writeByte == -1 || writeByte < strlen(stateStrings[index]))
        {
            close(__fd);
            fatal("write(__fd, stateStrings[index], strlen(stateStrings[index])");
        }
    }

    printSplitLine(maxStrLen, '-', __fd);
    printSplitLine(1, '\n', __fd);
}

void getFileAccessFlag(const char * __filePath, const int __checkedFd, const int __outputFd)
{
    if (!__filePath || (*__filePath) == '\0') { fatal("getFileAccessFlag() -> Empty __filePath!"); }

    char fileName[128] = {0};
    ssize_t writeReturnVal = 0;
    sprintf(fileName, "File: [%s] fd = [%d], access flag is:\n", __filePath, __checkedFd);

    if (write(__outputFd, fileName, strlen(fileName)) == -1)
    {
        errExit("getFileAccessFlag() -> write(__outputFd, fileName, strlen(fileName))");
    }

    if (IS_READ_ONLY(__checkedFd))  { writeReturnVal = write(__outputFd, "<read-only> ", 13); }
    if (IS_WRITE_ONLY(__checkedFd)) { writeReturnVal = write(__outputFd, "<write-only> ", 14); }
    if (IS_READ_WRITE(__checkedFd)) { writeReturnVal = write(__outputFd, "<read-write> ", 14); }
    if (IS_APPEND(__checkedFd))     { writeReturnVal = write(__outputFd, "<append> ", 10); }
    if (IS_CREATE(__checkedFd))     { writeReturnVal = write(__outputFd, "<create> ", 10); }
    if (IS_EXCL(__checkedFd))       { writeReturnVal = write(__outputFd, "<excl> ", 8); }
    if (IS_NOCTTY(__checkedFd))     { writeReturnVal = write(__outputFd, "<noctty> ", 10); }
    if (IS_TRUNC(__checkedFd))      { writeReturnVal = write(__outputFd, "<trunc> ", 9); }
    if (IS_ASYNC(__checkedFd))      { writeReturnVal = write(__outputFd, "<async> ", 9); }
    if (IS_NONBLOCK(__checkedFd))   { writeReturnVal = write(__outputFd, "<non-block> ", 13); }
    if (IS_SYNC(__checkedFd))       { writeReturnVal = write(__outputFd, "<sync> ", 8); }

    writeReturnVal = write(__outputFd, "\n", 1);

    if (writeReturnVal == -1) { errExit("write() out put file flag failed..."); }
}

void modifyFileFlag(const int __fd, enum FlagSetting __flagSet, const int __flag)
{
    int accessFlag = fcntl(__fd, F_GETFL);
    if (accessFlag == -1)
    {
        errExit("modifyFileFlag() -> fcntl(__fd, F_GETFL)");
    }

    switch (__flagSet)
    {
        // 增加新标志位，将新标志位与原标志位做或等于运算
        case INCREASE:
            accessFlag |= __flag;
            if (fcntl(__fd, F_SETFL, accessFlag) == -1)
            {
                errExit("modifyFileFlag(INCREASE) -> fcntl(__fd, F_SETFL, accessFlag)");
            }

            break;

        // 减少标志位，将要减少的标志位取反后再与原标志位做与等于运算
        case REDUCE:      
            accessFlag &= (~__flag);

            if (fcntl(__fd, F_SETFL, accessFlag) == -1)
            {
                errExit("modifyFileFlag(REDUCE) -> fcntl(__fd, F_SETFL, accessFlag)");
            }

            break;

        default:
            fatal("modifyFileFlag(..., [__flagSet], ...): invalid flagset!");
    }
}

off_t getFileCurrentOffset(const int __fd)
{
    off_t fileOffset = 0L;
    if ((fileOffset = lseek(__fd, 0, SEEK_CUR)) == -1) 
    { 
        errExit("getFileCurrentOffset() -> lseek(__fd, 0, SEEK_CUR)"); 
    }

    return fileOffset;
}