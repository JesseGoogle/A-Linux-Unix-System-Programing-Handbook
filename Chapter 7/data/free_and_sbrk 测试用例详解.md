# free_and_sbrk 测试用例详解

## 以下是 3 组测试用例

```sh
free_and_sbrk 1000 10240 2
```

```c
----------------------------------------------------------------
Alloc 1000 memory block, each block size = 10240
Alloc size = 0.009537 Mbytes, free stap = 2

Range of free(): from memoryBuffer[0] to memoryBuffer[999999]
----------------------------------------------------------------

Before malloc(), initial program break = [0x5adf86d7c000]

After malloc(),  initial program break = [0x5adf87744000]

After free(),    initial program break = [0x5adf87744000]
----------------------------------------------------------------
Done.
```

结果显示, 释放完内存后, `program break` 的位置仍然与分配完所有内存块后的水平相当.

```sh
free_and_sbrk 1000 10240 1 1 999
```

```c
----------------------------------------------------------------
Alloc 1000 memory block, each block size = 10240
Alloc size = 0.009537 Mbytes, free stap = 1

Range of free(): from memoryBuffer[0] to memoryBuffer[998]
----------------------------------------------------------------

Before malloc(), initial program break = [0x5a3ef42d3000]

After malloc(),  initial program break = [0x5a3ef4c9b000]

After free(),    initial program break = [0x5a3ef4c9b000]
----------------------------------------------------------------
Done.
```

命令要求程序释放除 `memoryBuffer[999]` 外的所有内存,
`program break` 的位置仍然与分配完所有内存块后的水平相当.

```sh
free_and_sbrk 1000 10240 1 500 1000
```

```C
----------------------------------------------------------------
Alloc 1000 memory block, each block size = 10240
Alloc size = 0.009537 Mbytes, free stap = 1

Range of free(): from memoryBuffer[499] to memoryBuffer[999]
----------------------------------------------------------------

Before malloc(), initial program break = [0x599835acb000]

After malloc(),  initial program break = [0x599836493000]

After free(),    initial program break = [0x599835fac000]
----------------------------------------------------------------
Done.
```

命令要求程序从堆顶释放一组完整的连续内存块,
program break 的位置从峰值上降了下来, 这表明 free() 函数调用了 sbrk() 去改变堆的边界.
在这种情况下, free() 函数的 glib 实现为避免造成内存碎片, 会在释放内存时将相邻空间内存块合并为一整块更大的内存,
因而也有能力识别出堆顶的整个空闲区域.
