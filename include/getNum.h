#ifndef _GET_NUM_H_
#define _GET_NUM_H_

#define GN_NONNEG   01
#define GN_GT_0     02
                            /*默认情况下，整数是十进制的*/
#define GN_ANY_BASE 0100    /*可以使用在任何形如 `strtol(3)`*/
#define GN_BASE_8   0200    /*值被表示为 8 进制*/
#define GN_BASE_16  0400    /*值被表示为 16 进制*/

long int getLong(const char * __arg, int __flags, const char * __name);

int getInt(const char * __arg, int __flags, const char * __name);

#endif // _GET_NUM_H_