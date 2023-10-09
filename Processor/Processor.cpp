#include "Processor.h"

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

void ProcessorDump(     ProcStruct      procs       , 
                        ProcessorError  errors      , 
                        const char*     PROCS_NAME  ,
                        const char*     FILE        , 
                        int             LINE        , 
                        const char*     FUNC
                    )
{
    if(PROCS_NAME != NULL && FILE != NULL && FUNC != NULL)
        printf("Procs \"%s\" called from %s(%d) in %s\n", PROCS_NAME , FILE, LINE, FUNC);
    else
        printf("Procs ??? called from ???(???) in ???\n");

    printf("code_size   = %lu\n", procs.code_size);
    printf("ip          = %lu\n", procs.ip);

    printf("{\n");
    printf("rax = ""%" ProcesseorSpecificator "\n", procs.reg[0]);
    printf("rbx = ""%" ProcesseorSpecificator "\n", procs.reg[1]);
    printf("rcx = ""%" ProcesseorSpecificator "\n", procs.reg[2]);
    printf("rdx = ""%" ProcesseorSpecificator "\n", procs.reg[3]);
    printf("}\n");

    if(procs.code_size <= procs.ip)
        printf("IP pos error! ip = %lu\n", procs.ip);

    if(procs.code != NULL){
        printf("code[%p] = {\n", procs.code);
        for(size_t pos = procs.ip - 10; pos < procs.ip + 10; pos++)
            printf("%"SpecificatorSize"lu ", pos);
        putchar('\n');
        for(size_t pos = procs.ip - 10; pos < procs.ip + 10; pos++)
            printf("%" SpecificatorSize ".0"ProcesseorSpecificator " ", ((ProcessorContainer*)procs.code)[pos]);
        putchar('\n');
        if(procs.code_size > procs.ip)
            for(size_t pos = procs.ip - 10; pos < procs.ip; pos++)
                printf("%"SpecificatorSize"c ", ' ');
            printf("^^^^\n");
        printf("}\n");
    }
    
    StackDump(procs.stk, STACK_ALL_OK, "procs.stk", FILE, LINE, FUNC);

    assert(procs.code != NULL);
    assert(procs.stk  != NULL);
    assert(procs.ip   < procs.code_size);
    assert(FILE != NULL);
    assert(FUNC != NULL);
}

ProcStruct ProcessorCtor (char** text, size_t lines){
    ProcStruct outproc = {};

    outproc.reg[0] = outproc.reg[1] = outproc.reg[2] = outproc.reg[3] = 0;

    outproc.ip          = IP_START_POS      ;
    outproc.code_size   = CODE_START_SIZE   ;
    ProcessorGetCode(&outproc, text, lines) ;

    StackErrorsBitmask stackerror = STACK_ALL_OK;
    outproc.stk = STACK_CTOR(&stackerror);
    assert(stackerror == STACK_ALL_OK);

    return outproc;
}

void ProcessorDtor(ProcStruct procs){
    assert(procs.code != NULL);
    free(procs.code);
    procs.ip = PROC_POISON_HEX_NUM;
    procs.code_size = PROC_POISON_HEX_NUM;
    procs.reg[0] = procs.reg[1] = procs.reg[2] = procs.reg[3] = PROC_POISON_NUM;
    StackDtor(procs.stk);
}

static inline void    PUSH    (Stack* stk, double value);
static inline double  POP     (Stack* stk);
static inline void    ADD     (Stack* stk);
static inline void    DIV     (Stack* stk);
static inline void    SUB     (Stack* stk);
static inline void    MUL     (Stack* stk);
static inline void    SIN     (Stack* stk);
static inline void    COS     (Stack* stk);
static inline void    SQRT    (Stack* stk);
static inline void    OUT     (Stack* stk);
static inline void    IN      (Stack* stk);

static double atof_with_saves_offset(const char* str, size_t* pos){
    assert(pos != NULL);

    while(isspace(*str))str++;

    double output = 0;
    double exp = 0.1;
    *pos = 0;

    while(isdigit(*str)){
        output *= 10;
        output += (double)(*str - '0');
        str++, ++*pos;        
    }
    if(*str == '.'){

        str++, ++*pos;
        while(isdigit(*str)){
            output += (double)(*str - '0') * exp;
            exp /= 10;
            str++, ++*pos;
        }
    }
    return output;
}

void ProcessorGetCode(ProcStruct *procs, char** text, size_t lines){
    assert(procs != NULL);
    assert(text  != NULL);

    procs->code = (char*)calloc(procs->code_size, sizeof(ProcessorContainer));
    assert(procs->code != NULL);

    ProcessorContainer value = {}, argument = {};

    size_t pt_status = 0;

    for(size_t pos = 0; pos < lines; pos++){

        if((pt_status + 4) * sizeof(ProcessorContainer) >= procs->code_size){
            char* new_code = (char*)recalloc(procs->code, 
                                            sizeof(ProcessorContainer), 
                                            procs->code_size * PROC_CODE_MULT,
                                            sizeof(ProcessorContainer), 
                                            procs->code_size
                                            );
            assert(new_code != NULL);
            procs->code_size *= PROC_CODE_MULT;
            procs->code = new_code;
        }

        size_t offset = 0;
        value = atof_with_saves_offset(text[pos], &offset);
        ((ProcessorContainer*)procs->code)[pt_status++] = value;

        if((uint64_t)value == PUSH_C){
            argument = atof(text[pos] + offset);
            ((ProcessorContainer*)procs->code)[pt_status++] = argument;
        }
    }
}

void processor(char **text, size_t lines){
    assert(text != NULL);

    ProcStruct pr = ProcessorCtor(text, lines);
    ProcessorGetCode(&pr, text, lines);

    size_t IP_MAX = pr.code_size / sizeof(ProcessorContainer);

    while(pr.ip < IP_MAX){
        PROCESSOR_DUMP(pr, PROC_ALL_OK);
        double value = 0;
        printf("NOW CODE:%d\n", (uint64_t)((double*)pr.code)[pr.ip]);
        switch ((uint64_t)((double*)pr.code)[pr.ip++]){
        case PUSH_C:
            PUSH(pr.stk, ((double*)pr.code)[pr.ip++]);
            break;
        case POP_C:
            POP(pr.stk);
            break;
        case IN_C:
            IN(pr.stk);
            break;
        case ADD_C:
            ADD(pr.stk);
            break;
        case SUB_C:
            SUB(pr.stk);
            break;
        case MUL_C:
            MUL(pr.stk);
            break;
        case DIV_C:
            DIV(pr.stk);
            break;
        case SQRT_C:
            SQRT(pr.stk);
            break;
        case SIN_C:
            SIN(pr.stk);
            break;
        case COS_C:
            COS(pr.stk);
            break;
        case OUT_C:
            OUT(pr.stk);
            break;
        case HLT_C:
            goto HLT;
        default /*SIGILL_C*/:
            printf("Incorrect instruction \"%d\", terminate process!\n", (uint64_t)((double*)pr.code)[pr.ip++]);
            goto HLT;
        }
    }
    HLT:
    PROCESSOR_DUMP(pr, PROC_ALL_OK);

    ProcessorDtor(pr);
}

static inline void PUSH(Stack* stk, double value){
    StackErrorsBitmask  stackerror = STACK_ALL_OK;
    StackPush(stk, value, &stackerror);
    assert(stackerror == STACK_ALL_OK);
}

static inline double POP(Stack* stk){
    StackErrorsBitmask  stackerror = STACK_ALL_OK;
    double outnum = StackPop(stk, &stackerror);
    assert(stackerror == STACK_ALL_OK);
    return outnum;
}

static inline void ADD(Stack* stk){
    double first_value  = POP(stk);
    double second_value = POP(stk);
    PUSH(stk, first_value + second_value);
}


static inline void DIV(Stack* stk){
    double first_value  = POP(stk);
    double second_value = POP(stk);
    PUSH(stk, second_value / first_value);
}

static inline void SUB(Stack* stk){
    double first_value  = POP(stk);
    double second_value = POP(stk);
    PUSH(stk, second_value - first_value);
}

static inline void MUL(Stack* stk){
    double first_value  = POP(stk);
    double second_value = POP(stk);
    PUSH(stk, second_value * first_value);
}

static inline void SIN(Stack* stk){
    PUSH(stk, sin(POP(stk)));
}

static inline void COS(Stack* stk){
    PUSH(stk, cos(POP(stk)));
}

static inline void SQRT(Stack* stk){
    PUSH(stk, sqrt(POP(stk)));
}

static inline void OUT(Stack* stk){
    double value  = POP(stk);
    PUSH(stk, value);
    printf("Your value : %lf\n", value);

}

static inline void IN(Stack* stk){
    double value = {};
    printf("Enter value:\n");
    assert(scanf("%lg", &value) == 1);
    PUSH(stk, value);
}