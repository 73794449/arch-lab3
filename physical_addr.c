#include "physical_addr.h"


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