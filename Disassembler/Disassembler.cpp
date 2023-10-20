#include "Disassembler.h"

char* creacte_reg(long offset){
    static char reg[] = "R&X";
    reg[1] = (char)('A' + offset);
    return reg;
}

#define DEF_CMD(name, rname, num, args, ...)                                                            \
    case num:{                                                                                          \
        if(args == 0){                                                                                  \
            fprintf(outputfile,"%s\n", #name);                                                          \
        }                                                                                               \
        else if(args == 1){                                                                             \
            if((command >> REG_BITS) != 0){                                                             \
                fprintf(outputfile,"%s %s\n",  #name, creacte_reg((command >> REG_BITS) - REG_OFFSET)); \
            }                                                                                           \
            else {                                                                                      \
                double dubarg = 0;                                                                      \
                fread(&dubarg, sizeof(ProcessorArgumentType), 1, inputfile);                            \
                fprintf(outputfile,"%s %lg\n",  #name, dubarg);                                         \
            }                                                                                           \
        }                                                                                               \
    }                                                                                                   \
    break;                                                                                              \

void disassembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        FILE* inputfile  = fopen(FILE_NAME_INPUT,  "rb");
        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "w");

        assert(inputfile  != NULL);
        assert(outputfile != NULL);

        ProcessorContainer command = {};

        while(fread(&command, sizeof(ProcessorContainer), 1, inputfile) == FREAD_SUCCES){
            switch (command & MASK_CODE){
            #include "../GlobalHeaders/DSL.h"
            default:
                printf("Incorrect Instuction!\n");
                break;
            }
    }

    fclose(inputfile );
    fclose(outputfile);
}