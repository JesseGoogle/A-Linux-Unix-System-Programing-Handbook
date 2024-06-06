#include <time.h>

#include "../../include/tlpi_hdr.h"

#define BEGIN_YEAR          1900
#define ONE_DAY_HOURS       24  

#define TIME_STRING_BUFF_SIZE 512

const char * Weeks [] = {
                            "Monday", "Tuesday", "Wednesday", 
                            "Thursday", "Friday", "Saturday", "Sunday"
                    };

static inline int getYears(const int __tmYear) { return BEGIN_YEAR + __tmYear; }

/**
 * @brief 输出 tm 结构体的信息
*/
void showtmStruct(const struct tm * __timeInfo);

/**
 * @brief 通过调用 strftime() 函数和格式化字符 __format, 
 *        输出格式化字符串
 * 
 * @param __format 格式化字符串 (如 %F %T) 等
 * 
 * @return 经 strftime() 函数处理的字符串
*/
char * getCurrentTime(const char * __format);