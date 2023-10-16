#include "Disassembler.h"

char* creacte_reg(long offset){
    static char reg[] = "r&x";
    reg[1] = (char)('a' + offset);
    return reg;
}

#define DEF_CMD(name, rname, num, args, ...)                                            \
    case nmae##_C:{                                                                     \
        if(args == 0){                                                                  \
            fprintf(outputfile,"%s\n", name##_S)                                        \
        }                                                                               \
        else if(args == 1){                                                             \
            if(((command) >> 32) == 0){                                                 \
                fprintf(outputfile,"%s %s\n",  name##_S, creacte_reg((command) >> 32)); \
            }                                                                           \
            else {                                                                      \
                double dubarg = 0;                                                      \
                fread(&dubarg, sizeof(ProcessorContainer), 1, inputfile);               \
                fprintf(outputfile,"%s %lg\n",  name##_S, dubarg);                      \
            }                                                                           \
        }                                                                               \
        break;                                                                          \
    }                                                                                   

void disassembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        FILE* inputfile  = fopen(FILE_NAME_INPUT,  "rb");
        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "w");

        assert(inputfile  != NULL);
        assert(outputfile != NULL);

        int64_t command = {};

        while(fread(&command, sizeof(ProcessorContainer), 1, inputfile) == FREAD_SUCCES){
            //static char argument[100] = {};
            switch (command & 0x00000000FFFFFFFF){
            case PUSH_C:{
                double dubarg = 0;
                fread(&dubarg, sizeof(ProcessorContainer), 1, inputfile);
                fprintf(outputfile,"%s %lg\n",  PUSH_S, dubarg);
                break;
            }
            case RPUSH_C:
                fprintf(outputfile,"%s %s\n",  PUSH_S, creacte_reg((command) >> 32));
                break;
            case RPOP_C:
                fprintf(outputfile,"%s %s\n",  PUSH_S, creacte_reg((command) >> 32));
                break;
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, IN  );
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, ADD );
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, SUB );
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, MUL );
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, DIV );
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, SQRT);
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, SIN );
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, COS );
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, OUT );
            CASE_ADD_INSTRUCTION_ZERO_ARGUMENTS(outputfile, HLT );
            default:
                break;
            }
    }

    fclose(inputfile );
    fclose(outputfile);
}