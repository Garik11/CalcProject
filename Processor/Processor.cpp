#include "Processor.h"

bool doubleEqual(const double& a, const double& b, const double eps){
    return fabs(a - b) < eps;
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
static inline void DO_OUT(Stack* stk){
    ProcessorArgumentType value  = DO_POP(stk);
    DO_PUSH(stk, value);
    printf("Your value : %" ElementSpecificator "\n", value);

}
static inline void DO_IN(Stack* stk){
    ProcessorArgumentType value = {};
    printf("Enter value:\n");
    assert(scanf("%" ElementSpecificator, &value) == 1);
    DO_PUSH(stk, value);
}

#define BINARY_ARITHMETIC_OPERATION(name, OP)               \
    static inline void DO_##name(Stack* stk){               \
        ProcessorArgumentType second_value = DO_POP(stk);   \
        ProcessorArgumentType first_value  = DO_POP(stk);   \
        DO_PUSH(stk, first_value OP second_value);          \
    }

BINARY_ARITHMETIC_OPERATION(ADD, +);
BINARY_ARITHMETIC_OPERATION(SUB, -);
BINARY_ARITHMETIC_OPERATION(DIV, /);
BINARY_ARITHMETIC_OPERATION(MUL, *);

#undef BINARY_ARITHMETIC_OPERATION

#define UNARY_ARITHMETIC_FUNCTION(name, func)   \
    static inline void DO_##name(Stack* stk){   \
        DO_PUSH(stk, func(DO_POP(stk)));        \
    }

UNARY_ARITHMETIC_FUNCTION(SIN,  sin);
UNARY_ARITHMETIC_FUNCTION(COS,  cos);
UNARY_ARITHMETIC_FUNCTION(SQRT, sqrt);
UNARY_ARITHMETIC_FUNCTION(LOG, log);

#undef UNARY_ARITHMETIC_FUNCTION

size_t calculateIP(const ProcStruct *pr){

    assert(pr != NULL);

    double calculatedIP = {};

    memcpy(
            &calculatedIP                                       , 
            pr->code + pr->ip * sizeof(ProcessorArgumentType)   , 
            sizeof(ProcessorArgumentType)
        );
    
    return (size_t)calculatedIP;
}

ProcessorArgumentType argumentRead(ProcStruct *pr, const ProcessorContainer nowcode){

    assert(pr != NULL);

    ProcessorArgumentType pushednum = PROC_POISON_NUM;

    if(nowcode & MEM_BIT){
        if(nowcode & REG_BIT){

            size_t R_STATUS = (size_t)((nowcode & MASK_REG) >> REG_BITS);
            assert(R_STATUS < NUMBER_OF_REGISTERS);

            size_t offset = (size_t)pr->reg[R_STATUS] * sizeof(ProcessorArgumentType);
            memcpy(
                    &pushednum, 
                    pr->MEM + offset, 
                    sizeof(ProcessorArgumentType)
                );

        }
        else if(nowcode & NUM_BIT){

            size_t offset = calculateIP(pr) * sizeof(ProcessorArgumentType);

            memcpy(
                    &pushednum, 
                    pr->MEM +  offset, 
                    sizeof(ProcessorArgumentType)
                );
            pr->ip++;
        }
    }
    else if(nowcode & REG_BIT){

        size_t R_STATUS = (size_t)((nowcode & MASK_REG) >> REG_BITS);
        assert(R_STATUS < NUMBER_OF_REGISTERS);

        pushednum = pr->reg[R_STATUS];
    }
    else if(nowcode & NUM_BIT){

        size_t offset = pr->ip * sizeof(ProcessorArgumentType);
        memcpy(
                &pushednum, 
                pr->code + offset, 
                sizeof(ProcessorArgumentType)
            );
        
        pr->ip++;
    }
    return pushednum;
}

ProcStruct ProcessorCtor (const char* FILE_NAME){

    assert(FILE_NAME != NULL);

    ProcStruct outproc = {};

    memset(
            &outproc.reg, 
            0, 
            NUMBER_OF_REGISTERS * sizeof(ProcessorArgumentType)
        );

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

    assert(procs        != NULL);
    assert(FILE_NAME    != NULL);

    size_t file_size = {};
    assert(fsize(FILE_NAME, &file_size) == FSIZE_SUCCES);
    
    procs->code = (char*)calloc(file_size, sizeof(ProcessorContainer));
    assert(procs->code != NULL);

    FILE* fp = fopen(FILE_NAME, "rb");
    assert(fp != NULL);
    assert(fread(procs->code, file_size, sizeof(ProcessorContainer), fp) == FREAD_SUCCES);

    int file_ownership = strncmp(procs->code, AUTHORS_NAME, sizeof(AUTHORS_NAME));
    if(file_ownership != BAD_FILE_OWNERSHIP){
        printf("Incorrect author PROGRAM_ATH:%s FILE_ATH%s!\n", AUTHORS_NAME, procs->code);
        assert(file_ownership == BAD_FILE_OWNERSHIP);
    }
    procs->code += sizeof(AUTHORS_NAME);

    int file_versionship = strncmp(procs->code, VERSION, sizeof(VERSION));
    if(file_versionship != BAD_FILE_VERSRSHIP){
        printf("Incorrect version PROGRAM_VER:%s FILE_VER:%s\n", VERSION, procs->code);
        assert(file_versionship == BAD_FILE_VERSRSHIP);
    }
    procs->code += sizeof(VERSION);

    fclose(fp);
}

ProcessorError ProcessorVerificator(const ProcStruct procs){

    assert(procs.code != NULL           );
    assert(procs.stk  != NULL           );
    assert(procs.ip   < procs.code_size );

    ProcessorError outerror = PROC_ALL_OK;

    if(procs.code == NULL           ) outerror |= PROC_CODE_ERROR   ;
    if(procs.stk  == NULL           ) outerror |= PROC_STACK_ERROR  ;
    if(procs.ip   >= procs.code_size) outerror |= PROC_IP_POS_ERROR ;

    return outerror;
}

void ProcessortOutAllErrors(const ProcessorError  errors){
    if(errors & PROC_CODE_ERROR     ) print_error(PROC_CODE_ERROR     );
    if(errors & PROC_STACK_ERROR    ) print_error(PROC_STACK_ERROR    );
    if(errors & PROC_IP_POS_ERROR   ) print_error(PROC_IP_POS_ERROR   );
}

void ProcessorDump(     
                        const ProcStruct        procs       , 
                        const ProcessorError    errors      , 
                        const char*             PROCS_NAME  ,
                        const char*             FILE_NAME   , 
                        const int               LINE_NUM    , 
                        const char*             FUNC_NAME
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
        printf("\t\tr%cx = %" ElementSpecificator " \n", (int)('a' + i), procs.reg[i]);
    printf("\t}\n");

    if(procs.code_size <= procs.ip)
        printf("IP pos error! ip = %lu\n", procs.ip);

    if(procs.code != NULL){
        printf("code[%p] = {\n", procs.code);
        size_t start_pos    = {};
        size_t end_pos      = {};
        if(procs.ip > CODE_OUTPUT_INTERVAL){
            start_pos   = procs.ip - CODE_OUTPUT_INTERVAL;
            end_pos     = procs.ip + CODE_OUTPUT_INTERVAL;
        }
        else {
            start_pos = 0;
            end_pos = CODE_OUTPUT_INTERVAL * 2;
        }

        for(size_t pos = start_pos; pos < end_pos; pos++)
            printf("%"ContainerSpecSize"lu ", pos);
        putchar('\n');
        for(size_t pos = start_pos; pos < end_pos; pos++){
            static ProcessorContainer outnum = {};
            memcpy(
                    &outnum, 
                    procs.code + pos * sizeof(ProcessorContainer), 
                    sizeof(ProcessorContainer)
                );
            printf("%"ContainerSpecSize ContainerSpecificator " ", outnum & MASK_CODE);
        }
        putchar('\n');
        for(size_t pos = start_pos; pos < end_pos; pos++){
            static ProcessorArgumentType outnum = {};
            memcpy(
                    &outnum, 
                    procs.code + pos * sizeof(ProcessorArgumentType), 
                    sizeof(ProcessorArgumentType)
                );
            printf("%" ElementSpecSize ElementSpecificator " ", outnum);
        }
        putchar('\n');
        if(procs.code_size > procs.ip){
            for(size_t pos = start_pos; pos < procs.ip - FIX_INSTURCION_IP; pos++)
                printf("%"ContainerSpecSize"c ", ' ');
            printf("^^^^");
        }
        putchar('\n');

        if(procs.MEM != NULL){
            printf("MEM: \n");
            size_t memside = sqrt(PROC_MEM_SIZE / sizeof(ProcessorArgumentType));
            for(size_t i = 0; i < memside; i++){

                putchar('\t');

                for(size_t j = 0; j < memside; j++){

                    static ProcessorArgumentType outnum = {};
                    memcpy(
                            &outnum, 
                            procs.MEM + (i * memside + j) * sizeof(ProcessorArgumentType), 
                            sizeof(ProcessorArgumentType)
                        );
                    printf("%" MEMSpecSize MEMSpecificator " ", outnum);

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

void processor(const char* FILE_NAME){
    
    assert(FILE_NAME != NULL);

    ProcStruct pr = ProcessorCtor(FILE_NAME);

    const size_t IP_MAX = pr.code_size / sizeof(ProcessorContainer);

    while(pr.ip < IP_MAX){

        ProcessorContainer nowcode = {};
        memcpy(
                &nowcode, 
                pr.code + pr.ip * sizeof(ProcessorContainer), 
                sizeof(ProcessorContainer)
            );

        pr.ip++;
        printf("NOW INST: %ld \n", nowcode & MASK_CODE);
        PROCESSOR_DUMP(pr, PROC_ALL_OK);

        #define DEF_CMD(name, num, args, ...)       \
            case num:                               \
            __VA_ARGS__                             \
            break;     

        switch (nowcode & MASK_CODE){
            #include "../GlobalHeaders/DSL.h"
            default /*SIGILL_C*/:
                printf("Incorrect instruction \"""%" ContainerSpecificator "\", terminate process!\n", nowcode & MASK_CODE);
                goto HLT;
        }
        #undef DEF_CMD
    }
    HLT:
        void(0); //del

    //PROCESSOR_DUMP(pr, PROC_ALL_OK);

    ProcessorDtor(pr);
}