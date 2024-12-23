#ifndef FUN_H
#define FUN_H

#include <iostream>
typedef enum{
    NO_OPERATION,
    HELP,
    VIEW,
    EDIT,
}opr;

int operation_type(int argc, char **argv);

char *is_valid(int agrc,char **argv);

void help();

void view(const char *name);

void edit(int argc, char **argv);

char *read(char **output,FILE *fptr);

void cpy_remaining(FILE *fptr,FILE *fptr2);

int cpy_tag(FILE *fptr,FILE *fptr2);

#endif