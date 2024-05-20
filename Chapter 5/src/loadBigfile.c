
#define _FILE_OFFSET_BITS 64

/**
 * 通常情况下，描述文件偏移量的 off_t 类型是一个 4 字节的有符号长整形（signed long int）,
 * 毕竟需要 -1 来表示错误，此类型最多为 2 GB 大小的文件执行偏移，但当前的文件很多都已经超过 2 GB，
 * 所以就需要更长的类型来表示偏移量，启用 _FILE_OFFSET_BITS 宏，把相关类型全部改成 64 位的。
 * 这样就能读写最大为 8192 PB 的文件（这世上应该没有容器能装下那么大的文件吧。。。。） 
 * 
 * 所以，此程序的目标就是打开一个大文件（超过 2 GB），
 * 然后在第 1919810 字节处往后写入 114514 个字节的数据。
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <math.h>

#include "../../include/tlpi_hdr.h"

#define WRITE_POSITION  1919810
#define BUFFER_BYTES    (ssize_t)114514

/**
 * @brief 在不截断文件且不偏移文件指针的情况下，将文件内的所有数据抹掉。
 * 
 * @param __fd          文件描述符
 * @param __fileSize    文件实际大小
 * 
 * @return no return
*/
void cleanFileData(const int __fd, const long int __fileSize);

int main(int argc, char const *argv[])
{
    if (system("clear") != 0) { errExit("system()"); }

#if false
    printf(
            "Max value of type long int = %d, Equals %0.f GB.\n", 
            INT_MAX, INT_MAX / pow(1024, 3)
        );
    
    printf(
            "Max value of type long long int = %lld, Equals %.4f GB, %.4f TB, %.4f PB.\n", 
            LLONG_MAX, LLONG_MAX / pow(1024, 3), LLONG_MAX / pow(1024, 4), LLONG_MAX / pow(1024, 5)
        );
#endif

    if (argc < 2 || (strcmp(argv[1], "--help") == 0)) { usageErr("[%s] <large-file> (over 2 GB)\n", argv[0]); }

    int largeFd = 0;
    int accessMode = O_RDWR;
    mode_t accessPower = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH;
    struct stat largeFileState = {0};
    char buffer[BUFFER_BYTES] = {0};

    if ((largeFd = open(argv[1], accessMode, accessPower)) == -1) { errExit("open(argv[1], accessMode, accessPower)"); }
    
    largeFileState = getFileState(largeFd);
    cleanFileData(largeFd, largeFileState.st_size);

    if ((largeFileState.st_size / pow(1024, 3)) <= 2) 
    { 
        close(largeFd); 
        usageErr("file [%s] is too small (%ld Bytes), over 2 GB please!\n", argv[1], largeFileState.st_size); 
    }

    memset(buffer, 1, sizeof(buffer));

    if (lseek(largeFd, WRITE_POSITION, SEEK_SET) < WRITE_POSITION) { usageErr("fewer file size.....\n"); }

    if (write(largeFd, buffer, BUFFER_BYTES) == -1) { errExit("write(largeFd, buffer, BUFFER_BYTES)"); }

    close(largeFd);

    return EXIT_SUCCESS;
}

void cleanFileData(const int __fd, const long int __fileSize)
{
    char *tempBuffer = (char *)calloc(__fileSize, 1);
    off_t offset = 0L;
    ssize_t bytesWritten = 0L;

    if (!tempBuffer) 
    { 
        close(__fd); 
        errExit("(char *)calloc(__fileSize, 1) Failed Allocate!"); 
    }

    while (offset < __fileSize)
    {
        bytesWritten = pwrite(__fd, tempBuffer + offset, __fileSize - offset, offset);

        if (bytesWritten == -1)
        {
            free(tempBuffer);
            close(__fd);
            errExit("pwrite(__fd, tempBuffer + offset, __fileSize - offset, offset)"); 
        }

        offset += bytesWritten;
    }

    free(tempBuffer);
}