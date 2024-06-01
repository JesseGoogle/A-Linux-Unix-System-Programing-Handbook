#include <pwd.h>
#include <grp.h>
#include <stdlib.h>

#include "../include/ugid_functions.h"

char * userNameFromID(uid_t __uid)
{
    struct passwd * pwd;

    pwd = getpwuid(__uid);

    return (!pwd) ? NULL : pwd->pw_name;
}

uid_t userIDFromName(const char * __name)
{
    struct passwd * pwd;
    uid_t uid = 0U;
    char * endPtr = NULL;

    if (!__name || !(*__name)) { return -1; }

    /**
     * 这里的操作可能是为了应对下面的调用设计的:
     * 
     *  userIDFromName("100000");
    */
    uid = strtol(__name, endPtr, 10);

    if (!(*endPtr)) { return uid; }

    pwd = getpwnam(__name);
    if (!pwd) { return -1; }

    return pwd->pw_uid;
}

char * groupNameFromID(gid_t __gid)
{
    struct group * grp;

    grp = getgrnam(__gid);

    return (!grp) ? NULL : grp->gr_name;
}

gid_t groupIDFromName(const char * __name)
{
    struct group * grp;
    gid_t gid = 0U;
    char * endPtr;

    if (!__name || !(*__name)) { return -1; }

    gid = strtol(__name, endPtr, 10);

    if (!(*endPtr)) { return gid; }

    grp = getgrnam(__name);

    if (!grp) { return -1; }

    return grp->gr_gid;
}