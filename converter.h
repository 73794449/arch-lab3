#include "physical_addr.h"
#include "virtual_addr.h"
#include "values_return_struct.h"
#include <stdio.h>

#ifndef CONVERTER_H
#define CONVERTER_h

/* converter */
outRet convert(char *_input, DSC *table);

/* converter-utils */
extern size_t SIZE_RESULT;
extern size_t SIZE_VALUES;

void add_bits(OUT *result, size_t out_c, uint8_t value);
void skip(FILE *reader);
void UNKNOWN(OUT *result, uint8_t *insts, size_t out_c);
OUT *realloc_with_memset_zero_out(OUT *p1);
uint8_t *realloc_with_memset_zero_uint8(uint8_t *p1);
valRet create_values(char *_input);

/* converter-commands */

size_t MOVREGREG(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, size_t out_count);
size_t MOVREGADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count);
size_t ADDREGREG(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, size_t out_count);
size_t ADDREGADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count);
size_t JMPADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count);
size_t JLADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count);
size_t JGADDR(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, DSC *table, size_t out_count);
size_t CMPREGREG(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, size_t out_count);
size_t MOVREGLIT32(OUT *result, uint8_t *values, size_t values_count, size_t i, uint8_t *insts, size_t out_count);

/* converter-commands-macro*/

#define mov1a() MOVREGREG(result, values, values_count, i, insts, out_count)
#define mov1b() MOVREGADDR(result, values, values_count, i, insts, table, out_count)
#define add01() ADDREGREG(result, values, values_count, i, insts, out_count)
#define add02() ADDREGADDR(result, values, values_count, i, insts, table, out_count)
#define jmp91() JMPADDR(result, values, values_count, i, insts, table, out_count)
#define jl93() JLADDR(result, values, values_count, i, insts, table, out_count)
#define jg95() JGADDR(result, values, values_count, i, insts, table, out_count)
#define cmp80() CMPREGREG(result, values, values_count, i, insts, out_count)
#define mov1c() MOVREGLIT32(result, values, values_count, i, insts, out_count)

#endif