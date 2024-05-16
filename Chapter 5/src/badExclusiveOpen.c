#include "../include/tlpi_hdr.h"

#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    system("clear");

    int fileDes = 0;

    if ((fileDes = open(argv[1], O_WRONLY)) != -1)  // 若成功打开了文件  
    {
        /**
         * 报告文件已经存在并输出文件名和打开这个文件程序的 PID，最后关闭文件。
        */
        fprintf(
                    stderr,
                    "[PID = %ld] File %s already exsists.\n",
                    (long int) getpid(), argv[1]
                );

        close(fileDes);
    }
    else
    {
#if false
        fprintf(
                    stderr,
                    "[PID = %ld] File %s doesn't exit yet.\n",
                    (long int) getpid(), argv[1]
            );

        if (argc > 2) 
        { 
            sleep(5);  
            printf("[PID = %ld] Done Sleeping.\n", (long int) getpid());
        }
#endif

        /*若打开文件出现了问题且不是 No such file or directory 这个问题*/
        if (errno != ENOENT) { errExit("open()"); }
        else
        {

            if ((fileDes = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) { errExit("open()"); }

            fprintf(
                        stderr,
                        "[PID = %ld] Crearte File: %s exclusively\n",
                        (long int) getpid(), argv[1]
            );
        }
    }

    close(fileDes);

    return EXIT_SUCCESS;
}
