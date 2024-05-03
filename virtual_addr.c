#include "virtual_addr.h"
#include <stdlib.h>
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