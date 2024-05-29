//#include <unistd.h> /* brk() sbrk() 系统调用所在 */

#include "../../include/tlpi_hdr.h"

/*
    brk() 和 sbrk() 函数主要用于内存管理，尤其是在低级别的程序设计中，它们与堆内存分配有关。

    brk() 系统调用用于调整进程的堆段结束地址（program break），从而改变进程的堆大小。
    具体来说，它允许程序动态地增加或减少可用堆空间。
    该函数接受一个指针参数，指向希望设置的新程序中断点（即堆的末尾）。
    如果成功，它会返回新的程序中断点地址；若失败，则返回 -1。

    sbrk()

    sbrk() 函数是 brk() 的一个更高级的接口，它也用于调整堆的大小，
    但与 brk() 不同，sbrk() 接受一个增量值作为参数，表示希望增加或减少的内存字节数。
    如果增量为正，sbrk() 将堆扩展相应的大小；
    若为负，则尝试缩小堆。返回值为调整前的程序中断点地址，即旧的堆顶位置。

    但实际编程中, 很少使用这两个函数, 它们通常会被 malloc 函数族调用.
*/

int main(int argc, char const *argv[])
{
    system("clear");

    /**
     * 使用 sbrk(0) 可以获取当前的堆边界地址.
    */
    void * currentBreakPtr = sbrk(0);
    void * afterIncreaseBreakPtr = NULL;
    size_t offset = 0UL;

    if (currentBreakPtr == (void *) -1) { errExit("sbrk(0)"); }
 
    printf(
            "Process [%s] PID = %d, current program break address = %p\n", 
            argv[0], getpid(), currentBreakPtr
        );

    printf("Increase 1024 Bytes for Heap Segment.\n");

    /*
        若 sbrk() 调用成功, 
        则返回前一个堆边界地址, 也可以理解为新分配内存地址的起始地址.
    */
    if ((afterIncreaseBreakPtr = sbrk(1024)) == (void *)-1) { errExit("sbrk(1024)"); }

    
    printf(
                "After sbrk(1024), we increase %lu bytes to heap memory.\ncurrent program break address = %p.\n", 
                (char *)sbrk(0) - (char *)afterIncreaseBreakPtr, sbrk(0)
        );

    return EXIT_SUCCESS;
}
