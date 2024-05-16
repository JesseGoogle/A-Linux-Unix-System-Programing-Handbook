#include "./holeFileCopy.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

void checkArgc(const int __argc, const char * __programName)
{
    if (__argc != 3)
    {
        cmdLineErr("\n[%s] <source-file> <dest-file>\n\n", __programName);
    }
}

struct stat getFileState(const int __srcFd, const int __destFd)
{
    struct stat srcState;

    if (fstat(__srcFd, &srcState) == -1)
    {
        close(__srcFd);
        close(__destFd);
        errExit("getFileState() -> fstat(__srcFd, ...)");
    }

    return srcState;
}

void showFileState(const char * __fileName, const struct stat * __fileState, FILE * __outputFp)
{
    if (!__fileState)
    {
        errExit("Empty stat struct");
    }

    printSplitLine(45, '-', __outputFp);

    fprintf(
                __outputFp,
                "File Path: %s\nDevice ID for this file: %lu\nInode No = %lu\nNlink count = %lu\nFile access mode: %u\n",
                __fileName,
                __fileState->st_dev, __fileState->st_ino, 
                __fileState->st_nlink, __fileState->st_mode 
        );

    fprintf(
               __outputFp,
               "User ID: %u\nGroup ID: %u\nDevice type ID: %lu\nFile reality size = %ld Bytes\nFile block size = %ld Bytes\nUse %ld blocks\n",
               __fileState->st_uid, __fileState->st_gid, __fileState->st_rdev,
               __fileState->st_size, __fileState->st_blksize, __fileState->st_blocks
        );

    fprintf(
                __outputFp,
                "Last access time: %sLast modify time: %sLast state change time: %s",
                ctime(&__fileState->st_atime),
                ctime(&__fileState->st_mtime),
                ctime(&__fileState->st_ctime)
    );

    printSplitLine(45, '-', __outputFp);
}