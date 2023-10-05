#ifndef CALC_PROCESSOR_H
#define CALC_PROCESSOR_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "./Stack/src/Stack/Stack.h"
#include "./Stack/src/Stack/StackHidden.h"
#include "../GlobalHeaders/Calc.h"

void calc(const char* FILE_NAME);

#endif // !CALC_PROCESSOR_H