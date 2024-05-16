#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "../include/getNum.h"

static void gnFail(
                      const char * __fname, const char * __msg, 
                      const char * __arg, const char * __name
                    )
{
    if (__name != NULL)
    {
        fprintf(stderr, "(in %s)", __name);
    }

    fprintf(stderr, ": %s\n", __msg);

    if ((__arg != NULL) && (*__arg != '\0'))
    {
        fprintf(stderr, "       offending text: %s\n", __arg);
    }

    exit(EXIT_FAILURE);
}

static long getNum(
                    const char * __fname, const char * __arg, 
                    int __flags, const char * __name
                )
{
    long int res;
    char * endPtr;
    int base;

    /*若字符串 __arg 为空或者解引用后是空字符，调用 gnFail() 函数。*/
    if (__arg == NULL || *__arg == '\0')
        gnFail(__fname, "No or Empty String", __arg, __name);

    base = (__flags & GN_ANY_BASE) ? 0 : (__flags & GN_BASE_8) ? 8 : \
           (__flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    res = strtol(__arg, &endPtr, base);

    if (errno != 0)
        gnFail(__fname, "strtol() failed", __arg, __name);

    if (*endPtr != '\0')
        gnFail(__fname, "nonnumeric characters", __arg, __name);

    if ((__flags & GN_NONNEG) && res < 0)
        gnFail(__fname, "Nagetive value not allowed", __arg, __name);

    if ((__flags & GN_GT_0) && res < 0)
        gnFail(__fname, "Value must be > 0", __arg, __name);

    return res;
}

long int getLong(const char * __arg, int __flags, const char * __name)
{
    return getNum("getLong", __arg, __flags, __name);
}

int getInt(const char * __arg, int __flags, const char * __name)
{
    long int res;

    res = getNum("getInt", __arg, __flags, __name);

    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "Intenger out of range", __arg, __name);

    return (int)res;
}