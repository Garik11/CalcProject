#include "Processor.h"

static inline double    DO_POP      (Stack* stk);
static inline void      DO_PUSH     (Stack* stk, double value);
static inline void      DO_ADD      (Stack* stk);
static inline void      DO_DIV      (Stack* stk);
static inline void      DO_SUB      (Stack* stk);
static inline void      DO_MUL      (Stack* stk);
static inline void      DO_SIN      (Stack* stk);
static inline void      DO_COS      (Stack* stk);
static inline void      DO_SQRT     (Stack* stk);
static inline void      DO_OUT      (Stack* stk);
static inline void      DO_IN       (Stack* stk);

ProcStruct ProcessorCtor (const char* FILE_NAME){
    ProcStruct outproc = {};

    outproc.reg[0] = outproc.reg[1] = outproc.reg[2] = outproc.reg[3] = 0;

    outproc.ip          = IP_START_POS      ;
    outproc.code_size   = CODE_START_SIZE   ;
    ProcessorGetCode(&outproc, FILE_NAME);

    StackErrorsBitmask stackerror = STACK_ALL_OK;
    outproc.stk = STACK_CTOR(&stackerror);
    assert(stackerror == STACK_ALL_OK);

    return outproc;
}

void ProcessorDtor(ProcStruct procs){
    assert(procs.code != NULL);
    free(procs.code);
    procs.ip        = PROC_POISON_HEX_NUM;
    procs.code_size = PROC_POISON_HEX_NUM;
    procs.reg[0] = procs.reg[1] = procs.reg[2] = procs.reg[3] = PROC_POISON_NUM;
    StackDtor(procs.stk);
}

void ProcessorGetCode(ProcStruct* procs, const char* FILE_NAME){

    assert(FILE_NAME != NULL);
    printf("%s \n", FILE_NAME);

    size_t file_size = {};
    assert(fsize(FILE_NAME, &file_size) == FSIZE_SUCCES);
    
    procs->code = (char*)calloc(file_size, sizeof(ProcessorContainer));
    assert(procs->code != NULL);

    FILE* fp = fopen(FILE_NAME, "rb");
    assert(fp != NULL);
    assert(fread(procs->code, file_size, sizeof(ProcessorContainer), fp) == FREAD_SUCCES);

    fclose(fp);
}

ProcessorError ProcessorVerificator(ProcStruct procs){

    ProcessorError outerror = PROC_ALL_OK;

    assert(procs.code != NULL);
    assert(procs.stk  != NULL);
    assert(procs.ip   < procs.code_size);

    if(procs.code == NULL           ) outerror |= PROC_CODE_ERROR   ;
    if(procs.stk  == NULL           ) outerror |= PROC_STACK_ERROR  ;
    if(procs.ip   >= procs.code_size) outerror |= PROC_IP_POS_ERROR ;

    return outerror;
}

void ProcessortOutAllErrors(ProcessorError  errors){
    if(errors & PROC_CODE_ERROR) print_error(PROC_CODE_ERROR);
    if(errors & PROC_STACK_ERROR) print_error(PROC_STACK_ERROR);
    if(errors & PROC_IP_POS_ERROR) print_error(PROC_IP_POS_ERROR);
}

void ProcessorDump(     ProcStruct      procs       , 
                        ProcessorError  errors      , 
                        const char*     PROCS_NAME  ,
                        const char*     FILE_NAME   , 
                        int             LINE_NUM    , 
                        const char*     FUNC_NAME
                    )
{
    ProcessortOutAllErrors(errors);
    if(PROCS_NAME != NULL && FILE_NAME != NULL && FUNC_NAME != NULL)
        printf("Procs \"%s\" called from %s(%d) in %s\n", PROCS_NAME , FILE_NAME, LINE_NUM, FUNC_NAME);
    else
        printf("Procs \?\?\? called from \?\?\?(\?\?\?) in \?\?\?\n");

    printf("code_size   = %lu\n", procs.code_size);
    printf("ip          = %lu\n", procs.ip);

    printf("{\n");
    printf("rax = ""%" "lf" "\n", procs.reg[0]);
    printf("rbx = ""%" "lf" "\n", procs.reg[1]);
    printf("rcx = ""%" "lf" "\n", procs.reg[2]);
    printf("rdx = ""%" "lf" "\n", procs.reg[3]);
    printf("}\n");

    if(procs.code_size <= procs.ip)
        printf("IP pos error! ip = %lu\n", procs.ip);

    if(procs.code != NULL){
        printf("code[%p] = {\n", procs.code);
        if(procs.ip > 10){
            for(size_t pos = procs.ip - 10; pos < procs.ip + 10; pos++)
                printf("%"SpecificatorSize"lu ", pos);
            putchar('\n');
            for(size_t pos = procs.ip - 10; pos < procs.ip + 10; pos++){
                printf("%" SpecificatorSize ProcesseorSpecificator " ", ((ProcessorContainer*)procs.code)[pos] & MASK_CODE);
            }
            putchar('\n');
            if(procs.code_size > procs.ip){
                for(size_t pos = procs.ip - 10; pos < procs.ip; pos++)
                    printf("%"SpecificatorSize"c ", ' ');
                printf("^^^^\n");
            }
        } 
        else {
            for(size_t pos = 0; pos < procs.ip + 10; pos++)
                printf("%"SpecificatorSize"lu ", pos);
            putchar('\n');
            for(size_t pos = 0; pos < procs.ip + 10; pos++)
                printf("%" SpecificatorSize ProcesseorSpecificator " ", ((ProcessorContainer*)procs.code)[pos] & MASK_CODE);
            putchar('\n');
            if(procs.code_size > procs.ip){
                for(size_t pos = 0; pos < procs.ip; pos++)
                    printf("%"SpecificatorSize"c ", ' ');
                printf("^^^^\n");
            }
        }

        printf("}\n");
    }
    
    StackDump(procs.stk, STACK_ALL_OK, "procs.stk", FILE_NAME, LINE_NUM, FUNC_NAME);

    assert(procs.code != NULL);
    assert(procs.stk  != NULL);
    assert(procs.ip   < procs.code_size);
    assert(FILE_NAME != NULL);
    assert(FUNC_NAME != NULL);
}

#define DEF_CMD(name, rname, num, args, ...)\
    case num:                               \
        __VA_ARGS__                         \
        break;                              

void processor(const char* FILE_NAME){
    assert(FILE_NAME != NULL);

    ProcStruct pr = ProcessorCtor(FILE_NAME);

    const size_t IP_MAX = pr.code_size / sizeof(ProcessorContainer);

    while(pr.ip < IP_MAX){

        ProcessorContainer nowcode = ((ProcessorContainer*)pr.code)[pr.ip++];

        printf("NOW INST: %ld \n", nowcode & MASK_CODE);
        PROCESSOR_DUMP(pr, PROC_ALL_OK);
        
        switch (nowcode & MASK_CODE){
            #include "../GlobalHeaders/DSL.h"
            default /*SIGILL_C*/:
                printf("Incorrect instruction \"""%" ProcesseorSpecificator "\", terminate process!\n", ((ProcessorContainer*)(pr.code))[pr.ip++]);
                goto HLT;
        }
    }
    HLT:
        void(0);

    PROCESSOR_DUMP(pr, PROC_ALL_OK);

    ProcessorDtor(pr);
}

static inline void DO_PUSH(Stack* stk, double value){
    StackErrorsBitmask  stackerror = STACK_ALL_OK;
    StackPush(stk, value, &stackerror);
    assert(stackerror == STACK_ALL_OK);
}

static inline double DO_POP(Stack* stk){
    StackErrorsBitmask  stackerror = STACK_ALL_OK;
    double outnum = StackPop(stk, &stackerror);
    assert(stackerror == STACK_ALL_OK);
    return outnum;
}

static inline void DO_ADD(Stack* stk){
    double first_value  = DO_POP(stk);
    double second_value = DO_POP(stk);
    DO_PUSH(stk, first_value + second_value);
}


static inline void DO_DIV(Stack* stk){
    double first_value  = DO_POP(stk);
    double second_value = DO_POP(stk);
    DO_PUSH(stk, second_value / first_value);
}

static inline void DO_SUB(Stack* stk){
    double first_value  = DO_POP(stk);
    double second_value = DO_POP(stk);
    DO_PUSH(stk, second_value - first_value);
}

static inline void DO_MUL(Stack* stk){
    double first_value  = DO_POP(stk);
    double second_value = DO_POP(stk);
    DO_PUSH(stk, second_value * first_value);
}

static inline void DO_SIN(Stack* stk){
    DO_PUSH(stk, sin(DO_POP(stk)));
}

static inline void DO_COS(Stack* stk){
    DO_PUSH(stk, cos(DO_POP(stk)));
}

static inline void DO_SQRT(Stack* stk){
    DO_PUSH(stk, sqrt(DO_POP(stk)));
}

static inline void DO_OUT(Stack* stk){
    double value  = DO_POP(stk);
    DO_PUSH(stk, value);
    printf("Your value : %lf\n", value);

}

static inline void DO_IN(Stack* stk){
    double value = {};
    printf("Enter value:\n");
    assert(scanf("%lg", &value) == 1);
    DO_PUSH(stk, value);
}