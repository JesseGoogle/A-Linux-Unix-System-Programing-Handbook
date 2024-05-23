#include "./5-7.h"

#include <fcntl.h>
#include <sys/stat.h>

ssize_t myReadv(const int __fd, const  MyIovec *__iovec, const int __iovCount)
{
    /**
     * 检查 __iovec 结构体指针或者结构体内的数据指针是否为空
    */
    if (!__iovec || !__iovec->resourcePointer) 
    {  
        fprintf(stderr, "myReadv(__fd, [NULL], __iovCount): read data __iovec is Emyty or NULL."); 

        return -1;
    }
    if (!__iovCount)    // 检查结构体数组
    { 
        fprintf(stderr, "myReadv(__fd, __iovec, [0]): Length of __iovec not be 0."); 
        return -1;
    }

    ssize_t totalBytesRead = 0L;

    for (int iovIndex = 0; iovIndex < __iovCount; ++iovIndex)
    {
        if (__iovec[iovIndex].resourceLength <= 0) { continue; }

        ssize_t readBytes = 0L;

        if ((readBytes = read(__fd, __iovec[iovIndex].resourcePointer, __iovec[iovIndex].resourceLength)) == -1)
        {
            perror("myReadv() -> read()");
            return -1;
        }
        else if (!readBytes || !totalBytesRead || !iovIndex) { return 0; }
        
        totalBytesRead += readBytes;

       if (readBytes < (ssize_t)(__iovec[iovIndex].resourceLength)) { break; } 
    }

    return totalBytesRead;
}

ssize_t myWritev(const int __fd, const MyIovec *__iovec, const int __iovCount)
{
    if (!__iovec || !__iovec->resourcePointer) 
    {  
        fprintf(stderr, "myWritev(__fd, [NULL], __iovCount): read data __iovec is Emyty or NULL."); 

        return -1;
    }
    if (!__iovCount) 
    { 
        fprintf(stderr, "myWritev(__fd, __iovec, [0]): Length of __iovec not be 0."); 
        return -1;
    }

    ssize_t totalBytesWrite = 0L;

    for (int iovIndex = 0; iovIndex < __iovCount; ++iovIndex)
    {
        if (__iovec[iovIndex].resourceLength <= 0) { continue; }

        ssize_t writeBytes = 0L;

        if ((writeBytes = write(__fd, __iovec[iovIndex].resourcePointer, __iovec[iovIndex].resourceLength)) == -1)
        {
            perror("myWritev() -> write()");
            return -1;
        }
        //else if (!writeBytes || !totalBytesWrite || !iovIndex) { return 0; }
        
        totalBytesWrite += writeBytes;

       if (writeBytes < (ssize_t)(__iovec[iovIndex].resourceLength)) { break; } 
    }

    return totalBytesWrite;
}