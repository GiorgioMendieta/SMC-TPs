/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 data_field.h                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   03_05_2006                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __DATA_FIELD_H__
#define __DATA_FIELD_H__

#include <endianness.h>

template<int      WIDTH,
         int      PADDING,
         typename data_type>
struct val_field { /* try to work with little endianess */
#if defined(little_endian)
  /* little endian */
//  data_type pad:PADDING;
  data_type valW:WIDTH;
#elif defined(big_endian)
  /* big endian */
  data_type pad:PADDING;
  data_type valW:WIDTH;
#else
#error You must define endianness.
#endif
};

template<int      WIDTH,
         typename data_type>
struct val_field<WIDTH,0,data_type> {
  data_type valW:WIDTH;
};

#endif

