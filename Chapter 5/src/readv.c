#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>

#include "../../include/tlpi_hdr.h"

#define BUFFER_SIZE 1024
#define IOV_STRUCT_AMOUNT 3

int main(int argc, char const *argv[])
{
    system("clear");

    int fd = 0;

    int accessMode = O_RDONLY;
    mode_t accessPower = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH;

    struct iovec iov[IOV_STRUCT_AMOUNT];

    struct stat myFileState = {0};
    char myBuffer[BUFFER_SIZE] = {0};
    long int x = 0;

    ssize_t readBytes = 0L, toRequired = 0L;

    if (argc != 2 || (strcmp(argv[1], "--help") == 0))
        usageErr("%s <write-file>\n", argv[0]);

    if ((fd = open(argv[1], accessMode, accessPower)) == -1)
        errExit("open(argv[1], accessMode, accessPower)");

    /**
     * 将要发送的数据和 iov 结构体数组一一绑定
    */
    iov[0].iov_base = &myFileState;
    iov[0].iov_len = sizeof(struct stat);

    toRequired += iov[0].iov_len;

    iov[1].iov_base = myBuffer;
    iov[1].iov_len  = 52;

    toRequired += iov[1].iov_len;

    iov[2].iov_base = &x;
    iov[2].iov_len = sizeof(long int);

    toRequired += iov[2].iov_len;

    if ((readBytes = readv(fd, iov, IOV_STRUCT_AMOUNT)) == -1)
        errExit("readv(fd, iov, IOV_STRUCT_AMOUNT)");


    showFileState(argv[1], &myFileState, STDOUT_FILENO);
    printf("\n%s\n", myBuffer);
    printf("%#lX\n", x);

    if (readBytes < toRequired) { fatal("read fewer bytes than requested."); }

    close(fd);

    return EXIT_SUCCESS;
}