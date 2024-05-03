#include "outputting.h"

void writeOut(size_t out_count, OUT *converted, char *_output)
{
    FILE *out_to = fopen(_output, "w");
    if (out_to == NULL)
        exit(-1);
    for (size_t i = 0; i < out_count; i++)
    {
        int p = 0;
        for (size_t k = 0; k < converted[i].values_count; k++)
        {
            fprintf(out_to, "%c", (char)converted[i].values[k]);
            p++;
            if (p == 2)
            {
                fprintf(out_to, " ");
                p = 0;
            }
        }
        fprintf(out_to, "\n");
        if (converted[i].hasError)
            fprintf(out_to, "%s\n", converted[i].ErrorMessage);
        else
        {
            fprintf(out_to, "%s ", converted[i].COM);
            if (converted[i].ARGS.hasReg1)
            {
                char val[2] = {(char)converted[i].ARGS.reg1, '\0'};
                fprintf(out_to, "R%lu", strtol(val, NULL, 16));
            }
            if (converted[i].ARGS.hasReg2)
            {
                char val[2] = {(char)converted[i].ARGS.reg2, '\0'};
                fprintf(out_to, ", R%lu", strtol(val, NULL, 16));
            }
            if (converted[i].ARGS.hasLit32)
            {
                char buff[100];
                buff[0] = '0';
                snprintf(buff, 100, "%" PRIx32, converted[i].ARGS.lit32);
                if (isalpha(buff[0]))
                    fprintf(out_to, ", 0");
                else
                    fprintf(out_to, ", ");
                fprintf(out_to, "%" PRIx32 "h", converted[i].ARGS.lit32);
            }
            if (converted[i].ARGS.hasAddr)
            {
                if (converted[i].ARGS.hasReg1)
                    fprintf(out_to, ", ");
                fprintf(out_to, "[0x%" PRIx32 "]", converted[i].ARGS.addr);
            }
            fprintf(out_to, "\n");
        }
    }
    fclose(out_to);
}