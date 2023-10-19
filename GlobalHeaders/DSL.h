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

#define NULL_REG 0

DEF_CMD(RPUSH   ,   NULL_REG, 0x11  ,   1, {
            int64_t R_STATUS = ((nowcode & MASK_REG) >> 32) - 1;
            printf("%ld\n", R_STATUS);
            assert(R_STATUS < 4);
            printf("PUSHED:%lf\n", pr.reg[R_STATUS]);
            DO_PUSH(pr.stk, pr.reg[R_STATUS]);
})
DEF_CMD(PUSH    ,   0x11   , 0x21  ,   1, {
            printf("PUSHED:%lf\n", ((double*)pr.code)[pr.ip]);
            DO_PUSH(pr.stk, ((double*)pr.code)[pr.ip++]);
})
DEF_CMD(RPOP    ,   NULL_REG, 0x2B  ,   1, {
            int64_t R_STATUS = ((nowcode & MASK_REG) >> 32) - 1;
            assert(R_STATUS < 4);
            pr.reg[R_STATUS] = DO_POP(pr.stk);
})
DEF_CMD(POP     ,   0x2B    , 0x35  ,   1, {
    DO_POP(pr.stk);
})
DEF_CMD(IN      ,   NULL_REG, 54    ,   0, {
    DO_IN(pr.stk);
})
DEF_CMD(ADD     ,   NULL_REG, 55    ,   0, {
    DO_ADD(pr.stk);
})
DEF_CMD(SUB     ,   NULL_REG, 56    ,   0, {
    DO_SUB(pr.stk);
})
DEF_CMD(MUL     ,   NULL_REG, 57    ,   0, {
    DO_MUL(pr.stk);
})
DEF_CMD(DIV     ,   NULL_REG, 58    ,   0, {
    DO_DIV(pr.stk);
})
DEF_CMD(SQRT    ,   NULL_REG, 59    ,   0, {
    DO_SQRT(pr.stk);
})
DEF_CMD(SIN     ,   NULL_REG, 60    ,   0, {
    DO_SIN(pr.stk);
})
DEF_CMD(COS     ,   NULL_REG, 61    ,   0, {
    DO_COS(pr.stk);
})
DEF_CMD(OUT     ,   NULL_REG, 62    ,   0, {
    DO_OUT(pr.stk);
})
DEF_CMD(HLT     ,   NULL_REG, 63    ,   0, {
    goto HLT;
})

#undef DEF_CMD
#undef NULL_REG
#undef NULL_REG_C