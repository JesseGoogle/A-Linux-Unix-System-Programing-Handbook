#include <sys/stat.h>
//#include <sys/uio.h>
#include <fcntl.h>

//#include "../../../../include/tlpi_hdr.h"
#include "./5-7.h"

#define BUFFER_SIZE 1024
#define IOV_STRUCT_AMOUNT 3

int main(int argc, char const *argv[])
{
    int fd = 0;

    int accessMode = O_RDWR | O_CREAT | O_TRUNC;
    mode_t accessPower = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH;

    MyIovec iov[IOV_STRUCT_AMOUNT];

    struct stat myFileState = {0};
    char myBuffer[BUFFER_SIZE] = {0};
    long int x = 0L;

    ssize_t writeBytes = 0L, toRequired = 0L;

    if (argc != 2 || (strcmp(argv[1], "--help") == 0))
        usageErr("%s <write-file>\n", argv[0]);

    if ((fd = open(argv[1], accessMode, accessPower)) == -1)
        errExit("open(argv[1], accessMode, accessPower)"); 

    myFileState = getFileState(fd);
    sprintf(myBuffer, "This buffer will writed to %s file.", argv[1]);
    x = 0x11FFFFFFFFFFFFFFL;

    iov[0].resourcePointer = &myFileState;
    iov[0].resourceLength  = sizeof(struct stat); 
    toRequired += sizeof(struct stat);

    iov[1].resourcePointer = &myBuffer;
    iov[1].resourceLength  = strlen(myBuffer);
    toRequired += strlen(myBuffer);

    iov[2].resourcePointer = &x;
    iov[2].resourceLength  = sizeof(long int);
    toRequired += sizeof(long int);


    if ((writeBytes = myWritev(fd, iov, IOV_STRUCT_AMOUNT)) == -1)
    { 
        close(fd);
        errExit("myWritev(fd, iov, IOV_STRUCT_AMOUNT)"); 
    }
    
    if (writeBytes < toRequired)
    {
        fprintf(stderr, "read Fewer bytes than requested.\n");
    }

    printf("Total bytes requested: %ld bytes; Reality write: %ld bytes.\n", toRequired, writeBytes);

    close(fd);

    return EXIT_SUCCESS;
}
