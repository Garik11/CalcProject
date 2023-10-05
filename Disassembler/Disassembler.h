#ifndef CALC_DISASSEMBLER_H
#define CALC_DISASSEMBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../GlobalHeaders/Calc.h"

void decompile(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT);

#endif // !CALC_DISASSEMBLER_H