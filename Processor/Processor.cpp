#include "Processor.h"

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
    procs.ip = PROC_POISON_HEX_NUM;
    procs.code_size = PROC_POISON_HEX_NUM;
    procs.reg[0] = procs.reg[1] = procs.reg[2] = procs.reg[3] = PROC_POISON_NUM;
    StackDtor(procs.stk);
}

void ProcessorGetCode(ProcStruct* procs, const char* FILE_NAME){
    size_t file_size = {};
    assert(fsize(FILE_NAME, &file_size) == FSIZE_SUCCES);
    procs->code = (char*)calloc(file_size, sizeof(ProcessorContainer));
    assert(procs->code != NULL);
    FILE* fp = fopen(FILE_NAME, "rb");
    assert(fp != NULL);
    assert(fread(procs->code, file_size, sizeof(ProcessorContainer), fp) == FREAD_SUCCES);
    printf("%lu\n", file_size);
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
                printf("%" SpecificatorSize ".0"ProcesseorSpecificator " ", ((ProcessorContainer*)procs.code)[pos] & 0x00000000FFFFFFFF);
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
                printf("%" SpecificatorSize ".0"ProcesseorSpecificator " ", ((ProcessorContainer*)procs.code)[pos] & 0x00000000FFFFFFFF);
            putchar('\n');
            if(procs.code_size > procs.ip){
                for(size_t pos = 0; pos < procs.ip; pos++)
                    printf("%"SpecificatorSize"c ", ' ');
                printf("^^^^\n");
            }
        }

        printf("}\n");
    }
    
    StackDump(procs.stk, STACK_ALL_OK, "procs.stk", FILE, LINE, FUNC);

    assert(procs.code != NULL);
    assert(procs.stk  != NULL);
    assert(procs.ip   < procs.code_size);
    assert(FILE != NULL);
    assert(FUNC != NULL);
}

void processor(const char* FILE_NAME){
    assert(FILE_NAME != NULL);

    ProcStruct pr = ProcessorCtor(FILE_NAME);

    size_t IP_MAX = pr.code_size / sizeof(ProcessorContainer);

    while(pr.ip < IP_MAX){
        double value = 0;

        int64_t nowcode = ((uint64_t*)pr.code)[pr.ip++];

        printf("NOW INST: %ld \n", nowcode & 0x00000000FFFFFFFF);
        PROCESSOR_DUMP(pr, PROC_ALL_OK);
        switch (nowcode & 0x00000000FFFFFFFF){
        case PUSH_C:
            printf("PUSHED:%lf\n", ((double*)pr.code)[pr.ip]);
            PUSH(pr.stk, ((double*)pr.code)[pr.ip++]);
            break;
        case RPUSH_C:
            printf("%lld\n", (nowcode & 0xFFFFFFFF00000000) >> 32);
            assert(((nowcode & 0xFFFFFFFF00000000) >> 32) < 4);
            printf("PUSHED:%lf\n", pr.reg[(nowcode & 0xFFFFFFFF00000000) >> 32]);
            PUSH(pr.stk, pr.reg[(nowcode & 0xFFFFFFFF00000000) >> 32]);
            break;
        case POP_C:
            POP(pr.stk);
            break;
        case RPOP_C:
            assert(((nowcode & 0xFFFFFFFF00000000) >> 32) < 4);
            pr.reg[(nowcode & 0xFFFFFFFF00000000) >> 32] = POP(pr.stk);
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