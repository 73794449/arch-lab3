#include "verify.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
char c = '\0';
char d = '\0';
size_t row = 0;
size_t col = 0;
size_t v_row = 0;
size_t v_col = 0;

void update_row_col(bool is_c)
{
    if (is_c)
    {
        if (c == '\n')
        {
            row++;
            col = 0;
        }
        else
            col++;
    }
    else
    {
        if (d == '\n')
        {
            v_row++;
            v_col = 0;
        }
        else
            v_col++;
    }
}

void cleanCom(FILE *read_output, FILE *read_verify)
{
    if (d == ';' || c == ';')
    {
        if (c == ';')
        {
            do
            {
                c = (char)getc(read_output);
                update_row_col(true);
            } while (c != EOF && c != ';');
            c = (char)getc(read_output);
            update_row_col(true);
            if (c == '\n'){
                c = (char)getc(read_output);
                update_row_col(true);
            }
        }
        if (d == ';')
        {
            do
            {
                d = (char)getc(read_verify);
                update_row_col(false);
            } while (d != EOF && d != ';');
            d = (char)getc(read_verify);
            update_row_col(false);
            if (d == '\n'){
                d = (char)getc(read_verify);
                update_row_col(false);
            }
        }
    }
    if (d == ';' || c == ';')
        cleanCom(read_output, read_verify);
}
verifyRet verify(char *_output, char *_verify)
{
    FILE *read_output = fopen(_output, "r");
    FILE *read_verify = fopen(_verify, "r");
    verifyRet ret = {.got_error = false, .error_count = 0};
    do
    {
        do
        {
            c = (char)getc(read_output);
            update_row_col(true);
        } while (c != EOF && !(isalnum(c) || c == ';' || c == ','));
        do
        {
            d = (char)getc(read_verify);
            update_row_col(false);
        } while (d != EOF && !(isalnum(d) || d == ';' || d == ','));

        cleanCom(read_output, read_verify);
        if (c != d)
        {
            ret.error[ret.error_count] = malloc(ERROR_SIZE * sizeof(char));
            snprintf(ret.error[ret.error_count++], ERROR_SIZE, "Output(%llu:%llu:%c) != Verify(%llu:%llu:%c), but have to", row + 1, col, c, v_row + 1, v_col, d);
            ret.got_error = true;
            if (ret.error_count == DEEP)
                return ret;
        }

    } while (!(c == EOF || d == EOF));
    return ret;
}