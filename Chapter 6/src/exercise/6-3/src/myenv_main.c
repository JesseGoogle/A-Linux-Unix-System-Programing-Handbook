#include "../include/myenv.h"

enum Display_Flag { SHOW_ALL, SHOW_ONE };

void showenvp(enum Display_Flag __displayFlag, const char * __envpName);

void showenvp(enum Display_Flag __displayFlag, const char * __envpName)
{
    if (__displayFlag == SHOW_ALL)
    {
        char ** ep;

        for (ep = __environ; *ep != NULL; ep++) { printf("%p\t%s\n", *ep, *ep); }

        puts("\n");
    }
    else if (__displayFlag == SHOW_ONE)
    {
        char * tarenvpPtr = NULL;
        if ((tarenvpPtr = getenv(__envpName)) != NULL)
        {
            printf("%p\t%s\n",tarenvpPtr, tarenvpPtr);
        }
        else
        {
            fatal("showenvp() -> getenv(__envpName): __envpName [%s] not found..\n", __envpName);
        }
    }
}

int main(int argc, char const *argv[])
{
    system("clear");

    showenvp(SHOW_ALL, NULL);

    printf("Before mySetenv()\n");
    
    showenvp(SHOW_ONE, "HOME");

    if (mySetenv("HOME", "/home/__jesse", CREATE_OR_MODIFY) == -1) { exit(EXIT_FAILURE); }

    printf("After mySetenv()\n");

    showenvp(SHOW_ONE, "HOME");

    showenvp(SHOW_ALL, NULL);

    if (myUnsetenv("SESSION_MANAGER") == -1) { errExit("myUnsetenv(""SESSION_MANAGER"")"); }

    showenvp(SHOW_ALL, NULL);

    return EXIT_SUCCESS;
}
