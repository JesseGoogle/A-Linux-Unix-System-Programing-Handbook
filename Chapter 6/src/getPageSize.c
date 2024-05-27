#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    /**
     * 通过 sysconf(_SC_PAGESIZE) 函数，
     * 获取系统内存页单页的最大尺寸，本系统是 4096 Bytes。
    */
    printf("This system max memery page size = %ld\n", sysconf(_SC_PAGESIZE));
    
    return 0;
}
