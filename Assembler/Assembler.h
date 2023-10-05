#ifndef CALC_ASSEMBLER_H
#define CALC_ASSEMBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../GlobalHeaders/Calc.h"

void compile(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT);

#endif // !CALC_ASSEMBLER_H
