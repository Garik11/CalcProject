#include "Disassembler.h"

char* creacte_reg(long offset){
    static char reg[] = "R&X";
    reg[1] = (char)('A' + offset);
    return reg;
}

#define DEF_CMD(name, num, args, ...)                                                                   \
    case num:{                                                                                          \
        if(args == 0){                                                                                  \
            fprintf(outputfile,"%s\n", #name);                                                          \
        }                                                                                               \
        else if(args == 1){                                                                             \
            fprintf(outputfile, "%s ", #name);                                                          \
            if(command & MEM_BIT){                                                                      \
                fprintf(outputfile, "%c", '[');                                                         \
            }                                                                                           \
            if(command & REG_BIT){                                                                      \
                fprintf(outputfile, "%s", creacte_reg((command >> REG_BITS)));                          \
            }                                                                                           \
            else if(command & NUM_BIT){                                                                 \
                ProcessorArgumentType dubarg = 0;                                                       \
                fread(&dubarg, sizeof(ProcessorArgumentType), 1, inputfile);                            \
                fprintf(outputfile,"%lg", dubarg);                                                      \
            }                                                                                           \
            else /*jmp mark*/{                                                                          \
                ProcessorContainer dubarg = 0;                                                          \
                fread(&dubarg, sizeof(ProcessorContainer), 1, inputfile);                               \
                fprintf(outputfile,"%ld", dubarg);                                                      \
            }                                                                                           \
            if(command & MEM_BIT){                                                                      \
                fprintf(outputfile, "%c", ']');                                                         \
            }                                                                                           \
            fprintf(outputfile, "\n");                                                                  \
        }                                                                                               \
    }                                                                                                   \
    break;                                                                                              \

void disassembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        FILE* inputfile  = fopen(FILE_NAME_INPUT,  "rb");
        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "w" );

        assert(inputfile  != NULL);
        assert(outputfile != NULL);

        ProcessorContainer command  = 0;
        
        char authors_name   [sizeof(AUTHORS_NAME)   + ONE_ADDITIONAL_SYMBOL] = {};
        assert(fread(authors_name, sizeof(AUTHORS_NAME),    1, inputfile) == FREAD_SUCCES);
        char version        [sizeof(VERSION)        + ONE_ADDITIONAL_SYMBOL] = {};
        assert(fread(version, sizeof(VERSION),              1, inputfile) == FREAD_SUCCES);

        printf("Author's name: %s, version: %s\n", authors_name, version);
        printf("Disassembling...\n");

        while(fread(&command, sizeof(ProcessorContainer), 1, inputfile) == FREAD_SUCCES){
            switch (command & MASK_CODE){
            #include "../GlobalHeaders/DSL.h"
            default:
                printf("Incorrect Instuction: %ld!\n", command & MASK_CODE);
                break;
            }
    }

    fclose(inputfile );
    fclose(outputfile);
    printf("Disassembling successfully!\n");
}