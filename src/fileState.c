#include <sys/stat.h>
#include <time.h>

#include "../include/fileState.h"

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