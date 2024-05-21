#include <fcntl.h>
#include <sys/stat.h>

#include "../../../include/tlpi_hdr.h"

/**
 * 编写一个程序，使用 O_APPEND 以及 O_WRONLY 宏打开一个已经存在的文件，
 * 并将文件偏移量置于文件起始处，再写入数据。数据会显示在文件的哪个位置？
*/

int main(int argc, char const *argv[])
{
    system("clear");

    int     fd          = 0;
    int     accessMode  = O_WRONLY | O_APPEND;
    mode_t  accessPower = S_IWUSR | S_IRUSR;

    ssize_t writtenBytes = 0L;

    if ((fd = open(argv[1], accessMode, accessPower)) == -1) 
    { 
        errExit("open(argv[1], accessMode, accessPower)"); 
    }

    off_t currentOffset = lseek(fd, 0, SEEK_SET);   // 将文件偏移量移到文件头
    struct stat fileState = getFileState(fd);

    /*调试结果显示调用成功*/
    printf(
                "After open(argv[1], O_WRONLY | O_APPEND, accessPower)\nFile [%s] offset = [%ld] %s to file reality size.\n",
                argv[1], currentOffset, (currentOffset == fileState.st_size) ? "equal" : "not equal"
        );

    /**
     * 但，文件是使用了 O_APPEND 标志位打开的，
     * 每次调用 write() 都会自动偏移到文件末尾，无视之前的 lseek() 操作。
    */
    if ((writtenBytes = write(fd, "\nProgram 5-2 write string here.", 31)) == -1)
    {
        errExit("write(fd, ""\nProgram 5-2 write string here."", 31)");
    }

    close(fd);
    
    return EXIT_SUCCESS;
}
