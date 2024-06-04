#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE

#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>

#include "../../include/tlpi_hdr.h"

int main(int argc, char const *argv[])
{
    char * userName = NULL, * password = NULL;
    char * encrypted = NULL;
    char * ptr = NULL;

    struct passwd * userPwd      = NULL;
    struct spwd   * superUserPwd = NULL;  

    Boolean authOk = FALSE;
    
    size_t len = 0UL;

    /**
     * sysconf(_SC_LOGIN_NAME_MAX) 调用获取本操作系统用户名最大长度
    */
    long int loginNameMax = sysconf(_SC_LOGIN_NAME_MAX);

    // 如果拿不到就设置个长点的..
    if (loginNameMax == -1) { loginNameMax = 256; }

    if (!(userName = (char *)malloc(loginNameMax))) { errExit("malloc(loginNameMax)"); }
    
    printf("User Name: ");
    fflush(stdout);

    /**
     * 从标准输入流读取最大 loginNameMax 长度的字符流给 userName 字符串
    */
    if (!fgets(userName, loginNameMax, stdin)) { fatal("fgets(userName, loginNameMax, stdin)\n"); }

    len = strlen(userName);

    /**
     * 把最后一位改成空字符, 构造字符串
    */
    if (userName[len - 1] == '\n') { userName[len - 1] = '\0'; }

    /**
     * 通过 userName 和 getpwnam() 函数该用户相关的信息
    */
    if (!(userPwd = getpwnam(userName))) { fatal("Couldn't get password record.\n"); }

    /**
     * 通过 userName 和 getspnam() 函数, 
     * 来检查是否拥有访问 /etc/shadow 文件的权限, 所以这样的程序需要 sudo 命令来执行
    */
    superUserPwd = getspnam(userName);

    if (!superUserPwd || errno == EACCES)
    {
        fatal("No permission to read shadow password file.\n");
    }

    /*
        如果上一步没问题, 
        就把 superUserPwd 中的密码字段赋值给 userPwd 的密码字段,
        但此时都是密文, 等待下一步的解密
    */
    if (superUserPwd != NULL) 
    {
        userPwd->pw_passwd = superUserPwd->sp_pwdp;
    }

    /**
     * 通过 getpass() 函数输入密码, 
     * 此函数调用期间会暂时屏蔽回显功能, 
    */
    password = getpass("Password: ");

    /**
     * 调用 crypt() 函数加密 password 明文 (通过 DES-like 或者 MD5), 
     * 返回加密后的密文给 encrypted
    */
    encrypted = crypt(password, userPwd->pw_passwd);

    /**
     * 完成加密操作后, 要立刻将明文抹掉, 
     * 防止有心之人中断程序后从核心转储文件里面拿到残存的明文密码
    */
    for (ptr = password; *ptr != '\0';) { *ptr++ = '\0'; }

    if (!encrypted) { errExit("crypt(password, userPwd->pw_passwd)"); }

    /**
     * 将加密后的 password 密文和 该用户密码的密文进行比对
    */
    authOk = (strcmp(encrypted, userPwd->pw_passwd) == 0);

    if (!authOk) { printf("Incorrect password.\n"); exit(EXIT_SUCCESS); }

    /**
     * 没问题就报告这个用户的 PID  
    */
    printf("Successfully authenticated: UID = %ld\n", (long) userPwd->pw_uid);

    free(userName);

    return EXIT_SUCCESS;
}