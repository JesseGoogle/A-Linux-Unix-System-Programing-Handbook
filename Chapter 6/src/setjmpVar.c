/**
 * 本程序测试, 编译器优化是如何受 longjmp() 函数影响的。
*/

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

static jmp_buf jumpBuffer;

void doJump(int __n, int __r, int __v);

int main(int argc, char const *argv[])
{
    int nvar = 111;           // 分配在栈中
    register int rvar = 222;  // 分配在 CPU 寄存器中 (如果可能的话)
    volatile int vvar = 333;  // 被 volatile 修饰, 编译器不会对它进行优化

    /**
     * 第一次调用 setjmp() 没什么好说的, 几个 var 被赋值, 然后调用 doJump() 输出:
     * 
     *      Inside doJump(), nvar = 777, rvar = 888, vvar = 999
     * 
     * 随后调用 longjmp(jumpBuffer, 1) 回到 if 表达式内, 
     * 由于 jumpBuffer 保存了第一次调用 setjmp() 时的栈信息, 
     * 所以 nvar rvar 都回到了第一次赋值时的样子, 而 vvar 被 volatile 修饰, 依旧保持 999 的值
     * 且此时 setjmp() 返回 1, 会跳转到 else 语句块, 输出:
     * 
     *      After longjmp(), nvar = 111, rvar = 222, vvar = 999
    */
    if (setjmp(jumpBuffer) == 0) 
    { 
        nvar = 777;
        rvar = 888;
        vvar = 999;

        doJump(nvar, rvar, vvar);
    }
    else
    {
        printf("After longjmp(), nvar = %d, rvar = %d, vvar = %d\n", nvar, rvar, vvar);
    }

    return EXIT_SUCCESS;
}

void doJump(int __n, int __r, int __v)
{
    printf("Inside doJump(), nvar = %d, rvar = %d, vvar = %d\n", __n, __r, __v);

    longjmp(jumpBuffer, 1);
}