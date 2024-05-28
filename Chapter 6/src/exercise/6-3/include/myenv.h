#ifndef _MY_ENV_H_
#define _MY_ENV_H_

#include "../../../../../include/tlpi_hdr.h"

#define CREATE_OR_MODIFY TRUE
#define NO_OPERATION     FALSE

/**
 * 6-3 使用 getenv(), putenv() 函数, 来实现 setenv() 和 unsetenv() 函数.
 * 
 * 注意: unsetenv() 函数需要检测环境变量数组内是否重复定义了要删除的环境变量, 若存在这一情况,
 *      就需要全部删除.
*/

/**
 * @brief 应题目要求自行实现的 setenv() 函数, 
 *        对 __environ 字符串数组进行操作, 实现环境变量的修改.
 * 
 * @param __name    环境变量键
 * @param __value   环境变量值
 * @param __replace 环境变量操作标志位, 若值为 CREATE_OR_MODIFY, 则覆盖或创建新变量,
 *                  若值为 NO_OPERATION, 则不对环境变量做任何修改.
 * 
 * @return 返回 0 则代表创建或修改成功, 返回其他值则代表错误, 会有详细指示.
*/
int mySetenv(const char * __name, const char * __value, int __replace);

/**
 * @brief 根据 __name 来删除 __environ 字符串数组中的环境变量.
 * 
 * @param __name 环境变量键
 * 
 * @return 返回 0 则代表删除成功, 返回其他值则代表错误, 会有详细指示.
*/
int myUnsetenv(const char * __name);

#endif // _MY_ENV_H_