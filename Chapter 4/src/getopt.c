#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include <ctype.h>

int main(int argc, char *argv[]) 
{
    system("clear");

    int a_flag = 0;
    char * b_value = NULL;

    // 初始化 getopt 环境
    opterr = 0; // 禁止 getopt 输出错误信息，手动处理错误
    int option;

    while ((option = getopt(argc, argv, "ab:")) != -1) {
        switch (option) 
        {
            case 'a':
                a_flag = 1;
                break;
            case 'b':
                b_value = optarg; // 获取选项参数
                break;
            case '?':
                if (optopt == 'b') 
                {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } 
                else if (isprint(optopt)) 
                {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } 
                else 
                {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                
                return 1;
            default:
                abort();
        }
    }

    // 检查是否还有剩余的非选项参数
    if (optind < argc) {
        printf("Non-option arguments: %s\n", argv[optind]);
    }

    // 打印结果
    if (a_flag) printf("Option -a is set.\n");
    if (b_value) printf("Option -b is set with value `%s'.\n", b_value);

    return 0;
}