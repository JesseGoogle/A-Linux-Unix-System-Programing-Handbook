#include <fcntl.h>
#include <sys/stat.h>

#include "../../../include/tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    /**
     * 打开或创建一个文件 argv[1]，
     * 在这之前要截断文件，并且该文件可以被该程序读写，
     * 仅文件拥有者可以打开文件读写。
    */
    int fd = open(
        argv[1],
        O_RDWR | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR
    );

    int fd2 = dup(fd);      // 拷贝文件描述符 fd，产生新文件描述符 fd2

    /**
     * 以可读可写形式打开文件
    */
    int fd3 = open(argv[1], O_RDWR);

    write(fd, "Hello, ", 7);    // 写入 "Hello, "，fd 文件偏移量此时为 7，文件内容为 "Hello, "

    sleep(2);

    write(fd2, "world", 5);     // 写入 "world"，fd2 文件偏移量为 12，文件内容为 "Hello, world"

    sleep(2);

    /**
     * 把 fd2 的偏移量拨回开头，由于 fd2 由 dup(fd) 复制而来，
     * fd 和 fd2 都代表同一个文件，所以 fd 的偏移量也回到了开头。
    */
    lseek(fd2, 0, SEEK_SET);

    write(fd, "HELLO", 5);  // 此时再往 fd 写入，就会覆盖前 5 个字节，文件内容为 "HELLO, world"

    sleep(2);

    /**
     * 由于 fd3 是由 open(argv[1], O_RDWR) 打开的，
     * 这是对 fd3 的第一次写入，所以会覆盖前 6 个字节，文件内容为 "Giddy world"。
    */
    write(fd3, "Gidday", 6);

    sleep(2);

    return EXIT_SUCCESS;
}

/**
 * 文件 argv[1] 的内容为：
 * 
 * Gidday world
*/