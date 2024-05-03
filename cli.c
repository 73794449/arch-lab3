#include "cli.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define chck(str)                    \
    if (i + 1 < argc)                \
    {                                \
        my_files->str = argv[i + 1]; \
        i++;                         \
    }                                \
    else                             \
    {                                \
        return false;                \
    }

void show_help()
{
    printf("\n===Help===\n");
    printf("Required:\n");
    printf("-i or --input  <filename>     - input file\n");
    printf("-o or --output <filename>     - output file\n");
    printf("-t or --table  <filename>     - table file\n");
    printf("Optional:\n");
    printf("-q or --quiet                - quiet mode\n");
    printf("-v or --verify <filename>     - verification file\n");
    printf("-h or --help                  - this help\n");
}

bool proc_cli(int argc, char *argv[], Files *my_files)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0){
            chck(_input);
        }
        if (strcmp(argv[i], "-o") == 0){
            chck(_output);
        }
        if (strcmp(argv[i], "-t") == 0){
            chck(_table);
        }
        if (strcmp(argv[i], "-v") == 0){
            chck(_verify);
        }
        if (strcmp(argv[i], "-q") == 0){
            my_files->verbose = false;
        }
        if (strcmp(argv[i], "-h") == 0)
        {
            show_help();
            if (argc == 2)
                return false;
        }

        if (strcmp(argv[i], "--input") == 0){
            chck(_input);
        }
        if (strcmp(argv[i], "--output") == 0){
            chck(_output);
        }
        if (strcmp(argv[i], "--table") == 0){
            chck(_table);
        }
        if (strcmp(argv[i], "--verify") == 0){
            chck(_verify);
        }
        if (strcmp(argv[i], "--quiet") == 0){
            my_files->verbose = false;
        }
        if (strcmp(argv[i], "--help") == 0)
        {
            show_help();
            if (argc == 2)
                return false;
        }
    }
    bool verified = true;
    if (my_files->_input == NULL)
    {
        printf("No input file\n");
        verified = false;
    }
    if (my_files->_output == NULL)
    {
        printf("No output file\n");
        verified = false;
    }
    if (my_files->_table == NULL)
    {
        printf("No table file\n");
        verified = false;
    }
    if (my_files->_verify)
        printf("Output will be verified\n");

    return verified;
}