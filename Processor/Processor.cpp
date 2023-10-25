#include "Processor.h"

static inline ProcessorArgumentType DO_POP  (Stack* stk                             );
static inline void                  DO_PUSH (Stack* stk, ProcessorArgumentType value);

static inline void      DO_ADD      (Stack* stk);
static inline void      DO_DIV      (Stack* stk);
static inline void      DO_SUB      (Stack* stk);
static inline void      DO_MUL      (Stack* stk);
static inline void      DO_SIN      (Stack* stk);
static inline void      DO_COS      (Stack* stk);
static inline void      DO_SQRT     (Stack* stk);
static inline void      DO_OUT      (Stack* stk);
static inline void      DO_IN       (Stack* stk);

ProcessorArgumentType checkArgument(ProcStruct pr, ProcessorContainer nowcode){
        if(nowcode & MEM_BIT){
            if(nowcode & REG_BIT){
                size_t R_STATUS = (size_t)((nowcode & MASK_REG) >> REG_BITS);
                assert(R_STATUS < NUMBER_OF_REGISTERS);
                ProcessorArgumentType pushednum = {};
                size_t offset = (size_t)pr.reg[R_STATUS] * sizeof(ProcessorArgumentType);
                memcpy(&pushednum, pr.MEM + offset, sizeof(ProcessorArgumentType));
                DO_PUSH(pr.stk, pushednum);
                return pushednum;
            }
            else if(nowcode & NUM_BIT){
                ProcessorArgumentType pushednum = {};
                size_t offset = (size_t)(((ProcessorArgumentType*)pr.code)[pr.ip]) * sizeof(ProcessorArgumentType);
                memcpy(&pushednum, pr.MEM +  offset, sizeof(ProcessorArgumentType));
                DO_PUSH(pr.stk, pushednum);
                pr.ip++;
                return pushednum;
            }
        }
        else if(nowcode & REG_BIT){
            size_t R_STATUS = (size_t)((nowcode & MASK_REG) >> REG_BITS);
            assert(R_STATUS < NUMBER_OF_REGISTERS);
            return pr.reg[R_STATUS];
        }
        else if(nowcode & NUM_BIT){
            ProcessorArgumentType pushednum = {};
            size_t offset = pr.ip * sizeof(ProcessorArgumentType);
            memcpy(&pushednum, pr.code + offset, sizeof(ProcessorArgumentType));
            pr.ip++;
            return pushednum;
        }
}

ProcStruct ProcessorCtor (const char* FILE_NAME){
    ProcStruct outproc = {};

    memset(&outproc.reg, 0, NUMBER_OF_REGISTERS * sizeof(ProcessorArgumentType));

    outproc.ip          = IP_START_POS      ;
    outproc.code_size   = CODE_START_SIZE   ;

    outproc.MEM = (char*)calloc(PROC_MEM_SIZE, sizeof(char));
    assert(outproc.MEM != NULL);

    ProcessorGetCode(&outproc, FILE_NAME);

    StackErrorsBitmask stackerror = STACK_ALL_OK;
    outproc.stk = STACK_CTOR(&stackerror);
    assert(stackerror == STACK_ALL_OK);

    outproc.call_stk = STACK_CTOR(&stackerror);
    assert(stackerror == STACK_ALL_OK);

    return outproc;
}

void ProcessorDtor(ProcStruct procs){
    assert(procs.code != NULL);

    free(procs.code - sizeof(AUTHORS_NAME) - sizeof(VERSION));
    free(procs.MEM);

    procs.ip        = PROC_POISON_HEX_NUM;
    procs.code_size = PROC_POISON_HEX_NUM;

    for(size_t i = 0; i < NUMBER_OF_REGISTERS; i++)
        procs.reg[i] = PROC_POISON_NUM;
    
    StackDtor(procs.stk);
    StackDtor(procs.call_stk);
}

void ProcessorGetCode(ProcStruct* procs, const char* FILE_NAME){

    assert(FILE_NAME != NULL);

    size_t file_size = {};
    assert(fsize(FILE_NAME, &file_size) == FSIZE_SUCCES);
    
    procs->code = (char*)calloc(file_size, sizeof(ProcessorContainer));
    assert(procs->code != NULL);

    FILE* fp = fopen(FILE_NAME, "rb");
    assert(fp != NULL);
    assert(fread(procs->code, file_size, sizeof(ProcessorContainer), fp) == FREAD_SUCCES);

    int file_ownership = strncmp(procs->code, AUTHORS_NAME, sizeof(AUTHORS_NAME));
    if(file_ownership != 0){
        printf("Incorrect author %s %s!\n", AUTHORS_NAME, procs->code);
        assert(file_ownership == 0);
    }
    procs->code += sizeof(AUTHORS_NAME);

    int file_versionship = strncmp(procs->code, VERSION, sizeof(VERSION));
    if(file_versionship != 0){
        printf("Incorrect version %s %s\n", VERSION, procs->code);
        assert(file_versionship == 0);
    }
    procs->code += sizeof(VERSION);

    fclose(fp);
}

ProcessorError ProcessorVerificator(ProcStruct procs){

    ProcessorError outerror = PROC_ALL_OK;

    assert(procs.code != NULL           );
    assert(procs.stk  != NULL           );
    assert(procs.ip   < procs.code_size );

    if(procs.code == NULL           ) outerror |= PROC_CODE_ERROR   ;
    if(procs.stk  == NULL           ) outerror |= PROC_STACK_ERROR  ;
    if(procs.ip   >= procs.code_size) outerror |= PROC_IP_POS_ERROR ;

    return outerror;
}

void ProcessortOutAllErrors(ProcessorError  errors){
    if(errors & PROC_CODE_ERROR     ) print_error(PROC_CODE_ERROR     );
    if(errors & PROC_STACK_ERROR    ) print_error(PROC_STACK_ERROR    );
    if(errors & PROC_IP_POS_ERROR   ) print_error(PROC_IP_POS_ERROR   );
}

void ProcessorDump(     
                        ProcStruct      procs       , 
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

    printf("\tcode_size   = %lu\n", procs.code_size);
    printf("\tip          = %lu\n", procs.ip);

    printf("\t{\n");
    for(size_t i = 0; i < NUMBER_OF_REGISTERS; i++)
        printf("\t\tr%cx = %lf \n", (int)('a' + i), procs.reg[i]);
    printf("\t}\n");

    if(procs.code_size <= procs.ip)
        printf("IP pos error! ip = %lu\n", procs.ip);

    if(procs.code != NULL){
        printf("code[%p] = {\n", procs.code);
        if(procs.ip > CODE_OUTPUT_INTERVAL){
            for(size_t pos = procs.ip - CODE_OUTPUT_INTERVAL; pos < procs.ip + CODE_OUTPUT_INTERVAL; pos++)
                printf("%"SpecificatorSize"lu ", pos);
            putchar('\n');
            for(size_t pos = procs.ip - CODE_OUTPUT_INTERVAL; pos < procs.ip + CODE_OUTPUT_INTERVAL; pos++){
                static ProcessorContainer outnum = {};
                memcpy(&outnum, procs.code + pos * sizeof(ProcessorContainer), sizeof(ProcessorContainer));
                printf("%4ld ", outnum & MASK_CODE);
            }
            putchar('\n');
            for(size_t pos = procs.ip - CODE_OUTPUT_INTERVAL; pos < procs.ip + CODE_OUTPUT_INTERVAL; pos++){
                static ProcessorArgumentType outnum = {};
                memcpy(&outnum, procs.code + pos * sizeof(ProcessorArgumentType), sizeof(ProcessorArgumentType));
                printf("%4.0lf ", outnum);
            }
            putchar('\n');
            if(procs.code_size > procs.ip){
                for(size_t pos = procs.ip - CODE_OUTPUT_INTERVAL; pos < procs.ip; pos++)
                    printf("%"SpecificatorSize"c ", ' ');
                printf("^^^^\n");
            }
        } 
        else {
            for(size_t pos = 0; pos < procs.ip + CODE_OUTPUT_INTERVAL; pos++)
                printf("%"SpecificatorSize"lu ", pos);
            putchar('\n');
            for(size_t pos = 0; pos < procs.ip + CODE_OUTPUT_INTERVAL; pos++){
                ProcessorContainer out = {};
                memcpy(&out, procs.code + pos * sizeof(ProcessorContainer), sizeof(ProcessorContainer));
                out &= MASK_CODE;
                printf("%" SpecificatorSize ProcesseorSpecificator " ", out);
            }
            putchar('\n');
            if(procs.code_size > procs.ip){
                for(size_t pos = 0; pos < procs.ip; pos++)
                    printf("%"SpecificatorSize"c ", ' ');
                printf("^^^^\n");
            }
        }
        if(procs.MEM != NULL){
            printf("MEM: \n");
            size_t memside = sqrt(PROC_MEM_SIZE / sizeof(ProcessorArgumentType));
            for(size_t i = 0; i < memside; i++){
                putchar('\t');
                for(size_t j = 0; j < memside; j++){
                    static ProcessorArgumentType outnum = {};
                    memcpy(&outnum, procs.MEM + (i * memside + j) * sizeof(ProcessorArgumentType), sizeof(ProcessorArgumentType));
                    printf("%2.2lg ", outnum);
                }
                putchar('\n');
            }
        }
        printf("}\n");
    }
    
    StackDump(procs.stk, STACK_ALL_OK, "procs.stk", FILE_NAME, LINE_NUM, FUNC_NAME);

    assert(procs.code != NULL           );
    assert(procs.stk  != NULL           );
    assert(procs.ip   < procs.code_size );
    assert(FILE_NAME != NULL            );
    assert(FUNC_NAME != NULL            );
}


#warning move near switch, then undef
#define DEF_CMD(name, num, args, ...)       \
    case num:                               \
        __VA_ARGS__                         \
        break;                                

void processor(const char* FILE_NAME){
    assert(FILE_NAME != NULL);

    ProcStruct pr = ProcessorCtor(FILE_NAME);

    const size_t IP_MAX = pr.code_size / sizeof(ProcessorContainer);

    while(pr.ip < IP_MAX){

        ProcessorContainer nowcode = {};
        memcpy(&nowcode, pr.code + pr.ip * sizeof(ProcessorContainer), sizeof(ProcessorContainer));

        pr.ip++;
        printf("NOW INST: %ld \n", nowcode & MASK_CODE);
        PROCESSOR_DUMP(pr, PROC_ALL_OK);
        /*
        int64_t b = nowcode;
        while(b){
            if(b & 1)
                printf("1");
            else
                printf("0");
            b >>= 1;
        }*/
        //putchar('\n');
        switch (nowcode & MASK_CODE){
            #include "../GlobalHeaders/DSL.h"
            default /*SIGILL_C*/:
                printf("Incorrect instruction \"""%" ProcesseorSpecificator "\", terminate process!\n", nowcode & MASK_CODE);
                goto HLT;
        }
    }
    HLT:
        void(0); //del

    //PROCESSOR_DUMP(pr, PROC_ALL_OK);

    ProcessorDtor(pr);
}

static inline void DO_PUSH(Stack* stk, ProcessorArgumentType value){
    StackErrorsBitmask  stackerror = STACK_ALL_OK;
    StackPush(stk, value, &stackerror);
    assert(stackerror == STACK_ALL_OK);
}

static inline ProcessorArgumentType DO_POP(Stack* stk){
    StackErrorsBitmask  stackerror = STACK_ALL_OK;
    ProcessorArgumentType outnum = StackPop(stk, &stackerror);
    assert(stackerror == STACK_ALL_OK);
    return outnum;
}

#warning for simple ops, move this block to cmd define
static inline void DO_ADD(Stack* stk){
    ProcessorArgumentType first_value  = DO_POP(stk);
    ProcessorArgumentType second_value = DO_POP(stk);
    DO_PUSH(stk, first_value + second_value);
}

static inline void DO_DIV(Stack* stk){
    ProcessorArgumentType first_value  = DO_POP(stk);
    ProcessorArgumentType second_value = DO_POP(stk);
    DO_PUSH(stk, second_value / first_value);
}

static inline void DO_SUB(Stack* stk){
    ProcessorArgumentType first_value  = DO_POP(stk);
    ProcessorArgumentType second_value = DO_POP(stk);
    DO_PUSH(stk, second_value - first_value);
}

static inline void DO_MUL(Stack* stk){
    ProcessorArgumentType first_value  = DO_POP(stk);
    ProcessorArgumentType second_value = DO_POP(stk);
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
    ProcessorArgumentType value  = DO_POP(stk);
    DO_PUSH(stk, value);
    printf("Your value : %lf\n", value);

}

static inline void DO_IN(Stack* stk){
    ProcessorArgumentType value = {};
    printf("Enter value:\n");
    assert(scanf("%lg", &value) == 1);
    DO_PUSH(stk, value);
}