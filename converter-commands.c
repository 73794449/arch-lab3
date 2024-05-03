#include "converter.h"
#include <string.h>
#include <stdlib.h>

size_t MOVREGREG(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, size_t out_count)
{
    result[out_count].COM = "MOV";
    result[out_count].ARGS.hasReg1 = true;
    result[out_count].ARGS.hasReg2 = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);
    if (i + 1 < values_count)
    {
        add_bits(result, out_count, values[i]);
        add_bits(result, out_count, values[i + 1]);
        result[out_count].ARGS.reg1 = values[i];
        result[out_count].ARGS.reg2 = values[i + 1];
        i++;
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Not enough args";
    }
    return i;
}

size_t MOVREGADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count)
{
    result[out_count].COM = "MOV";
    result[out_count].ARGS.hasReg1 = true;
    result[out_count].ARGS.hasAddr = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);
    if (values[i] == '0')
    {
        add_bits(result, out_count, values[i]);
        if (i + 9 < values_count)
        {
            add_bits(result, out_count, values[i + 1]);
            char for_phys[9];
            for (size_t k = 0; k < 8; k++)
            {
                add_bits(result, out_count, values[i + k + 2]);
                for_phys[k] = (char)values[i + k + 2];
            }
            result[out_count].ARGS.reg1 = values[i + 1];
            for_phys[8] = '\0';
            i += 9;
            set_vaddr(result, out_count, for_phys);
            char *physCheck = checkPhysAddr(for_phys, table);
            if (strcmp(physCheck, "") == 0)
                result[out_count].ARGS.addr = getPhysAddr(for_phys, table);
            else
            {
                result[out_count].hasError = true;
                result[out_count].ErrorMessage = physCheck;
            }
        }
        else
        {
            result[out_count].hasError = true;
            result[out_count].ErrorMessage = "Not enough args";
        }
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Missing 0 after 1B in MOV REG, ADDR";
    }
    return i;
}

size_t ADDREGREG(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, size_t out_count)
{
    result[out_count].COM = "ADD";
    result[out_count].ARGS.hasReg1 = true;
    result[out_count].ARGS.hasReg2 = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);
    if (i + 1 < values_count)
    {
        add_bits(result, out_count, values[i]);
        add_bits(result, out_count, values[i + 1]);
        result[out_count].ARGS.reg1 = values[i];
        result[out_count].ARGS.reg2 = values[i + 1];
        i++;
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Not enough args";
    }
    return i;
}

size_t ADDREGADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count)
{
    result[out_count].COM = "ADD";
    result[out_count].ARGS.hasReg1 = true;
    result[out_count].ARGS.hasAddr = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);

    if (values[i] == '0')
    {
        add_bits(result, out_count, values[i]);
        if (i + 9 < values_count)
        {
            add_bits(result, out_count, values[i + 1]);
            char for_phys[9];
            for (size_t k = 0; k < 8; k++)
            {
                add_bits(result, out_count, values[i + k + 2]);
                for_phys[k] = (char)values[i + k + 2];
            }
            result[out_count].ARGS.reg1 = values[i + 1];
            for_phys[8] = '\0';
            i += 9;
            set_vaddr(result, out_count, for_phys);
            char *physCheck = checkPhysAddr(for_phys, table);
            if (strcmp(physCheck, "") == 0)
                result[out_count].ARGS.addr = getPhysAddr(for_phys, table);
            else
            {
                result[out_count].hasError = true;
                result[out_count].ErrorMessage = physCheck;
            }
        }
        else
        {
            result[out_count].hasError = true;
            result[out_count].ErrorMessage = "Not enough args";
        }
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Missing 0 after 02 in ADD REG, ADDR";
    }
    return i;
}

size_t JMPADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count)
{
    result[out_count].COM = "JMP";
    result[out_count].ARGS.hasAddr = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);
    if (i + 7 < values_count)
    {
        char for_phys[9];
        for (size_t k = 0; k < 8; k++)
        {
            add_bits(result, out_count, values[i + k]);
            for_phys[k] = (char)values[i + k];
        }
        result[out_count].ARGS.reg1 = values[i];
        for_phys[8] = '\0';
        i += 7;
        set_vaddr(result, out_count, for_phys);
        char *physCheck = checkPhysAddr(for_phys, table);
        if (strcmp(physCheck, "") == 0)
            result[out_count].ARGS.addr = getPhysAddr(for_phys, table);
        else
        {
            result[out_count].hasError = true;
            result[out_count].ErrorMessage = physCheck;
        }
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Not enough args";
    }
    return i;
}

size_t JLADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count)
{
    result[out_count].COM = "JL";
    result[out_count].ARGS.hasAddr = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);
    if (i + 7 < values_count)
    {
        char for_phys[9];
        for (size_t k = 0; k < 8; k++)
        {
            add_bits(result, out_count, values[i + k]);
            for_phys[k] = (char)values[i + k];
        }
        result[out_count].ARGS.reg1 = values[i];
        for_phys[8] = '\0';
        i += 7;
        set_vaddr(result, out_count, for_phys);
        char *physCheck = checkPhysAddr(for_phys, table);
        if (strcmp(physCheck, "") == 0)
            result[out_count].ARGS.addr = getPhysAddr(for_phys, table);
        else
        {
            result[out_count].hasError = true;
            result[out_count].ErrorMessage = physCheck;
        }
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Not enough args";
    }
    return i;
}

size_t JGADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count)
{
    result[out_count].COM = "JG";
    result[out_count].ARGS.hasAddr = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);
    if (i + 7 < values_count)
    {
        char for_phys[9];
        for (size_t k = 0; k < 8; k++)
        {
            add_bits(result, out_count, values[i + k]);
            for_phys[k] = (char)values[i + k];
        }
        result[out_count].ARGS.reg1 = values[i];
        for_phys[8] = '\0';
        i += 7;
        set_vaddr(result, out_count, for_phys);
        char *physCheck = checkPhysAddr(for_phys, table);
        if (strcmp(physCheck, "") == 0)
            result[out_count].ARGS.addr = getPhysAddr(for_phys, table);
        else
        {
            result[out_count].hasError = true;
            result[out_count].ErrorMessage = physCheck;
        }
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Not enough args";
    }
    return i;
}

size_t CMPREGREG(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, size_t out_count)
{
    result[out_count].COM = "CMP";
    result[out_count].ARGS.hasReg1 = true;
    result[out_count].ARGS.hasReg2 = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);
    if (i + 1 < values_count)
    {
        add_bits(result, out_count, values[i]);
        add_bits(result, out_count, values[i + 1]);
        result[out_count].ARGS.reg1 = values[i];
        result[out_count].ARGS.reg2 = values[i + 1];
        i++;
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Not enough args";
    }
    return i;
}

size_t MOVREGLIT32(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, size_t out_count)
{
    result[out_count].COM = "MOV";
    result[out_count].ARGS.hasReg1 = true;
    result[out_count].ARGS.hasLit32 = true;
    add_bits(result, out_count, insts[0]);
    add_bits(result, out_count, insts[1]);
    if (values[i] == '2')
    {
        add_bits(result, out_count, values[i]);
        if (i + 9 < values_count)
        {
            add_bits(result, out_count, values[i + 1]);
            char for_lit[9];
            for (size_t k = 0; k < 8; k++)
            {
                add_bits(result, out_count, values[i + k + 2]);
                for_lit[k] = (char)values[i + k + 2];
            }
            result[out_count].ARGS.reg1 = values[i + 1];
            for_lit[8] = '\0';
            i += 9;
            result[out_count].ARGS.lit32 = (uint32_t)strtoll(for_lit, NULL, 16);
        }
        else
        {
            result[out_count].hasError = true;
            result[out_count].ErrorMessage = "Not enough args";
        }
    }
    else
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Missing 2 after 1C in MOV REG, LIT32";
    }
    return i;
}