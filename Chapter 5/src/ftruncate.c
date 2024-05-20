#include <sys/stat.h>
#include <fcntl.h>

#include "../../include/tlpi_hdr.h"

#ifndef USEPATH_OR_FD

#define USEPATH_OR_FD FALSE

#endif

int main(int argc, char const *argv[])
{
    system("clear");

    int fd = 0;

    const char * sourceFile = argv[1];
    int accessMode = O_RDWR;
    mode_t accessPower = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH;

    if (argc < 2 || (strcmp(argv[1], "--help") == 0))
    {
        usageErr("[%s] <source-file>\n", argv[0]);
    }

    if ((fd = open(sourceFile, accessMode, accessPower)) == -1)
    {
        errExit("open(argv[1], accessMode, accessPower)");
    }

    struct stat logState = getFileState(fd);

    printf("Before turncate() or fturncat() File [%s] attribution: \n", sourceFile);

    showFileState(sourceFile, &logState, STDOUT_FILENO);

    off_t fileSizeBeforEnlarge = logState.st_size;

#if USEPATH_OR_FD

    truncate(sourceFile, logState.st_size + 1024);

#else

    ftruncate(fd, fileSizeBeforEnlarge + 1024);

#endif

    printf("After turncate() or fturncat() File [%s] attribution: \n", sourceFile);

    logState = getFileState(fd);

    showFileState(sourceFile, &logState, STDOUT_FILENO);

    printf("We will enlarge file [%s] by [%ld] Bytes.\n", sourceFile, logState.st_size - fileSizeBeforEnlarge);

    close(fd);

    return EXIT_SUCCESS;
}
