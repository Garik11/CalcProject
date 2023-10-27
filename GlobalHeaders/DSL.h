#define ONE_ARGUMENT 1
#define NON_ARGUMENT 0

DEF_CMD(PUSH, 0x21, ONE_ARGUMENT, 
    {
        ProcessorArgumentType value = argumentRead(&pr, nowcode);
        DO_PUSH(pr.stk, value);
    }
)

DEF_CMD(POP, 0x35, ONE_ARGUMENT, 
    {
        if(nowcode & MEM_BIT){
            if(nowcode & REG_BIT){
                size_t R_STATUS = (size_t)((nowcode & MASK_REG) >> REG_BITS);
                assert(R_STATUS < NUMBER_OF_REGISTERS);
                size_t offset   = (size_t)pr.reg[R_STATUS] * sizeof(ProcessorArgumentType);
                ProcessorArgumentType poppednum = DO_POP(pr.stk);
                memcpy(pr.MEM + offset, &poppednum, sizeof(ProcessorArgumentType));

            }
            else if(nowcode & NUM_BIT){
                size_t offset = (size_t)(((ProcessorArgumentType*)pr.code)[pr.ip]);
                ((ProcessorArgumentType*)pr.MEM)[offset] = DO_POP(pr.stk);
                pr.ip++;
            }
        }
        else if(nowcode & REG_BIT){
                size_t R_STATUS = (size_t)((nowcode & MASK_REG) >> REG_BITS);
                assert(R_STATUS < NUMBER_OF_REGISTERS);
                pr.reg[R_STATUS] = DO_POP(pr.stk);
        }
        else if(nowcode & NUM_BIT){
            assert(false);
            DO_POP(pr.stk);
        }
    }
)

DEF_CMD(IN, 54, NON_ARGUMENT, 
    {
        DO_IN(pr.stk);
    }
)

DEF_CMD(ADD, 55, NON_ARGUMENT, 
    {
        DO_ADD(pr.stk);
    }
)

DEF_CMD(SUB, 56, NON_ARGUMENT, 
    {
        DO_SUB(pr.stk);
    }
)

DEF_CMD(MUL, 57, NON_ARGUMENT, 
    {
        DO_MUL(pr.stk);
    }
)

DEF_CMD(DIV, 58, NON_ARGUMENT, 
    {
        DO_DIV(pr.stk);
    }
)

DEF_CMD(SQRT, 59, NON_ARGUMENT, 
    {
        DO_SQRT(pr.stk);
    }
)

DEF_CMD(SIN, 60, NON_ARGUMENT, 
    {
        DO_SIN(pr.stk);
    }

)
DEF_CMD(COS, 61, NON_ARGUMENT, 
    {
        DO_COS(pr.stk);
    }
)

DEF_CMD(OUT, 62, NON_ARGUMENT, 
    {
        DO_OUT(pr.stk);
    }
)

DEF_CMD(HLT, 63, NON_ARGUMENT, 
    {
        goto HLT;
    }
)

DEF_CMD(JMP, 64, ONE_ARGUMENT, 
    {
        pr.ip = calculateIP(&pr);
    }
)

DEF_CMD(JE, 65, ONE_ARGUMENT, 
    {
        if(fabs(DO_POP(pr.stk) - DO_POP(pr.stk)) < epsilan)
            pr.ip = calculateIP(&pr);
        else
            pr.ip += NEXT_INSTRUCTION;               
    }
)

DEF_CMD(JNE, 66, ONE_ARGUMENT, 
    {
        if(fabs(DO_POP(pr.stk) - DO_POP(pr.stk)) > epsilan)
            pr.ip = calculateIP(&pr);
        else
            pr.ip += NEXT_INSTRUCTION;            
    }
)

DEF_CMD(JG, 67, ONE_ARGUMENT, 
    {
        if(DO_POP(pr.stk) > DO_POP(pr.stk))
            pr.ip = calculateIP(&pr);
        else
            pr.ip += NEXT_INSTRUCTION;         
    }
)

DEF_CMD(JGE, 68, ONE_ARGUMENT, 
    {
        double first    = DO_POP(pr.stk);
        double second   = DO_POP(pr.stk);
        if(first > second || fabs(first - second) < epsilan)
            pr.ip = calculateIP(&pr);
        else
            pr.ip += NEXT_INSTRUCTION;         
    }
)

DEF_CMD(JL, 69, ONE_ARGUMENT, 
    {
        if(DO_POP(pr.stk) < DO_POP(pr.stk))
            pr.ip = calculateIP(&pr);
        else
            pr.ip += NEXT_INSTRUCTION;         
    }
)

DEF_CMD(JLE, 70, ONE_ARGUMENT, 
    {
        double first    = DO_POP(pr.stk);
        double second   = DO_POP(pr.stk);
        if(DO_POP(pr.stk) < DO_POP(pr.stk) || fabs(first - second) < epsilan)
            pr.ip = calculateIP(&pr);
        else
            pr.ip += NEXT_INSTRUCTION;          
    }
)

DEF_CMD(CALL, 71, ONE_ARGUMENT, 
    {
        DO_PUSH(pr.call_stk, (double)(pr.ip + NEXT_INSTRUCTION));
        pr.ip = calculateIP(&pr);
    }
)

DEF_CMD(RET, 72, NON_ARGUMENT, 
    {
        pr.ip = (size_t)DO_POP(pr.call_stk);
    }
)

DEF_CMD(OUTC, 73, ONE_ARGUMENT, 
    {
        ProcessorArgumentType value = argumentRead(&pr, nowcode);
        printf("%c", (int)value);
    }
)
DEF_CMD(OUTALL, 74, NON_ARGUMENT, 
    {
        size_t memside = sqrt(PROC_MEM_SIZE / sizeof(ProcessorArgumentType));
        for(size_t i = 0; i < memside; i++){
            for(size_t j = 0; j < memside; j++){
                static ProcessorArgumentType outnum = {};
                memcpy(
                        &outnum, 
                        pr.MEM + (i * memside + j) * sizeof(ProcessorArgumentType), 
                        sizeof(ProcessorArgumentType)
                );
                printf("%c", (int)outnum);
            }
            putchar('\n');
        }
    }
)

//JG JL JLE JGE для них первый элемент - верхний
// Stack - ret register

#undef DEF_CMD
#undef ONE_ARGUMENT
#undef NON_ARGUMENT