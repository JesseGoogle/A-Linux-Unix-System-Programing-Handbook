#ifndef _FILE_STATE_H_
#define _FILE_STATE_H_

#include "../include/tlpi_hdr.h"

/**
 * @brief 传入文件描述符然后获取源文件的详细属性。
*/
struct stat getFileState(const int __fd);

/**
 * @brief 发送一个文件的详细属性到 __outpuFd 文件描述符所代表的文件中。
 * 
 * @param __fileName    文件名或文件路径
 * @param __fileState   一个文件的详细结构体属性
 * @param __fd          要将这些信息输出到哪个文件
 * 
 * @return no return
*/
void showFileState(const char * __fileName, const struct stat * __fileState, const int __fd);

#endif //_FILE_STATE_H_