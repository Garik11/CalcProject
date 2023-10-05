#include "Processor.h"

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


void calc(const char* FILE_NAME){

    FILE *fp = fopen(FILE_NAME, "r");
    assert(fp != NULL);

    StackErrorsBitmask stackerror = STACK_ALL_OK;
    Stack* stk = STACK_CTOR(&stackerror);
    assert(stackerror == STACK_ALL_OK);

    int command_code = 0;

    while(fscanf(fp, "%d", &command_code) != EOF){
        double value = 0;
        switch (command_code){
        case PUSH_C:
            fscanf(fp, "%lg", &value);
            PUSH(stk, value);
            break;
        case POP_C:
            POP(stk);
            break;
        case IN_C:
            IN(stk);
            break;
        case ADD_C:
            ADD(stk);
            break;
        case SUB_C:
            SUB(stk);
            break;
        case MUL_C:
            MUL(stk);
            break;
        case DIV_C:
            DIV(stk);
            break;
        case SQRT_C:
            SQRT(stk);
            break;
        case SIN_C:
            SIN(stk);
            break;
        case COS_C:
            COS(stk);
            break;
        case OUT_C:
            OUT(stk);
            break;
        case HLT_C:
            break;
        default /*SIGILL_C*/:
            printf("Incorrect instruction \"%d\", terminate process!\n", command_code);
            break;
        }
    }

    StackDtor(stk);
    fclose(fp);

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