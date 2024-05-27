#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    /**
     * getpid() 用来获取一个进程的进程号，返回值为 pid_t 类型，除少数系统级进程外（如 PID = 1 的 init 进程），
     * 程序与运行程序进程的进程号没有关系。
    */
    printf("The program %s, PID = %d.\n", argv[0], getpid());

    /**
     * 任何一个进程都有它自己的父进程，调用 getppid() 函数获取。
    */
    printf("And his PPID = %d", getppid());

    /**
     * 64 位机上可容纳的进程数会很大，约 400 万个
    */

    return 0;
}
