#ifndef CALC_ASSEMBLER_H
#define CALC_ASSEMBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../GlobalHeaders/asm.h"
#include "../Processor/Stack/src/recalloc/recalloc.h"
#include "../Processor/Processor.h"
#include "../FileWork/TextReadFile.h"

void assembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT);

void toupper_all(char *str);

#endif // !CALC_ASSEMBLER_H
