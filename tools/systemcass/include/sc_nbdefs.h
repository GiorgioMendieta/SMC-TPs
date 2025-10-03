/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_main.cc                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_DEFS_H__
#define __SC_DEFS_H__

#ifndef WIN32
typedef signed long long   int64;
typedef unsigned long long uint64;
#else
typedef signed __int64     int64;
typedef unsigned __int64   uint64;
#endif

typedef long           int32;
typedef unsigned long  uint32;
typedef short          int16;
typedef unsigned short uint16;
typedef char           int8;
typedef unsigned char  uint8;

typedef uint16         smallest_uint;
typedef int16          smallest_int;

#endif
