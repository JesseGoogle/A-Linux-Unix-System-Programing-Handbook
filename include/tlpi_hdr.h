#ifndef _TLPI_HDR_H_
#define _TLPI_HDR_H_

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "./getNum.h"
#include "./errorFunctions.h"
#include "./fileState.h"

typedef enum { FALSE, TRUE } Boolean;

#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#endif // _TLPI_HDR_H_