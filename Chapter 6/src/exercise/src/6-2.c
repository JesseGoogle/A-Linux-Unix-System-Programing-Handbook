#include <setjmp.h>

#include "../../../include/tlpi_hdr.h"

/**
 * 一个简单的, 测试跳转至一个已经返回的函数的示例, 程序结果为:
 * 
 * Call function f1(), set jump position.
 * f1() -> array = 1 2 3 4 5 
 * -----------------------------------
 * Call function f2(), jump to f1().
 * -----------------------------------
 * f1() -> Jump from other place.
 * f1() -> array = 1990704912 32766 1990705224 32766 1990704912 
 * -----------------------------------
 * *** stack smashing detected ***: terminated
 * 已放弃 (核心已转储)
 * 
 * stack smashing detected 意味检测到堆栈损毁, 
 * 由于从 f2() 跳转后, f1() 函数已经返回, 
 * 所以属于 f1() 的栈帧已经不存在, 跳转恢复后, array 的值是未定义的.
*/

static jmp_buf jumpBuffer;

void showArray(int * __arr, size_t __arrSize);

void f1(void) 
{
    int array[] = {1, 2, 3, 4, 5};
 
    if (setjmp(jumpBuffer) == 0)
    {
        printf("Call function f1(), set jump position.\n"); 
    }
    else 
    {
        printf("f1() -> Jump from other place.\n");
    }
    printf("f1() -> array = ");
    showArray(array, sizeof(array) / sizeof(int));
    printSplitLine(35, '-', STDOUT_FILENO); putchar('\n');
}

void f2(void)
{
    printf("Call function f2(), jump to f1().\n"); 
    printSplitLine(35, '-', STDOUT_FILENO); putchar('\n');

    longjmp(jumpBuffer, 1);
}

int main(int argc, char const *argv[])
{
    system("clear");
    f1();
    f2();
    
    return EXIT_SUCCESS;
}

void showArray(int * __arr, size_t __arrSize)
{
    for (size_t index = 0; index < __arrSize; ++index)
    {
        printf("%d ", __arr[index]);
    }

    putchar('\n');
}
