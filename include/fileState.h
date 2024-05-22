#ifndef _FILE_STATE_H_
#define _FILE_STATE_H_

#include "../include/tlpi_hdr.h"

#define FILE_STATE_STRING_LENGTH 256       // showFileState() 中，每一条文件字符属性串的长度
#define FILE_STATE_STRING_AMOUNT 8         // showFileState() 中，文件字符属性串的数量

/**
 * 一个枚举类型，用于表示文件访问标志位的增加或减少
*/
enum FlagSetting { INCREASE = 1, REDUCE = 0 };

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

/**
 * @brief 用文本的形式输出一个文件的具体访问模式
 * 
 * @param __filePath  要检测的文件路径
 * @param __checkedFd 要检测的文件描述符
 * @param __outputFd  要输出结果的文件
 * 
 * @return no return
*/
void getFileAccessFlag(const char * __filePath, const int __checkedFd, const int __outputFd);

/**
 * @brief 修改一个文件的访问标志位
 * 
 * @param __fd          要修改访问标志位的文件
 * @param __flagSet     增加/减少 标志位
 * @param __flag        要增加/减少的标志位
 * 
 * @return no return
*/
void modifyFileFlag(const int __fd, enum FlagSetting __flagSet, const int __flag);

/**
 * @brief 获取一个文件当前的指针偏移量
 * 
 * @param __fd    要访问的文件
 * 
 * @return 当前文件的偏移量
*/
off_t getFileCurrentOffset(const int __fd);

#endif //_FILE_STATE_H_