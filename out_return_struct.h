#include <stdint.h>
#include <stdlib.h>
#include "output_struct.h"
#ifndef OUT_RETURN_STRUCT_H
#define OUT_RETURN_STRUCT_H

struct out_return
{
    size_t count;
    OUT *result;
};
typedef struct out_return outRet;

#endif