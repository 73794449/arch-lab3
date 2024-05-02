#include "table.h"
#include <stdio.h>
#ifndef CONVERTER_H
#define CONVERTER_h

extern size_t out_count;

struct args{
    bool hasReg1;
    bool hasReg2;
    bool hasAddr;
    bool hasLit32;
    uint8_t reg1;
    uint8_t reg2;
    uint32_t addr;
    char* vaddr;
    uint16_t seg;
    uint32_t offset;
    uint32_t lit32;
};


struct output
{
    uint8_t* values;
    size_t values_count;
    char* COM;
    struct args ARGS;
    char* ErrorMessage;
    bool hasError;
};
typedef struct output OUT;


OUT* convert(char* _input, DSC* table);
#endif