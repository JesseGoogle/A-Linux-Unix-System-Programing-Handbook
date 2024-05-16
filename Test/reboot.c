#include <stdio.h>

#include <unistd.h>
#include <sys/reboot.h>

int main(int argc, char const *argv[])
{
    printf("After 5 second, shout down this system.\n");

    sleep(5);

    /**
     * 调用系统级函数 rebbot() 直接关机，需要 root 用户权限。
    */
    reboot(RB_POWER_OFF);
    return 0;
}
