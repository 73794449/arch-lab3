#include "converter.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t SIZE_RESULT = 500;
size_t SIZE_VALUES = 2000;

void add_bits(OUT *result, size_t out_c, uint8_t value)
{
    result[out_c].values_count++;
    result[out_c].values = (uint8_t *)realloc(result[out_c].values, result[out_c].values_count * sizeof(uint8_t));
    if (result[out_c].values == NULL)
        exit(EXIT_FAILURE);
    result[out_c].values[result[out_c].values_count - 1] = value;
}

void skip(FILE *reader)
{
    char c;
    do
        c = (char)fgetc(reader);
    while (!(c == EOF || c == ';'));
}

void UNKNOWN(OUT *result, uint8_t *insts, size_t out_c)
{
    add_bits(result, out_c, insts[0]);
    add_bits(result, out_c, insts[1]);
    result[out_c].hasError = true;
    result[out_c].ErrorMessage = "Unknown instruction";
}

OUT *realloc_with_memset_zero_out(OUT *p1)
{
    OUT *p2 = (OUT *)malloc(SIZE_RESULT * 2 * sizeof(OUT));
    if (p2 == NULL)
        exit(EXIT_FAILURE);

    memset(p2, 0, SIZE_RESULT * 2 * sizeof(p2[0]));
    if (p1 != NULL)
    {
        for (size_t i = 0; i < SIZE_RESULT; i++)
            p2[i] = p1[i];

        SIZE_RESULT *= 2;
    }
    return p2;
}

uint8_t *realloc_with_memset_zero_uint8(uint8_t *p1)
{
    uint8_t *p2 = (uint8_t *)malloc(SIZE_VALUES * 2 * sizeof(uint8_t));
    if (p2 == NULL)
        exit(EXIT_FAILURE);

    memset(p2, 0, SIZE_VALUES * 2 * sizeof(p2[0]));
    if (p1 != NULL)
    {
        for (size_t i = 0; i < SIZE_VALUES; i++)
            p2[i] = p1[i];

        SIZE_VALUES *= 2;
    }
    return p2;
}

valRet create_values(char *_input)
{
    size_t values_count = 0;
    FILE *reader = fopen(_input, "r");
    if (reader == NULL)
        exit(-1);
    uint8_t *values = NULL;
    values = realloc_with_memset_zero_uint8(values);
    char c = '\0';
    do
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

                if (values_count == SIZE_VALUES)
                    values = realloc_with_memset_zero_uint8(values);
            }
        }
    } while (c != EOF);
    fclose(reader);
    valRet ret = {values_count, values};
    return ret;
}