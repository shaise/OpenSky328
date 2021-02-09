#ifndef __HAL_DEFINES_H__
#define __HAL_DEFINES_H__

#include <stdint.h>
#define EXTERNAL_MEMORY
#define LO(w) (w & 0xFF)
#define HI(w) ((w>>8) & 0xFF)

#endif // __HAL_DEFINES_H__
