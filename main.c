#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "file_dest.h"
#include "converter.h"
#include <inttypes.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    Files my_files;
    if (argc == 7)
    {
        // Hardcoded CLI params order -i <inputfile> -o <outputfile> -t <tablefile>
        if (strcmp(argv[1], "-i") == 0 &&
            strcmp(argv[3], "-o") == 0 &&
            strcmp(argv[5], "-t") == 0)
        {
            my_files._input = argv[2];
            my_files._output = argv[4];
            my_files._table = argv[6];
        }
    }
    else
    {
        printf("No input/output/table file destination \n");
        exit(0);
    }

    printf("Start working\n");
    DSC *my_table = parseTable(my_files._table);
    printf("Table parsing is done\n");
    OUT *converted = convert(my_files._input, my_table);
    printf("Results output created\n");
    printf("----------------------\n");
    for (size_t i = 0; i < out_count; i++)
    {
        int p = 0;
        for (size_t k = 0; k < converted[i].values_count; k++)
        {
            printf("%c", (char)converted[i].values[k]);
            p++;
            if (p == 2)
            {
                printf(" ");
                p = 0;
            }
        }
        printf("\n");
        if (converted[i].hasError)
        {
            printf("%s\n", converted[i].ErrorMessage);
            if(converted[i].ARGS.hasAddr){
                printf("Virtual address:%s\n", converted[i].ARGS.vaddr);
                printf("Seg id: 0x%" PRIx16 "\n",converted[i].ARGS.seg);
                printf("Seg offset: 0x%" PRIx32 "\n", converted[i].ARGS.offset);
            }
        }
        else
        {
            printf("%s ", converted[i].COM);
            if (converted[i].ARGS.hasReg1)
            {
                char val[2] = {(char)converted[i].ARGS.reg1, '\0'};
                printf("R%lu", strtol(val, NULL, 16));
            }
            if (converted[i].ARGS.hasReg2)
            {
                char val[2] = {(char)converted[i].ARGS.reg2, '\0'};
                printf(", R%lu", strtol(val, NULL, 16));
            }
            if(converted[i].ARGS.hasLit32)
            {
                char buff[100];
                buff[0] = '0';
                snprintf(buff, 100, "%" PRIx32, converted[i].ARGS.lit32);
                if(isalpha(buff[0]))
                    printf(", 0");
                else
                    printf(", ");
                printf("%" PRIx32 "h", converted[i].ARGS.lit32);
            }
            if(converted[i].ARGS.hasAddr)
            {
                if(converted[i].ARGS.hasReg1)
                {
                    printf(", ");
                }
                printf("[0x%" PRIx32"]",converted[i].ARGS.addr);
            }
            printf("\n");
        }
        printf("----------------------\n");
    }
    return 0;
}