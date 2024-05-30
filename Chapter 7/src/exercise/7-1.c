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
 * 但练习 7-1 对程序稍作修改, 
 * 更加模块化的同时, 在每一次执行 malloc() 后打印出 program break 的当前值.
 * 
 * 指定一个较小的内存分配尺寸来运行该程序, 
 * 证明 malloc() 不会在每次被调用时都调用 sbrk() 去调整堆边界的位置, 而是周期性的分配大块内存, 并将其中的小片返回给调用者.
*/

#include "../../../include/tlpi_hdr.h"
#include <math.h>

#define MAX_ALLOCS          1000000             // 最大分配内存块数量
#define SPLIT_LINE_LENGTH   64                  // 文本分割线长度

#define GET_CURRENT_PROGRAM_BREAK sbrk(0)        // 获取当前堆边界地址

#define K_BYTES             pow(1024, 1)
#define M_BYTES             pow(1024, 2)

/**
 * @brief 本程序的参数列表结构体
*/
typedef struct __Argument_List
{
    /**
     * 释放内存的步长 (可选参数), 
     * 代表每隔 (freeStep - 1) 释放一块已经分配的内存, 默认为 1. 
    */
    int freeStep;

    /**
     * 释放内存块的范围, 如 freeMin = 1, freeMax = 128,
     * 就意味着释放指针数组 memoryBuffer 下标从 0 ~ 127 的所有指向的内存块, 
     * 如果没有输入该参数就释放所有的内存块.
    */
    int freeMin, freeMax;

    long int blockSize;     // 单块内存的大小
    long int numAllocs;     // 要分配的内存块数量

} Argument_List;

void checkArgumentCount(const int __argc, char const * argv[]);
void setArgumentList(const int __argc, char const * __argv[], Argument_List * __argList);
void checkArgumentValue(const Argument_List * __argList);
void showArgListInfo(const Argument_List * __argList);
void allocateMemory(const Argument_List * __argList, char * __buffer[]);
void freeMeomry(const Argument_List * __argList, char * __buffer[]);

int main(int argc, char const * argv[])
{
    putchar('\n');

    checkArgumentCount(argc, argv);

    char * memoryBuffer[MAX_ALLOCS] = {0};
    Argument_List argumentList = {0};

    setArgumentList(argc, argv, &argumentList);

    checkArgumentValue(&argumentList);

    showArgListInfo(&argumentList);

    printf("Befor allocateMemory() inital program break = %p\n\n", GET_CURRENT_PROGRAM_BREAK);

    allocateMemory(&argumentList, memoryBuffer);

    printf("\nAfter allocateMemory() program break = %p\n\n", GET_CURRENT_PROGRAM_BREAK);

    freeMeomry(&argumentList, memoryBuffer);

    printf("After freeMeomry() program break = %p\n", GET_CURRENT_PROGRAM_BREAK);

    return EXIT_SUCCESS;
}

void checkArgumentCount(const int __argc, char const * __argv[])
{
    if (__argc < 3 || strcmp(__argv[1], "--help") == 0)
    {
        usageErr("%s <num-allocs> <block-size> [free-step] [free-min ~ free-max]\n", __argv[0]);
    }
}

void setArgumentList(const int __argc, char const * __argv[], Argument_List * __argList)
{
    if (!__argList)
    {
        fatal("setArgumentList(..., ..., __argList): argument list not exist.\n");
    }

    __argList->numAllocs = getInt(__argv[1], GN_GT_0, "<num-allocs>");
    __argList->blockSize = getInt(__argv[2], GN_GT_0, "<block-size>");
    __argList->freeStep  = (__argc > 3) ? (getInt(__argv[3], GN_GT_0, "<free-step>")) : 1;
    __argList->freeMin   = (__argc > 4) ? (getInt(__argv[4], GN_GT_0, "min")) : 1;
    __argList->freeMax   = (__argc > 5) ? (getInt(__argv[5], GN_GT_0, "max")) : __argList->numAllocs;
}

void checkArgumentValue(const Argument_List * __argList)
{
    if (!__argList)
    {
        fatal("checkArgumentValue(__argList): argument list not exist.\n");
    }
    
    /*
        <num-allocs> 参数值超过 MAX_ALLOCS 或 小于等于 0 时
    */
    if (__argList->numAllocs > MAX_ALLOCS || __argList->numAllocs <= 0)
    {
        cmdLineErr(
                    "num-allocs = [%d] is invalid, range of <num-allocs> = (1 ~ %d).\n", 
                    __argList->numAllocs, MAX_ALLOCS
                );
    }

    /**
     * 在最大释放索引值 max 大于 <num-allocs> 参数值或小于等于 0 时,
     * 需要报错, 虽然 free(NULL) 约等于什么都不执行, 但多余的操作本就不应该有.
    */
    if (
            __argList->freeMax > __argList->numAllocs ||
            __argList->freeMax <= 0 ||
            __argList->freeMin > __argList->freeMax
        )  
    {
        cmdLineErr(
                    "free range = [%d ~ %d] is invalid, range of [free-min ~ free-max] = [1 ~ %d].\n",
                    __argList->freeMin, __argList->freeMax, __argList->numAllocs
                );
    }

    /**
     * 当然步长也不得超过已分配的内存大小 (numAllocs) 或 小于 0
     * 
     * 虽然分配的数组很大, 但依旧有风险
    */
    if (__argList->freeStep > __argList->numAllocs || __argList < 0)
    {
        cmdLineErr(
            "free-step = %d is invalid, the range of free-step = (1 < free-step < %d)\n",
            __argList->freeStep, __argList->numAllocs
        );
    }
}

void showArgListInfo(const Argument_List * __argList)
{
    //printSplitLine(SPLIT_LINE_LENGTH, '-', STDOUT_FILENO); puts("");

    printf(
            "Allocate %ld * %ld Bytes (Total %.4lf Kbytes)\n\n",
            __argList->numAllocs, __argList->blockSize, 
            (double)(__argList->numAllocs * __argList->blockSize / K_BYTES)
        );

    printf(
            "Range of free(): from memoryBuffer[%d] to memoryBuffer[%d] (free-stap = %d)\n\n",
            __argList->freeMin - 1, __argList->freeMax - 1, __argList->freeStep
        );

    //printSplitLine(SPLIT_LINE_LENGTH, '-', STDOUT_FILENO); puts("\n");
}

void allocateMemory(const Argument_List * __argList, char * __buffer[])
{
    //printSplitLine(SPLIT_LINE_LENGTH, '-', STDOUT_FILENO); puts("");
    puts("function allocateMemory() calling.");
    puts("Allocate for\tCurrent program break");
    //printSplitLine(SPLIT_LINE_LENGTH, '-', STDOUT_FILENO); puts("");

    for (int index = 0; index < __argList->numAllocs; ++index)
    {
        __buffer[index] = (char *)malloc(__argList->blockSize);
        if (!__buffer[index])
        {
            errExit("allocateMemory(): __buffer[%d] = (char *)malloc(__argList->blockSize): Faild to allocate.", index);
        }

        printf(
                "buffer[%d]\t\t%p\n", index, GET_CURRENT_PROGRAM_BREAK
            );
    }
}

void freeMeomry(const Argument_List * __argList, char * __buffer[])
{
    for (int index = __argList->freeMin - 1; index < __argList->freeMax; index += __argList->freeStep)
    {
        free(__buffer[index]);
    }
}