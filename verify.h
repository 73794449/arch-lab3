#ifndef VERIFY_H
#define VERIFY_H
#include <stdbool.h>
#include <stdlib.h>

#define DEEP 3
#define ERROR_SIZE 200

struct verify_return
{
    bool got_error;
    char* error[DEEP];
    size_t error_count;
};
typedef struct verify_return verifyRet;

verifyRet verify(char *_output, char *_verify);

#endif