
#ifndef _TYPE_DEFS_HEADER_H
#define _TYPE_DEFS_HEADER_H

typedef unsigned int uint32;
typedef unsigned char uchar;
typedef unsigned long ulong;
#ifndef __cplusplus
typedef unsigned char bool;
#endif

#define MSG                         printf

#define TRUE                        (1)
#define FALSE                       (0)

#define COMPILE_ASSERT(condition) ((void)sizeof(char[1 - 2*!!!(condition)]))

#endif

