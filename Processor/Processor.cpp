#include "Processor.h"
static void    push    (Stack* stk, double value, CalcErrorsBitmask *outerror = NULL);
static double  pop     (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    add     (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    div     (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    sub     (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    mul     (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    sin     (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    cos     (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    sqrt    (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    out     (Stack* stk, CalcErrorsBitmask *outerror = NULL);
static void    in      (Stack* stk, CalcErrorsBitmask *outerror = NULL);

void calc(const char* FILE_NAME, CalcErrorsBitmask *outerror){
    FILE *fp = fopen(FILE_NAME, "r");
    assert(fp != NULL);

    CalcErrorsBitmask calcerror     = CALC_ALL_OK ;    
    StackErrorsBitmask stackerror   = STACK_ALL_OK;

    Stack* stk = STACK_CTOR(&stackerror);

    int command_code = 0;

    while(fscanf(fp, "%d", &command_code) != EOF){
        double value = 0;
        switch (command_code){
        case PUSH_C:
            fscanf(fp, "%lf", &value);
            push(stk, value);
            break;
        case POP_C:
            pop(stk);
            break;
        case IN_C:
            in(stk);
            break;
        case ADD_C:
            add(stk);
            break;
        case SUB_C:
            sub(stk);
            break;
        case MUL_C:
            mul(stk);
            break;
        case DIV_C:
            div(stk);
            break;
        case SQRT_C:
            sqrt(stk);
            break;
        case SIN_C:
            sin(stk);
            break;
        case COS_C:
            cos(stk);
            break;
        case OUT_C:
            out(stk);
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

static void push(Stack* stk, double value, CalcErrorsBitmask *outerror){

    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    StackPush(stk, value, &stackerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_PUSH_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_PUSH_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;
}

static double pop(Stack* stk, CalcErrorsBitmask *outerror){

    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double outnum = StackPop(stk, &stackerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_POP_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_POP_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

    return outnum;
}

static void add(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double first_value  = pop(stk, &calcerror);
    double second_value = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_ADD_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_ADD_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

    push(stk, first_value + second_value, &calcerror);

    if(outerror != NULL)
        *outerror |= calcerror;
}


static void div(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double first_value  = pop(stk, &calcerror);
    double second_value = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_DIV_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_DIV_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

    push(stk, second_value / first_value, &calcerror);

    if(outerror != NULL)
        *outerror |= calcerror;
}

static void sub(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double first_value  = pop(stk, &calcerror);
    double second_value = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_SUB_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SUB_ERROR));
    )

    push(stk, second_value - first_value, &calcerror);

    if(outerror != NULL)
        *outerror |= calcerror;
}

static void mul(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double first_value  = pop(stk, &calcerror);
    double second_value = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_MUL_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_MUL_ERROR));
    )

    push(stk, second_value * first_value, &calcerror);

    if(outerror != NULL)
        *outerror |= calcerror;
}

static void sin(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value  = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_SIN_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SIN_ERROR));
    )

    printf("sin(%lf) = %lf\n", value, sin(value));

    push(stk, sin(value), &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_SIN_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SIN_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;
}

static void cos(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value  = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_COS_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_COS_ERROR));
    )

    push(stk, cos(value), &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_COS_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_COS_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;
}

static void sqrt(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value  = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_SQRT_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SQRT_ERROR));
    )

    push(stk, sqrt(value), &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_SQRT_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SQRT_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

}

static void out(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value  = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_OUT_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_OUT_ERROR));
    )

    push(stk, value, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_OUT_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_OUT_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

    printf("Your value : %lf\n", value);

}

static void in(Stack* stk, CalcErrorsBitmask *outerror){
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value = 0;

    printf("Enter value:\n");
    #warning badscanf
    scanf("%lf", &value);

    push(stk, value, &calcerror);

    calcerror |= CALC_SET_ERROR(calcerror == CALC_ALL_OK, CALC_IN_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_IN_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

}