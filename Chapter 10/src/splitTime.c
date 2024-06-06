#include <sys/unistd.h>

#include "../include/timeFuncAssistant.h"

/**
 * 本程序旨在说明 gmtime() 和 localtime() 函数的区别和用法
 * 
 * gmtime() 可以把时间戳转换成一个对应与 UTC 的分解时间 (GM 意为格林威治标准时间)
 * 
 * 而 localtime() 需要考虑时区和夏令时设置 (中国不实行夏令时) 再把把时间戳转换成一个对应与 UTC 的分解时间
 * 
 * 两个函数都传入一个时间戳 (通常由 time() 函数获取), 
 * 然后解析时间戳后将结果分配在一个静态分配的结构体 tm 中, tm 结构体的示例如下:
*/

#if FALSE
/* ISO C `broken-down time' structure.  */
struct tm
{
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

# ifdef	__USE_MISC
  long int tm_gmtoff;		/* Seconds east of UTC.  */
  const char *tm_zone;		/* Timezone abbreviation. (时区缩写) */
# else
  long int __tm_gmtoff;		/* Seconds east of UTC.  */
  const char *__tm_zone;	/* Timezone abbreviation.  */
# endif
};
#endif

int main(int argc, char const *argv[])
{
	while (TRUE)
  	{
		system("clear");

    	time_t curTimeStamp = time(NULL);
      	struct tm * localTimeInfo = localtime(&curTimeStamp);

		if (!localTimeInfo) { errExit("main() -> localtime(&curTimeStamp): "); }

		/**
		 * 由于 gmtime() 和 localtime() 都是往同一个静态结构体写入, 
		 * 这在多线程或者并发的环境下会造成数据竞争, 
		 * 所以还需要两个非重入版本的函数, gmtime_r() 函数就是其一, 需要自行创建一个 tm 结构体实例,
		 * 用法如下:
		 */
		struct tm gmTimeInfo = {0};
		gmtime_r(&curTimeStamp, &gmTimeInfo);

		puts("Local time: ");
		showtmStruct(localTimeInfo);

		printSplitLine(45, '-', STDOUT_FILENO); puts("");

		puts("Greenwich mean time: ");
		showtmStruct(&gmTimeInfo);
		
		printSplitLine(45, '-', STDOUT_FILENO); puts("");

		/**
		 * mktime() 函数用于把 tm 结构体转化成时间戳的形式
		*/
		printf(
				"Local time stamp = %ld, GM time stamp = %ld\n", 
				mktime(localTimeInfo), mktime(&gmTimeInfo)
			);

		sleep(1);
  	}
    return EXIT_SUCCESS;
}