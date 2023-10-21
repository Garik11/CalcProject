
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

//0000000000000000BBBB-MIRCCCCCCCC
//                      -100000000

/*
MASK_REG_FLAG = 1 << 8;
CMD_PUSH = 0xDED;
CMD_PUSH | MASK_REG_FLAG;
*/

#define NULL_REG 0
#define ONE_REG  1   

DEF_CMD(PUSH, 0x21  ,   1, {
            if(nowcode & REG_BIT){
                int64_t R_STATUS = ((nowcode & MASK_REG) >> REG_BITS);
                printf("%ld\n", R_STATUS);
                assert(R_STATUS < 4);
                printf("PUSHED:%lf\n", pr.reg[R_STATUS]);
                DO_PUSH(pr.stk, pr.reg[R_STATUS]);
            }
            else if(nowcode & NUM_BIT){
                printf("PUSHED:%lf\n", ((double*)pr.code)[pr.ip]);
                DO_PUSH(pr.stk, ((double*)pr.code)[pr.ip++]);
            }
})

DEF_CMD(POP, 0x35  ,   1, {
    if(nowcode & REG_BIT){
            int64_t R_STATUS = ((nowcode & MASK_REG) >> REG_BITS);
            assert(R_STATUS < 4);
            pr.reg[R_STATUS] = DO_POP(pr.stk);
    }
    else if(nowcode & NUM_BIT){
        DO_POP(pr.stk);
    }
})
DEF_CMD(IN, 54    ,   0, {
    DO_IN(pr.stk);
})
DEF_CMD(ADD, 55    ,   0, {
    DO_ADD(pr.stk);
})
DEF_CMD(SUB, 56    ,   0, {
    DO_SUB(pr.stk);
})
DEF_CMD(MUL, 57    ,   0, {
    DO_MUL(pr.stk);
})
DEF_CMD(DIV, 58    ,   0, {
    DO_DIV(pr.stk);
})
DEF_CMD(SQRT, 59    ,   0, {
    DO_SQRT(pr.stk);
})
DEF_CMD(SIN, 60    ,   0, {
    DO_SIN(pr.stk);
})
DEF_CMD(COS, 61    ,   0, {
    DO_COS(pr.stk);
})
DEF_CMD(OUT, 62    ,   0, {
    DO_OUT(pr.stk);
})
DEF_CMD(HLT, 63    ,   0, {
    printf("HLT DO\n");
    goto HLT;
})

DEF_CMD(JMP, 64     ,   1, {
    pr.ip = (size_t)((ProcessorContainer*)pr.code)[pr.ip];
})
DEF_CMD(JE, 65     ,   1, {
    if(fabs(DO_POP(pr.stk) - DO_POP(pr.stk)) < epsilan)
        pr.ip = (size_t)((ProcessorContainer*)pr.code)[pr.ip];
})
DEF_CMD(JNE, 66     ,   1, {
    if(fabs(DO_POP(pr.stk) - DO_POP(pr.stk)) > epsilan)
        pr.ip = (size_t)((ProcessorContainer*)pr.code)[pr.ip];
})
DEF_CMD(JG, 67     ,   1, {
    if(DO_POP(pr.stk) > DO_POP(pr.stk))
        pr.ip = (size_t)((ProcessorContainer*)pr.code)[pr.ip];
})
DEF_CMD(JGE, 68     ,   1, {
    if(DO_POP(pr.stk) >= DO_POP(pr.stk))
        pr.ip = (size_t)((ProcessorContainer*)pr.code)[pr.ip];
})
DEF_CMD(JL, 69     ,   1, {
    printf("TYTE VIZOF JL\n");
    if(DO_POP(pr.stk) < DO_POP(pr.stk))
        pr.ip = (size_t)((ProcessorContainer*)pr.code)[pr.ip];
})
DEF_CMD(JLE, 70     ,   1, {
    if(DO_POP(pr.stk) <= DO_POP(pr.stk))
        pr.ip = (size_t)((ProcessorContainer*)pr.code)[pr.ip];
})

#undef DEF_CMD
#undef NULL_REG
#undef NULL_REG_C