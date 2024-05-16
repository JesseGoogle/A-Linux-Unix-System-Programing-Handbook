/**
 * 一个常用的错误处理函数的声明。
*/

#ifndef _ERROR_FUNCTION_H_
#define _ERROR_FUNCTION_H_

/**
 *  @brief            这个函数可以打印不同风格的分割线
 *  @param __len      分割线的长度
 *  @param __style    分割线的风格，如（------- ****** 或 ·········）
 *  
 *  @return           non-return 
*/
void printSplitLine(int __len, const char __style, FILE * __outFp);

/**
 * @brief 用于诊断系统调用发生的错误，
 *        会往 `stderr` 中发送错误消息，同时往消息尾部追加换行符。
 * 
 * @brief - 该函数会打印出与当前 `errno` 值相对应的错误文本，其中包括：
 * 
 * @brief      - 错误名（如 `EPERM`）
 * @brief      - 由 `strerror()` 返回的错误描述
 * @brief      - 由参数列表指定的格式化输出
 * 
 * @param __format 消息类型，和 `printf()` 用法相同
 * @param ...      可变参数列表
 * 
 * @return no return
*/
void errMessage(const char * __format, ...);

#ifdef __GNUC__

#define NORETURN __attribute__((__noreturn__))

#else

#define NORETURN

#endif // __GNUC__

/**
 * @brief 用来终止程序并生成核心转储文件（Core Dump）供 Debuger 调试，用法和 `errMessage()` 相似，
 *        但实现会根据情况选择退出方式：
 * 
 * @brief 1. 使用 `exit()` 函数退出
 * @brief 2. 使用 `abort()` 函数退出（在环境变量 `EF_DUMPCORE` 定义为非空字符串时）
 * 
 * @param __format 消息类型，和 `printf()` 用法相同
 * @param ...      可变参数列表
 * 
 * @return no return
*/
void errExit(const char * __format, ...) NORETURN;

/**
 * @brief 和 `errExit` 类似，但有两点不同：
 * 
 * @brief 1. 输出错误消息前，不会刷新 `stdout`
 * @brief 2. 使用 `_exit()` 而非 `exit()` 终止程序，
 *           这样就略去了对 `stdio` 缓冲区的刷新以及退出处理程序（exit handler）的调用
 * 
 * @param __format 消息类型，和 `printf()` 用法相同
 * @param ...      可变参数列表
 * 
 * @return no return
*/
void err_exit(const char * __format, ...) NORETURN;

/**
 * @brief 和 `errExit()` 类似，仅有一点不同：
 * @brief - `errExitEN()` 只会打印 errnum 参数中给定的错误号相对应的文本
 * 
 * @param __errNum 错误号
 * @param __format 消息类型，和 `printf()` 用法相同
 * @param ...      可变参数列表
 * 
 * @return no return
*/
void errExitEN(int __errNum, const char * __format, ...) NORETURN;

/**
 * @brief 用于诊断一般错误，也会在错误信息尾部追加换行符，
 *        在 stderr 上打印格式化输出，然后像 errExit() 那样终止程序。
 * 
 * @param __format 消息类型，和 `printf()` 用法相同
 * @param ...      可变参数列表
 * 
 * @return no return
*/
void fatal(const char * __format, ...) NORETURN;

/**
 * @brief 用于诊断命令行参数使用方面的错误，会在错误信息前插入 "Usage: "，
 *        然后输出错误信息并终止程序。
 * 
 * @param __errNum 错误号
 * @param __format 消息类型，和 `printf()` 用法相同
 * @param ...      可变参数列表
 * 
 * @return no return
*/
void usageErr(const char * __format, ...) NORETURN;

/**
 * @brief 和 `usageErr()` 函数相似，
 *        但错误诊断是针对特定程序的命令行参数。
 * 
 * @param __errNum 错误号
 * @param __format 消息类型，和 `printf()` 用法相同
 * @param ...      可变参数列表
 * 
 * @return no return
*/
void cmdLineErr(const char * __format, ...) NORETURN;

#endif // _ERROR_FUNCTION_H_