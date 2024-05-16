#ifndef _HOLE_FILE_COPY_H_
#define _HOLE_FILE_COPY_H_

#include "../../../../include/tlpi_hdr.h"

#ifndef BUFFER_SIZE

#define BUFFER_SIZE 1024

#endif // BUFFER_SIZE

#define SEEK_DATA 3
#define SEEK_HOLE 4

/**
 * @brief 检查命令行参数
*/
void checkArgc(const int __argc, const char * __programName);

/**
 * @brief 传入文件描述符然后获取源文件的详细属性。
*/
struct stat getFileState(const int __srcFd, const int __destFd);

/**
 * @brief 发送一个文件的详细属性到 __outpuFd 文件描述符所代表的文件中。
 * 
 * @param __fileState   一个文件的详细结构体属性
 * @param __outputFp    要将这些信息输出到哪个文件
 * 
 * @return no return
*/
void showFileState(const char * __fileName, const struct stat * __fileState, FILE * __outputFp);

#endif // _HOLE_FILE_COPY_H_