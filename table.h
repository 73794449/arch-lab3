#include <stdint.h>
#include <stdbool.h>
#ifndef TABLE_H
#define TABLE_H
#define RDT 2048

struct descriptor
{
    uint32_t base_addr;
    uint16_t segment_size;
    bool in_memory;
    bool other[11];
};
typedef struct descriptor DSC;

DSC *parseTable(char *_table);

#endif