#include "../include/myenv.h"

static size_t strFind(const char * __str, size_t __len, const char __ch);

/**
 * @brief 在 __environ 字符串数组中寻找指定环境的下标
*/
static int searchEnviromentName(const char * __name);

static size_t strFind(const char * __str, size_t __len, const char __ch)
{
    size_t target = 0UL;

    for (; target < __len; ++target)
    {
        if (__str[target] == __ch) { return target; }
    }

    if (target == __len) { return FALSE; }
}

static int searchEnviromentName(const char * __name)
{
    if (!__name || !(*__name))
    {
        fprintf(
                stderr, 
                "searchEnviromentName(__name): Argument __name is not exist or empty.\n"
            );

        return -1;
    }

    int targetIndex = 0;

    for (char ** environIndex = __environ; environIndex != NULL; ++environIndex)
    {
        if (strncmp(*environIndex, __name, strlen(__name)) != 0 && (*environIndex)[strlen(__name)] != '=') { targetIndex++; }
        else
        {
            return targetIndex;
        }
    }

    return -1;
}

int mySetenv(const char * __name, const char * __value, int __replace)
{
    if (!__name || !(*__name))
    {
        fprintf(
                stderr, 
                "mySetenv(__name = %s, ..., ...): Environment name is not exist or empty.\n",
                __name
            );

        return -1;
    }

    if (!__value || !(*__value))
    {
        fprintf(
            stderr,
            "mySetenv(..., __value = %s, ...): Environment value is not exist or empty.\n",
            __value
        );

        return -1;
    }

    int targetInvName = 0;
    size_t newEnvpStrLen = strlen(__name) + strlen(__value) + 2;

    switch (__replace)
    {
        case CREATE_OR_MODIFY:
            if ((targetInvName = searchEnviromentName(__name)) != -1)
            {
                char * newEnvironmentStr = calloc(newEnvpStrLen, sizeof(char));

                strcpy(newEnvironmentStr, __name);
                strcat(newEnvironmentStr, "=");
                strcat(newEnvironmentStr, __value);

                putenv(newEnvironmentStr);
            }
            else { return -1; }
            
            return 0;

        case NO_OPERATION:

            return 0;

        default:
            fprintf(
                        stderr, 
                        "mySetenv(..., ..., __replace = %d): invalid flag, choice CREATE_OR_MODIFY or NO_OPERATION\n",
                        __replace
                    );
            break;
    }

    return -1;
}

int myUnsetenv(const char * __name)
{
    if (!__name || !(*__name) || strchr(__name, '=') != NULL)
    {
        errno = EINVAL;

        return -1;
    }

    size_t nameLen = strlen(__name);

    /**
     * 在整个 __environ 指针数组中循环遍历
    */
    for (char ** ep = __environ; *ep != NULL;)
    {
        /**
         * 直到找到符合 __name 要求且格式正确的环境变量
        */
        if (strncmp(*ep, __name, nameLen) == 0 && (*ep)[nameLen] == '=')
        {
            /**
             * 这一步很关键, 把从 ep 开始的全部指针都更新到下一个元素的值, 以此完成删除的效果,
             * 具体图例见 ../data/myUnsetenv.dio
            */
            for (char ** sp = ep; *sp != NULL; ++sp)
            {
                *sp = *(sp + 1);
            }
        }
        else { ++ep; }
    }

    return 0;
}