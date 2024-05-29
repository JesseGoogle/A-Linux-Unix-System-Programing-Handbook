#include "../../include/tlpi_hdr.h"

#include <math.h>

/**
 * 该程序旨在验证 free() 函数对堆内存边界的影响, 
 * 程序分配来多块内存后, 根据可选命令行参数来释放全部或其中一部分, 
 * 程序参数如下, <> 为必要, [] 为可选.
 * 
 *      free_and_sbrk <num-allocs> <block-size> [free-step] [min - max]
 * 
 * 1. <num-allocs> 分配的内存块数量, 不得超过 MAX_ALLOCS
 * 2. <block-size> 每个内存块分配的大小
 * 3. [free-step]  释放内存块的步长, 默认为 1
 * 4. [min - max]  释放内存块的范围 memoryBuffer[min] ~ memoryBuffer[max], 默认为 1 - MAX_ALLOCS
 * 
*/

#define MAX_ALLOCS 1000000
#define SPLIT_LINE_LENGTH 64

int main(int argc, char const *argv[])
{
    system("clear");

    putchar('\n');

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s <num-allocs> <block-size> [free-step] [min - max]\n", argv[0]);
    }

    size_t index = 0;
    /**
     * 在栈上开辟一个长 MAX_ALLOCS * sizeof(char *) 字节, 
     * 每一个元素都是指针类型的指针数组.
    */
    char * memoryBuffer[MAX_ALLOCS] = {0};

    /**
     * 释放内存的步长 (可选参数), 
     * 代表每隔 (freeStep - 1) 释放一块已经分配的内存, 默认为 1. 
    */
    int freeStep = 0;

    /**
     * 释放内存块的范围, 如 freeMin = 1, freeMax = 128,
     * 就意味着释放指针数组 memoryBuffer 下标从 0 ~ 127 的所有指向的内存块, 
     * 如果没有输入该参数就释放所有的内存块.
    */
    int freeMin = 0, freeMax = 0;

    long int blockSize = getInt(argv[2], GN_GT_0, "<block-size>");      // 单块内存的大小
    long int numAllocs = getInt(argv[1], GN_GT_0, "<num-allocs>");      // 要分配的内存块数量

    freeStep = (argc > 3) ? (getInt(argv[3], GN_GT_0, "<free-step>")) : 1;
    freeMin  = (argc > 4) ? (getInt(argv[4], GN_GT_0, "min")) : 1;
    freeMax  = (argc > 5) ? (getInt(argv[5], GN_GT_0, "max")) : MAX_ALLOCS;

    if (numAllocs > MAX_ALLOCS)     // 检查 numAllocs 参数, 不得超过 MAX_ALLOCS
    { 
        cmdLineErr("[%d] is too big, max alloc value = [%d].\n\n", numAllocs, MAX_ALLOCS); 
    }

    printSplitLine(SPLIT_LINE_LENGTH, '-', STDOUT_FILENO); puts("");
    printf(
            "Alloc %ld memory block, each block size = %ld\nAlloc size = %lf Mbytes, free stap = %d\n\n", 
            numAllocs, blockSize, (double)(numAllocs * blockSize / pow(1024, 3)), freeStep
        );
    printf(
            "Range of free(): from memoryBuffer[%d] to memoryBuffer[%d]\n",
            freeMin - 1, freeMax - 1
        );
    printSplitLine(SPLIT_LINE_LENGTH, '-', STDOUT_FILENO); puts("\n");
    

    printf("Before malloc(), initial program break = [%p]\n\n", sbrk(0));

    for (index = 0; index < numAllocs; ++index)
    {
        if ((memoryBuffer[index] = (char *)malloc(blockSize)) == NULL)
        {
            errExit("memoryBuffer[index] = malloc(blockSize): Faild to alloc memory.");
        }
    }

    printf("After malloc(),  initial program break = [%p]\n\n", sbrk(0));

    for (index = freeMin - 1; index < freeMax; index += freeStep)
    {
        free(memoryBuffer[index]);
    }

    printf("After free(),    initial program break = [%p]\n\n", sbrk(0));

    printSplitLine(SPLIT_LINE_LENGTH, '-', STDOUT_FILENO); puts("");
    
    puts("Done.");

    return EXIT_SUCCESS;
}
