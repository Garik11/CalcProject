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

/*Max text label size*/
static const size_t MAX_MARK_TEXT_SIZE = 100;

/*Declared text labels*/
struct Lashes
{
    char label_name[MAX_MARK_TEXT_SIZE];
    size_t label_ip;
};

/*Undeclared text labels
Made to avoid a double pass through the code*/
struct UndefLashes
{
    char label_name[MAX_MARK_TEXT_SIZE];
    size_t label_pos;
};

/*Max asm fubc name size*/
static const size_t MAX_FUNC_NAME_SIZE = 30;

/*Max argument size (double can take up a lot of space)*/
static const size_t MAX_ARGUMENT_SIZE = 300;

/*Start buffer size, if the size is not enough,
it is multiplied by a constant : Buffer multiplier*/
static const size_t START_BUFFER_SIZE = 1000;

/*Buffer multiplier*/
static const size_t BUFFER_MULTIPLIER = 2;

/*INACCURACY of buffer size for mult*/
static const size_t INACCURACY = 4;

/*MAX LABEL SIZE*/
static const size_t MAX_LABEL_NUMBER = 1000;

/*the extra byte for the last character*/
static const size_t EXTRA_BYTE = 1;

/*assemble text to eq file*/
void assembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT);

/*to toupper all alp chars*/
void toupper_all(char *str);

/*scan an argument and сonverting an argument to code*/
void argument_scaner(  
                            char*           outbuffer           , 
                            size_t*         outbuffer_offset    , 
                            int64_t         bytecode            , 
                            char*           argument            , 
                            size_t          argument_size       , 
                            UndefLashes*    undeflabels         , 
                            size_t*         undeflabelspos
                        );

/*scanf with saving pos and with saving the size of the read line*/
int sscanf_s_fidex_n(
                        const char*     input       , 
                        char*           output      , 
                        size_t*         input_offset, 
                        size_t*         output_size
                    );

#endif // !CALC_ASSEMBLER_H