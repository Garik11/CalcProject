#ifndef CALC_ASSEMBLER_H
#define CALC_ASSEMBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "../GlobalHeaders/config.h"
#include "../Processor/Stack/src/recalloc/recalloc.h"
#include "../FileWork/TextReadFile.h"

void assembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT);

void toupper_all(char *str);

struct Lashes
{
    char LABEL_NAME[100];
    size_t label_ip;
};

struct UndefLashes
{
    char UNDEF_LABEL_NAME[100];
    size_t UNDEF_BUFFER_POS;
};


#endif // !CALC_ASSEMBLER_H
