#ifndef CALC_PROCESSOR_H
#define CALC_PROCESSOR_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "./Stack/src/Stack/Stack.h"
#include "./Stack/src/Stack/StackHidden.h"
#include "../GlobalHeaders/Calc.h"

#define CALC_SET_ERROR(сondition, сondition_name) ((сondition) == 0) ? сondition_name : CALC_ALL_OK

enum ProcessorErrors{
    CALC_ALL_OK                 = 0 << 0,
    CALC_PUSH_ERROR             = 1 << 0,
    CALC_POP_ERROR              = 1 << 1,
    CALC_ADD_ERROR              = 1 << 2,
    CALC_DIV_ERROR              = 1 << 3,
    CALC_SUB_ERROR              = 1 << 4,
    CALC_OUT_ERROR              = 1 << 5,
    CALC_MUL_ERROR              = 1 << 6,
    CALC_SIN_ERROR              = 1 << 7,
    CALC_COS_ERROR              = 1 << 8,
    CALC_SQRT_ERROR             = 1 << 9,
    CALC_IN_ERROR               = 1 << 10,
};

typedef size_t CalcErrorsBitmask;

void calc(const char* FILE_NAME, CalcErrorsBitmask *outerror = NULL);

#endif // !CALC_PROCESSOR_H