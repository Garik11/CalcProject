/*
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
};*/

#define NULL_REG
#define NULL_REG_C 0

DEF_CMD(RPUSH   ,   NULL_REG, 0x11  ,   1)
DEF_CMD(PUSH    ,   RPUSH   , 0x21  ,   1)
DEF_CMD(RPOP    ,   NULL_REG, 0x2B  ,   1)
DEF_CMD(POP     ,   RPOP    , 0x35  ,   1)
DEF_CMD(IN      ,   NULL_REG, 54    ,   0)
DEF_CMD(ADD     ,   NULL_REG, 55    ,   0)
DEF_CMD(SUB     ,   NULL_REG, 56    ,   0)
DEF_CMD(MUL     ,   NULL_REG, 57    ,   0)
DEF_CMD(DIV     ,   NULL_REG, 58    ,   0)
DEF_CMD(SQRT    ,   NULL_REG, 59    ,   0)
DEF_CMD(SIN     ,   NULL_REG, 60    ,   0)
DEF_CMD(COS     ,   NULL_REG, 61    ,   0)
DEF_CMD(OUT     ,   NULL_REG, 62    ,   0)
DEF_CMD(HLT     ,   NULL_REG, 63    ,   0)

#undef DEF_CMD
#undef NULL_REG
#undef NULL_REG_C