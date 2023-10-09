#ifndef CALC_PROCESSOR_H
#define CALC_PROCESSOR_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#include "./Stack/src/Stack/Stack.h"
#include "./Stack/src/recalloc/recalloc.h"
#include "../FileWork/TextReadFile.h"
#include "../GlobalHeaders/asm.h"
#include "../GlobalHeaders/config.h"

#define PROCESSOR_DUMP(procs, errors) ProcessorDump(procs, errors, #procs, __FILE__, __LINE__, __PRETTY_FUNCTION__)

typedef double ProcessorContainer;

static uint64_t PROC_POISON_HEX_NUM = 0xDEADBABE;
static ProcessorContainer PROC_POISON_NUM = *((ProcessorContainer*)&PROC_POISON_HEX_NUM);
static const size_t PROC_CODE_MULT = 2;


#define ProcesseorSpecificator "lf"
#define SpecificatorSize        "4"

typedef uint64_t    ProcessorError;

struct ProcStruct
{
    Stack *stk;

    ProcessorContainer reg[4];

    char*   code;
    size_t  code_size;
    size_t  ip;
};

enum PROCESS_ERRORS{
    PROC_ALL_OK         = 0 << 0,
    PROC_CODE_ERROR     = 1 << 0,
    PROC_STACK_ERROR    = 1 << 1,
    PROC_IP_POS_ERROR   = 1 << 2
};

ProcStruct      ProcessorCtor       (const char* FILE_NAME);
void            ProcessorDtor       (ProcStruct procs);
void            ProcessorGetCode    (ProcStruct *pr, const char* FILE_NAME);


ProcessorError  ProcessorVerificator(ProcStruct procs);
void            ProcessorDump       (   ProcStruct      procs       , 
                                        ProcessorError  errors      , 
                                        const char*     PROCS_NAME  ,
                                        const char*     FILE        , 
                                        int             LINE        , 
                                        const char*     FUNC
                                    );


void processor(const char* FILE_NAME);

#endif // !CALC_PROCESSOR_H