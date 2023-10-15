#ifndef CALC_DISASSEMBLER_H
#define CALC_DISASSEMBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../GlobalHeaders/asm.h"
#include "../Processor/Processor.h"

#define CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(OUTPUTFILE, DECD) case DECD##_C:fprintf(outputfile,"%s\n", DECD##_S);break;

char* creacte_reg(long offset);

void disassembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT);

#endif // !CALC_DISASSEMBLER_H