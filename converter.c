#include "converter.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
size_t out_count = 0;

void add_bits(OUT *result, size_t out_c, uint8_t value)
{
    result[out_c].values_count++;
    result[out_c].values = (uint8_t *)realloc(result[out_c].values, result[out_c].values_count * sizeof(uint8_t));
    if (result[out_c].values == NULL)
        exit(EXIT_FAILURE);
    result[out_c].values[result[out_c].values_count - 1] = value;
}

void set_vaddr(OUT *result, size_t out_c, char* for_phys)
{
    result[out_c].ARGS.vaddr = malloc(13*sizeof(char));
    size_t k = 0;
    size_t space = 0;
    for(size_t i = 0; i < 12; i++){
        if(space != 2){
        result[out_c].ARGS.vaddr[i] = for_phys[k];
        k++;
        space++;
        }
        else
        {
            space = 0;
            result[out_c].ARGS.vaddr[i] = ' ';
        }
    }
    result[out_c].ARGS.vaddr[12] = '\0';
    uint32_t number = (uint32_t)strtol(for_phys, NULL, 16);
    result[out_c].ARGS.seg = (uint16_t)(number >> 21);
    uint32_t mask = 0x1fffff;
    result[out_c].ARGS.offset = (number & mask);
}

char *checkPhysAddr(char *for_phys, DSC *table)
{
    uint32_t number = (uint32_t)strtol(for_phys, NULL, 16);
    uint16_t num_of_d = (uint16_t)(number >> 21);
    uint32_t mask = 0x1fffff;
    uint32_t offset = (number & mask);

    if (num_of_d < RDT)
    {
        if(table[num_of_d].segment_size != 0){
        if (table[num_of_d].in_memory)
        {
            uint32_t physical = table[num_of_d].base_addr + offset;
            if (physical < table[num_of_d].base_addr + table[num_of_d].segment_size)
            {
                return "";
            }
            else
            {
                return "Out of segment";
            }
        }
        else
        {
            return "Flag not in memory";
        }
        }
        else
        {
            return "Segment does not exist";
        }
    }
    else
    {
        return "Number of segment > RDT";
    }
}

uint32_t getPhysAddr(char *for_phys, DSC *table)
{
    uint32_t number = (uint32_t)strtol(for_phys, NULL, 16);
    uint16_t num_of_d = (uint16_t)(number >> 21);
    uint32_t mask = 0x1fffff;
    uint32_t offset = (number & mask);

    uint32_t physical = table[num_of_d].base_addr + offset;
    return physical;
}

void skip(FILE *reader)
{
    char c;
    do
    {
        c = (char)fgetc(reader);
    } while (!(c == EOF || c == ';'));
}

OUT *convert(char *_input, DSC *table)
{
    size_t MAX_NUMBERS_V = 1000;
    size_t MAX_NUMBERS_O = 1000;
    size_t values_count = 0;
    FILE *reader = fopen(_input, "r");
    if (reader == NULL)
        exit(-1);
    uint8_t *values = malloc(MAX_NUMBERS_V * sizeof(uint8_t));
    if (values == NULL)
        exit(EXIT_FAILURE);

    char c;
    while (c != EOF)
    {
        c = (char)fgetc(reader);
        if (c == ';')
            skip(reader);
        else
        {
            if (isalnum(c))
            {
                values[values_count] = (uint8_t)c;
                values_count++;

                if (values_count == MAX_NUMBERS_V)
                {
                    MAX_NUMBERS_V *= 2;
                    values = realloc(values, MAX_NUMBERS_V * sizeof(uint8_t));
                    if (values == NULL)
                        exit(EXIT_FAILURE);
                }
            }
        }
    }

    uint8_t inst = 0;
    uint8_t insts[2];
    OUT *result = (OUT *)malloc(MAX_NUMBERS_O * sizeof(OUT));
    memset(result, 0, MAX_NUMBERS_O * sizeof(result[0]));
    for (size_t i = 0; i < values_count; i++)
    {
        if (inst < 2)
        {
            insts[inst] = values[i];
            inst++;
        }
        else
        {
            char instruction[2] = {(char)insts[0], (char)insts[1]};
            uint8_t command = (uint8_t)strtol(instruction, NULL, 16);
            switch (command)
            {
            case 0x1A:
                // MOV REG1,REG2
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
                break;
            case 0x1B:
                // MOV REG, ADDR
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
                        result[out_count].ARGS.reg1 = values[i+1];
                        for_phys[8] = '\0';
                        i += 9;
                        
                        set_vaddr(result,out_count,for_phys);
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
                break;
            case 0x01:
                // ADD REG, REG
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
                break;
            case 0x02:
                // ADD REG, ADDR
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
                        result[out_count].ARGS.reg1 = values[i+1];
                        for_phys[8] = '\0';
                        i += 9;
                        set_vaddr(result,out_count,for_phys);
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
                break;
            case 0x91:
                // JMP ADDR
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
                    set_vaddr(result,out_count,for_phys);
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
                break;
            case 0x93:
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
                    set_vaddr(result,out_count,for_phys);
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
                break;
            case 0x95:
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
                    set_vaddr(result,out_count,for_phys);
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
                break;
            case 0x80:
                // CMP REG, REG
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
                break;
            case 0x1C:
                // MOV REG, LIT32
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
                        result[out_count].ARGS.reg1 = values[i+1];
                        for_lit[8] = '\0';
                        i += 9;
                        result[out_count].ARGS.lit32= (uint32_t)strtol(for_lit, NULL, 16);
                        
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
                break;
            default:
                add_bits(result, out_count, insts[0]);
                add_bits(result, out_count, insts[1]);
                result[out_count].hasError = true;
                result[out_count].ErrorMessage = "Unknown instruction";
                break;
            }
            inst = 0;
            out_count++;
            if (out_count == MAX_NUMBERS_O)
            {
                OUT *result2 = (OUT *)malloc(MAX_NUMBERS_O * 2 * sizeof(OUT));
                if (result2 == NULL)
                    exit(EXIT_FAILURE);
                memset(result2, 0, MAX_NUMBERS_O * sizeof(result2[0]));
                for (size_t move = 0; move < MAX_NUMBERS_O; move++)
                {
                    result2[move] = result[move];
                }
                result = result2;
                MAX_NUMBERS_O *= 2;
            }
        }
    }
    if (inst != 0)
    {
        result[out_count].hasError = true;
        result[out_count].ErrorMessage = "Readen instruction, but no operands";
        add_bits(result, out_count, insts[0]);
        if (inst == 2)
        {
            add_bits(result, out_count, insts[1]);
        }
    }

    free(values);
    return result;
}