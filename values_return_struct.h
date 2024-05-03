#include <stdint.h>
#include <stdlib.h>
#ifndef VALUES_RETURN_STRUCT_H
#define VALUES_RETURN_STRUCT_H

struct values_return
{
    size_t count;
    uint8_t *values;
};
typedef struct values_return valRet;

#endif