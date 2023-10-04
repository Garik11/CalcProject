#include "Processor.h"
static const char PUSH_C[] = "push" ; //+ 
static const char POP_C [] = "pop"  ; 
static const char IN_C  [] = "in"   ; 
static const char ADD_C [] = "add"  ; //+
static const char SUB_C [] = "sub"  ; //+
static const char MUL_C [] = "mul"  ; //+
static const char DIV_C [] = "div"  ; //+
static const char SQRT_C[] = "sqrt" ; //+
static const char SIN_C [] = "sin"  ; //+
static const char COS_C [] = "cos"  ; //+
static const char OUT_C [] = "out"  ; //+
static const char HLT_C [] = "hlt"  ; //+

static const size_t PUSH_N = 4;
static const size_t POP_N  = 3; 
static const size_t IN_N   = 2;
static const size_t ADD_N  = 3;
static const size_t SUB_N  = 3;
static const size_t MUL_N  = 3;
static const size_t DIV_N  = 3;
static const size_t SQRT_N = 4;
static const size_t SIN_N  = 3;
static const size_t COS_N  = 3;
static const size_t OUT_N  = 3;
static const size_t HLT_N  = 3;

//push + 
//sqrt + 
//pop + 
//add + 
//sub + 
//mul +
//div + 
//sin + 
//cos + 
//out +
//hlt + 


void push(Stack* stk, double num, CalcErrorsBitmask *outerror){

    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    StackPush(stk, num, &stackerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_PUSH_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_PUSH_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;
}

double pop(Stack* stk, CalcErrorsBitmask *outerror){

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

void add(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double first_value  = pop(stk, &calcerror);
    double second_value = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_ADD_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_ADD_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

    push(stk, first_value + second_value, &calcerror);

    if(outerror != NULL)
        *outerror |= calcerror;
}

void div(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double first_value  = pop(stk, &calcerror);
    double second_value = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_DIV_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_DIV_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

    push(stk, second_value / first_value, &calcerror);

    if(outerror != NULL)
        *outerror |= calcerror;
}

void sub(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double first_value  = pop(stk, &calcerror);
    double second_value = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_SUB_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SUB_ERROR));
    )

    push(stk, second_value - first_value, &calcerror);

    if(outerror != NULL)
        *outerror |= calcerror;
}

void mul(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double first_value  = pop(stk, &calcerror);
    double second_value = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_MUL_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_MUL_ERROR));
    )

    push(stk, second_value * first_value, &calcerror);

    if(outerror != NULL)
        *outerror |= calcerror;
}

void sin(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value  = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_SIN_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SIN_ERROR));
    )

    printf("sin(%lf) = %lf\n", value, sin(value));

    push(stk, sin(value), &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_SIN_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SIN_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;
}

void cos(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value  = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_COS_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_COS_ERROR));
    )

    push(stk, cos(value), &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_COS_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_COS_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;
}

void sqrt(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value  = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_SQRT_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SQRT_ERROR));
    )

    push(stk, sqrt(value), &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_SQRT_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_SQRT_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

}

void out(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value  = pop(stk, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_OUT_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_OUT_ERROR));
    )

    push(stk, value, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_OUT_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_OUT_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

    printf("Your value : %lf\n", value);

}

void in(Stack* stk, CalcErrorsBitmask *outerror){
    StackErrorsBitmask  stackerror  =   STACK_ALL_OK;
    CalcErrorsBitmask   calcerror   =   CALC_ALL_OK ;

    double value = 0;

    printf("Enter number:\n");
    #warning badscanf
    scanf("%lf", &value);

    push(stk, value, &calcerror);

    calcerror |= CALC_SET_ERROR(stackerror == STACK_ALL_OK, CALC_IN_ERROR);

    ON_DEBUG(
        assert(!(calcerror & CALC_IN_ERROR));
    )

    if(outerror != NULL)
        *outerror |= calcerror;

}

void calc(char** text, size_t lines, CalcErrorsBitmask *outerror){
    assert(text != NULL);
    //assert(outerror != NULL);

    CalcErrorsBitmask calcerror = CALC_ALL_OK;    

    StackErrorsBitmask stackerror = STACK_ALL_OK;
    Stack* stk = STACK_CTOR(&stackerror);

    for(size_t pos = 0; pos < lines; pos++){
        STACK_DUMP(stk, stackerror);
        if(strncmp(text[pos], PUSH_C, PUSH_N) == 0){
            push(stk, atof(text[pos] + PUSH_N), &calcerror);
        } 
        else if(strncmp(text[pos], ADD_C, ADD_N) == 0){
            add(stk, &stackerror);
        }
        else if(strncmp(text[pos], SUB_C, SUB_N) == 0){
            sub(stk, &stackerror);
        }
        else if(strncmp(text[pos], OUT_C, OUT_N) == 0){
            out(stk, &stackerror);
        }
        else if(strncmp(text[pos], DIV_C, DIV_N) == 0){
            div(stk, &stackerror);
        }
        else if(strncmp(text[pos], HLT_C, HLT_N) == 0){
            break;
        }
        else if(strncmp(text[pos], MUL_C, MUL_N) == 0){
            mul(stk, &stackerror);
        }
        else if(strncmp(text[pos], SQRT_C, SQRT_N) == 0){
            sqrt(stk, &stackerror);
        }
        else if(strncmp(text[pos], SIN_C, SIN_N) == 0){
            sin(stk, &stackerror);
        }
        else if(strncmp(text[pos], COS_C, COS_N) == 0){
            cos(stk, &stackerror);
        }
        else if(strncmp(text[pos], DIV_C, DIV_N) == 0){
            div(stk, &stackerror);
        }
        else if(strncmp(text[pos], IN_C, IN_N) == 0){
            in(stk, &stackerror);
        }
    }
    
    StackDtor(stk);
}