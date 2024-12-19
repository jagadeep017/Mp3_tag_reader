#ifndef FUN_H
#define FUN_H

#include <iostream>
typedef enum{
    NO_OPERATION,
    HELP,
    VIEW,
    EDIT,
}opr;

typedef class Mp3{
    public:
    char name[20];
    char ver[2];
    char flags;
    char *buffer;
    Mp3(){
        ver[0]='\0';
        ver[1]='\0';
        flags=0;
    }
}mp3;

int operation_type(int argc, char **argv);

void help();

void view(int argc, char **argv,mp3 *obj);

void edit(int argc, char **argv);

void read(char **output,FILE *fptr);

void cpy_remaining(FILE *fptr,FILE *fptr2);

int cpy_tag(FILE *fptr,FILE *fptr2);

#endif