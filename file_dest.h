#include <stdbool.h>
#ifndef FILE_DEST_H
#define FILE_DEST_H

struct files
{
    char *_input;
    char *_output;
    char *_table;
    char *_verify;
    bool verbose;
};
typedef struct files Files;

#endif