#include "table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXCHAR 100


DSC* parseTable(char* _table)
{
    DSC* Table;
    Table = (DSC*)malloc(RDT*sizeof(DSC));
    memset(Table,0,RDT*sizeof(Table[0]));
    FILE* reader = fopen(_table, "r");
    if(reader == NULL)
        exit(-1);

    char row[MAXCHAR];
    char *token;
    while(feof(reader) != true)
    {
        fgets(row, MAXCHAR, reader);
        token  = strtok(row, "|");
        size_t id = atoi(token);
        size_t parse = 0;
        while(token != NULL)
        {
            token = strtok(NULL, "|");
            switch(parse)
            {
                case 0:
                    Table[id].base_addr = strtoull(token, NULL, 0);
                    break;
                case 1:
                    Table[id].segment_size = atoi(token);
                    break;
                case 2:
                    Table[id].in_memory = atoi(token);
                    break;
                default:
                    break;
            };
            parse++;
        }
    }

    return Table;
}