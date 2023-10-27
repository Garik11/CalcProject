#ifndef CALC_PROCESSOR_H
#define CALC_PROCESSOR_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "./Stack/src/Stack/Stack.h"
#include "./Stack/src/recalloc/recalloc.h"
#include "../FileWork/TextReadFile.h"
#include "../GlobalHeaders/config.h"

#define PROCESSOR_DUMP(procs, errors) ProcessorDump (                           \
                                                        procs               ,   \
                                                        errors              ,   \
                                                        #procs              ,   \
                                                        __FILE__            ,   \
                                                        __LINE__            ,   \
                                                        __PRETTY_FUNCTION__     \
                                                    )

#define print_error(error) printf("ERROR: %s!\n", #error);

struct ProcStruct
{
    Stack *stk          ;

    ProcessorArgumentType reg[NUMBER_OF_REGISTERS];

    char*   code        ;
    size_t  code_size   ;
    size_t  ip          ;

    Stack *call_stk     ;

    char* MEM           ;
};

enum PROCESS_ERRORS{
    PROC_ALL_OK         = 0 << 0,
    PROC_CODE_ERROR     = 1 << 0,
    PROC_STACK_ERROR    = 1 << 1,
    PROC_IP_POS_ERROR   = 1 << 2
};

ProcStruct      ProcessorCtor       (const char* FILE_NAME                      );
void            ProcessorDtor       (ProcStruct  procs                          );
void            ProcessorGetCode    (ProcStruct *procs, const char* FILE_NAME   );

ProcessorError  ProcessorVerificator(ProcStruct procs);
void            ProcessorDump       (   
                                        ProcStruct      procs       , 
                                        ProcessorError  errors      , 
                                        const char*     PROCS_NAME  ,
                                        const char*     FILE        , 
                                        int             LINE        , 
                                        const char*     FUNC
                                    );

ProcessorArgumentType argumentRead(ProcStruct *pr, ProcessorContainer nowcode);
size_t calculateIP(const ProcStruct *pr);

void ProcessortOutAllErrors(ProcessorError  errors);

void processor(const char* FILE_NAME);

#endif // !CALC_PROCESSOR_H