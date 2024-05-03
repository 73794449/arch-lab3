#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cli.h"
#include "outputting.h"
#include "verify.h"

int main(int argc, char *argv[])
{
    Files my_files = {NULL, NULL, NULL, NULL, true};
    /* processing cli */
    if (proc_cli(argc, argv, &my_files))
    {
        /* starting work */
        if (my_files.verbose)
            printf("Start working\n");

        /* parsing table from CSV file */
        DSC *my_table = parseTable(my_files._table);
        if (my_files.verbose)
            printf("Table parsing is done\n");

        /* converting bytes to instructions, virtual addresses to physical*/
        outRet convStruct = convert(my_files._input, my_table);
        OUT *converted = convStruct.result;
        size_t out_count = convStruct.count;

        /* verbose output if needed */
        if (my_files.verbose)
            printOut(out_count, converted);

        /* output to file */
        writeOut(out_count, converted, my_files._output);

        /* freeing memory */
        free(converted->values);
        free(converted->ARGS.vaddr);
        free(converted);
        free(my_table);

        /* finished work */
        if (my_files.verbose)
            printf("Finished work\n");

        /* verifying work */
        if (my_files._verify != NULL)
        {
            verifyRet verified = verify(my_files._output, my_files._verify);
            if (!verified.got_error)
                printf("Verification passed\n");
            else
            {
                for (size_t i = 0; i < verified.error_count; i++)
                    printf("%s\n", verified.error[i]);
            }
        }
    }
    return 0;
}