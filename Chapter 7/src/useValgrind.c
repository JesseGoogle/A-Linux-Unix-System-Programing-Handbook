/**
 * 本程序旨在练习基本的 Valgrind 使用方法, 比如 
 *      
 *      valgrind --leak-check=full <execution-file>
 * 
 * 程序会制造一些内存泄漏, 或者非法访问的情境, 供 valgrind 检测.
*/

/*
    valgrind 的检测结果如下:

    valgrind --leak-check=full ./bin/useValgrind

    ==27002== Memcheck, a memory error detector
    ==27002== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
    ==27002== Using Valgrind-3.21.0 and LibVEX; rerun with -h for copyright info
    ==27002== Command: ./bin/useValgrind
    ==27002== 
    ==27002== Invalid write of size 4
    ==27002==    at 0x109181: function (in /home/jesse/桌面/linuxSystemProgram/Chapter 7/bin/useValgrind)
    ==27002==    by 0x1091A6: main (in /home/jesse/桌面/linuxSystemProgram/Chapter 7/bin/useValgrind)
    ==27002==    Address 0x4a7d240 is 0 bytes after a block of size 512 alloc'd
    ==27002==    at 0x4845828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==27002==    by 0x109168: function (in /home/jesse/桌面/linuxSystemProgram/Chapter 7/bin/useValgrind)
    ==27002==    by 0x1091A6: main (in /home/jesse/桌面/linuxSystemProgram/Chapter 7/bin/useValgrind)
    ==27002== 
    ==27002== 
    ==27002== HEAP SUMMARY:
    ==27002==     in use at exit: 512 bytes in 1 blocks
    ==27002==   total heap usage: 1 allocs, 0 frees, 512 bytes allocated
    ==27002== 
    ==27002== 512 bytes in 1 blocks are definitely lost in loss record 1 of 1
    ==27002==    at 0x4845828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==27002==    by 0x109168: function (in /home/jesse/桌面/linuxSystemProgram/Chapter 7/bin/useValgrind)
    ==27002==    by 0x1091A6: main (in /home/jesse/桌面/linuxSystemProgram/Chapter 7/bin/useValgrind)
    ==27002== 
    ==27002== LEAK SUMMARY:
    ==27002==    definitely lost: 512 bytes in 1 blocks
    ==27002==    indirectly lost: 0 bytes in 0 blocks
    ==27002==      possibly lost: 0 bytes in 0 blocks
    ==27002==    still reachable: 0 bytes in 0 blocks
    ==27002==         suppressed: 0 bytes in 0 blocks
    ==27002== 
    ==27002== For lists of detected and suppressed errors, rerun with: -s
    ==27002== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)

    具体解析详见: https://github.com/JesseGoogle/A-Linux-Unix-System-Programing-Handbook/discussions/15#discussioncomment-9602760
*/

#include <stdlib.h>
#include <stdio.h>

void function(int __size);

void function(int __size)
{
    int * ptr = (int *)malloc(__size * sizeof(int));

    ptr[__size] = 114514;       // 一次非法访问

    // 函数调用结束时没有 free()
}

int main(int argc, char const *argv[])
{
    function(128);

    return EXIT_SUCCESS;
}
