#include <setjmp.h>

#include "../../include/tlpi_hdr.h"

static jmp_buf  jumpBuffer;

static void f2(void)
{
    longjmp(jumpBuffer, 2);
}

static void f1(const int __argc)
{
    if (__argc == 1) { longjmp(jumpBuffer, 1); }

    f2();
}

int main(int argc, char const *argv[])
{
    system("clear");

    /**
     * 在 switch 语句内的表达式设立跳转目标, 
     * 之后所有的 longjmp 函数调用都会回到此处, 
     * 第一次调用 setjmp() 函数后, 函数返回 0
    */
    switch (setjmp(jumpBuffer))     
    {
    case 0:
        printf("Calling f1() after initial setjmp().\n");
        
        /**
         * 调用 f1() 根据程序参数个数会有不同的结果:
         * 
         * 1. argc 为 1, 直接在 f1 内调用 longjmp(jumpBuffer, 1), 
         *    解开由 f1() 产生的栈帧后, 回到 switch 表达式内, setjmp() 函数返回 1, 跳转去 case 1
         * 
         * 2. argc 大于 1, 在 f1() 内调用 f2(), 
        *     在 f2() 内 调用 longjmp(jumpBuffer, 2), 
        *     以此解开 f2() f1() 的栈帧后, 跳转去 switch 表达式内, setjmp() 函数返回 2, 跳转去 case 2
        */
        f1(argc);

        break;

    case 1:
        printf("Just back from f1()\n");

        break;

    case 2:
        printf("Just back from f2()\n");

        break;
    }
    
    return EXIT_SUCCESS;
}
