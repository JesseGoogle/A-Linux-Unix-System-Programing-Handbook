/**
 * 本程序展示 
 *      clearenv(void) 
 *      int putenv(char * __string)  
 *      int setenv(const char * __name, const char * __value, int __overwrite);
 *      int unsetenv(const char * __name);
 * 这些 envp 函数族的用法
*/

#define _GNU_SOURCE

#include "../../include/tlpi_hdr.h"

extern char ** environ;

int main(int argc, char *argv[])
{
    int j;
    char ** ep;

    clearenv();     // 清理从父进程继承下来的环境变量

    for (j = 1; j < argc; j++)
    {
        /**
         * 从 argv 参数列表中设置环境变量，
         * 它接受格式为 变量名=变量值 的字符串格式来设置或修改环境变量。
         * 
         * putenv()不提供一个明确控制是否创建新变量或更新现有变量的标志。
         * 传递给它的字符串直接被解释为“设置或更新”操作，这意味着如果变量已经存在，它的值将被改变，而如果不存在，则会被新增
        */
        if (putenv(argv[j]) != 0) { errExit("main() -> putenv(%s)", argv[j]); }
    }
    
    /**
     * 它接受三个参数：环境变量名、环境变量值和一个标志（通常为 0 或者 1），
     * 如果标志为 1，则如果变量已经存在则会更新其值；
     * 如果为 0（或不指定，默认行为相当于 0），且变量已存在，则该调用不会生效，避免了意外地覆盖已存在的环境变量。
     * 
     * setenv() 会确保环境变量数组（environ）中的其他元素依然有效，
     * 且由于它可能会重新分配environ数组，因此它不是线程安全的，除非在调用前明确锁定了相关的全局数据。
    */
    if (setenv("GREET", "Hello world", 0) == -1) { errExit("main() -> setenv()"); }

    /**
     * 根据传入的环境变量值，
     * 删除 environ 中制定的环境变量。
    */
    unsetenv("BYE");

    for (ep = environ; *ep != NULL; ep++) { puts(*ep); }

    return EXIT_SUCCESS;
}
