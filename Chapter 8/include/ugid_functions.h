#ifndef _UGID_FUNCTIONS_H_
#define _UGID_FUNCTIONS_H_

#include <sys/types.h>

/**
 * @brief 通过用户的 UID 去搜索该用户名
 * 
 * @param __uid 用户的 UID
 * 
 * @return 用户名, 若搜索不到就返回 NULL
 */
char * userNameFromID(uid_t __uid);

/**
 * @brief 通过用户名去搜索用户 UID
 * 
 * @param __name 用户名
 * 
 * @return 用户 UID, 错误则返回 -1
*/
uid_t userIDFromName(const char * __name);

/**
 * @brief 通过群组 ID 去查找群组名
 * 
 * @param __gid 群组 ID (GID)
 * 
 * @return 群组名, 错误则返回 NULL
*/
char * groupNameFromID(gid_t __gid);

/**
 * @brief 通过群组名去查找群组 ID
 * 
 * @param __name 群组名
 * 
 * @return 群组 ID, 错误则返回 -1
*/
gid_t groupIDFromName(const char * __name);

#endif // _UGID_FUNCTIONS_H_