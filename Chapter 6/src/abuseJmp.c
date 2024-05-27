#include <setjmp.h>

#include "../../include/tlpi_hdr.h"

/**
 * jmp 函数族虽然能轻松完成指令间的跳转, 
 * 但是有严格的限制,只能在如下语境中使用:
 * 
 *      1. 构成选择或迭代语句中的整个控制表达式 如: while (setjmp(jmp_buff)) {...}
 * 
 *      2. 作为一元操作符 ! (not) 的操作对象, 
 *         其最终表达式构成来选择或迭代语句的整个控制表达式, 如: if (!setjmp(jmp_buff)) {...}
 * 
 *      3. 作为独立的函数调用, 且没有嵌入到更大的表达式之中.
 * 
 *      4. 作为比较操作符的一部分, 另一操作对象必须是一个整数常量表达式, 
 *         且其最终表达式构成选择或迭代语句的整个控制表达式, 如: while (setjmp(jmp_buff) > 2) {...}
 * 
 * 还需要注意的是, 像下面的语句:
 *      int val = setjmp(jmp_buff);
 * 是不符合标准的, 之所以有这些限制, 
 * 是因为作为常规函数的 setjmp() 实现无法保证拥有足够信息来保存所有寄存器的值和封闭表达式中用到的临时栈的位置, 
 * 以便使用 longjmp() 实行跳转. 因此, 仅允许在足够简单且无需临时存储的表达式中调用 setjmp().
*/

/**
 * 程序演示一个错误使用 jmp 系列函数的例子, 
 * 但是目前测试下来,好像没啥问题, longjmp() 会根据 jumpBuffer 内的数据恢复当时栈的情况.
*/

static jmp_buf jumpBuffer;
static int f1CallTimes = 0;

void f1(void) 
{ 
    printf("Call function f1(), set jump position.\n"); 
    setjmp(jumpBuffer); 

    printf("Call f1() %d times.\n", ++f1CallTimes);
}

void f2(void)
{
    printf("Call function f2(), jump to f1().\n"); 

    longjmp(jumpBuffer, 1);
}

int main(int argc, char const *argv[])
{
    f1();
    f2();
    
    return EXIT_SUCCESS;
}
