#include "converter.h"
#include <inttypes.h>
#include <ctype.h>
#ifndef OUTPUTTING_H
#define OUTPUTTING_H

void printOut(size_t out_count, OUT* converted);
void writeOut(size_t out_count, OUT* converted, char* _output);

#endif