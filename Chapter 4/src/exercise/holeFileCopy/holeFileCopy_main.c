#include "./holeFileCopy.h"

#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    system("clear");

    // 检查命令行参数是否符合要求
    checkArgc(argc, argv[0]);

    const char * sourceFile = argv[1];  // 源文件路径
    const char * destFile   = argv[2];  // 目标文件路径

    int sourceFd = -1;                  // 源文件描述符
    int sourceFlag = O_RDONLY;          // 源文件读写权限：只读

    int destFd   = -1;                                  // 目标文件描述符
    int destFlag = (O_WRONLY | O_TRUNC | O_CREAT);      // 目标文件读写权限：只写，若文件同名则清空文件，若文件不存在则创建之
    mode_t destPower = (S_IRUSR | S_IWUSR);             // 目标文件访问权限：用户可读写

    /**
     * 打开源文件和目标文件并检查返回值
    */
    if ((sourceFd = open(sourceFile, sourceFlag)) == -1) { errExit("source file open()"); }

    if ((destFd = open(destFile, destFlag, destPower)) == -1) 
    { 
        close(destFd); 
        errExit("dest file open()"); 
    }

    // 获取源文件的详细属性
    struct stat srcFileState = getFileState(sourceFd, destFd);
    struct stat destFileState = getFileState(destFd, sourceFd);

    showFileState(sourceFile, &srcFileState, stdout);
    showFileState(destFile, &destFileState, stdout);

    char byterBuffer[BUFFER_SIZE] = {""};   // 设置缓冲区

    off_t offset = 0L;  // 文件读写偏移量

    /*文件读写字节数*/
    ssize_t bytesWrite = 0L, bytesRead = 0L;

    off_t nextReadPos = 0L;     // 下一个要读取的位置
    off_t holeLength  = 0L;     // 当前空洞的长度

    /**
     * 当偏移量小于文件大小的时候进入循环。
    */
    while (offset < srcFileState.st_size)
    {
        /**
         * SEEK_DATA 宏，在 lseek 函数中可以让文件指针偏移到下一个可读数据块中，
         * 这对于稀疏文件的读写很有用。
        */
        if ((nextReadPos = lseek(sourceFd, offset, SEEK_DATA)) == -1)
        {
            perror("lseek to data");
            break;
        }

        holeLength = nextReadPos - offset;  // 计算空洞的大小

        if (holeLength > 0) // 如真的存在空洞
        {
            /*
                也让目标文件的文件指针偏移源文件空洞大小的字节数，
                这样就能在目标文件上制造空洞。
            */
            if (lseek(destFd, holeLength, SEEK_CUR) == -1)
            {
                perror("lseek to creat holefile in destination");
                break;
            }
        }

        /*空洞偏移完毕后读取数据块到缓冲区*/
        if ((bytesRead = read(sourceFd, byterBuffer, sizeof(byterBuffer))) == -1)
        {
            perror("read(sourceFd, ...)");
            break;
        }
        else if (bytesRead > 0) // 若读到了数据
        {
            // 写入目标文件
            if ((bytesWrite = write(destFd, byterBuffer, bytesRead)) != bytesRead)
            {
                perror("write(destFd, ...)");
                break;
            }

            offset += bytesRead;    // 累加读取的字节数
        }
        else { break; }
    }

    close(sourceFd);
    close(destFd);

    if (offset != srcFileState.st_size) // 在程序最后检测读取的数据和文件实际大小是否相等
    {   
        fatal("Error during copy, not all data was transferred.\n");
    }

    return EXIT_SUCCESS;
}
