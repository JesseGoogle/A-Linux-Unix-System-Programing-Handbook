#ifndef _MY_READV_WRITEV_H_
#define _MY_READV_WRITEV_H_

#include <sys/types.h>

#include "../../../../include/tlpi_hdr.h"

/**
 * 使用 read() write() 函数，
 * 以及 malloc 函数包实现 readv() 和 writev() （分散输入 集中输出）的功能，当前我设计的函数原型如下：
 * 
 *      ssize_t myReadv(const int __fd, const struct MyIovec *__iovec, const int __iovCount)
 *      ssize_t myWritev (const int __fd, const struct MyIovec *__iovec, const int __iovCount)
*/

/**
 * @brief 用于供 myReadv() 和 myWritev() 函数调用的结构体
*/
typedef struct __MyIovec
{
    void * resourcePointer;     // 数据所在
    size_t resourceLength;      // 数据长度

} MyIovec;

/**
 * @brief 用于执行集中输入`（Gather Input）`
 * 
 * @param __fd       要读取的文件
 * @param __iovec    一个 `MyIovec` 结构体数组，文件 `__fd` 中的数据会被读取到此处
 * @param __iovCount `MyIovec` 结构体数组的长度
 * 
 * @return 一共写入了多少字节，若返回 `-1` 则代表发生了错误
*/
ssize_t myReadv(const int __fd, const MyIovec *__iovec, const int __iovCount);

/**
 * @brief 用于执行分散输出`（Scatter Output）`
 * 
 * @param __fd       要读取的文件
 * @param __iovec    一个 `MyIovec` 结构体数组，会将结构体中的数据逐个写入到文件中
 * @param __iovCount `MyIovec` 结构体数组的长度
 * 
 * @return 一共写入了多少字节，若返回 `-1` 则代表发生了错误
*/
ssize_t myWritev(const int __fd, const MyIovec *__iovec, const int __iovCount);

#endif // _MY_READV_WRITEV_H_