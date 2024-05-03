#include "converter.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

outRet convert(char *_input, DSC *table)
{
    size_t out_count = 0;
    valRet valStruct = create_values(_input);
    uint8_t* values = valStruct.values;
    size_t values_count = valStruct.count;
    uint8_t inst_i = 0;
    uint8_t insts[2];
    OUT *result = NULL;
    result = realloc_with_memset_zero_out(result);
    for (size_t i = 0; i < values_count; i++)
    {
        if (inst_i < 2)
            insts[inst_i++] = values[i];
        else
        {
            char instruction[2] = {(char)insts[0], (char)insts[1]};
            uint8_t command = (uint8_t)strtol(instruction, NULL, 16);
            switch (command)
            {
            case 0x1A:
                // MOV REG1,REG2
                i = mov1a();
                break;
            case 0x1B:
                // MOV REG, ADDR
                i = mov1b();
                break;
            case 0x01:
                // ADD REG, REG
                i = add01();
                break;
            case 0x02:
                // ADD REG, ADDR
                i = add02();
                break;
            case 0x91:
                // JMP ADDR
                i = jmp91();
                break;
            case 0x93:
                // JL ADDR
                i = jl93();
                break;
            case 0x95:
                // JG ADDR
                i = jg95();
                break;
            case 0x80:
                // CMP REG, REG
                i = cmp80();
                break;
            case 0x1C:
                // MOV REG, LIT32
                i = mov1c();
                break;
            default:
                UNKNOWN(result, insts, out_count);
                break;
            }
            inst_i = 0;
            out_count++;
            if (out_count == SIZE_RESULT)
                result = realloc_with_memset_zero_out(result);
        }
    }

    if (inst_i != 0)
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Readen instruction, but no operands";
        add_bits(result, out_count, insts[0]);
        if (inst_i == 2)
            add_bits(result, out_count, insts[1]);
        out_count++;
    }

    free(values);
    outRet ret = {out_count,result};
    return ret;
}