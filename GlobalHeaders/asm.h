#ifndef CALC_CALC_H
#define CALC_CALC_H

#include <stddef.h>

static const char PUSH_S[] = "push" ;
static const char POP_S [] = "pop"  ; 
static const char IN_S  [] = "in"   ; 
static const char ADD_S [] = "add"  ;
static const char SUB_S [] = "sub"  ;
static const char MUL_S [] = "mul"  ;
static const char DIV_S [] = "div"  ;
static const char SQRT_S[] = "sqrt" ;
static const char SIN_S [] = "sin"  ;
static const char COS_S [] = "cos"  ;
static const char OUT_S [] = "out"  ;
static const char HLT_S [] = "hlt"  ;

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

enum COMMANDCODS{
    SIGILL_С    = 0x0,
    PUSH_C      = 0x21,
    RPUSH_C     = 0x11,
    RPOP_C      = 0x2B,
    POP_C       = 0x35,
    IN_C        ,
    ADD_C       ,
    SUB_C       ,
    MUL_C       ,
    DIV_C       ,
    SQRT_C      ,
    SIN_C       ,
    COS_C       ,
    OUT_C       ,
    HLT_C
};

#endif // !CALC_CALC_H