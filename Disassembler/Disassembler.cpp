#include "Disassembler.h"

void disassembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        FILE* inputfile  = fopen(FILE_NAME_INPUT,  "r");
        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "w");

        assert(inputfile  != NULL);
        assert(outputfile != NULL);

        static int asmint = {};

        while(fscanf(inputfile, "%d", &asmint) != EOF){
            double value = 0;
            switch (asmint){
            case PUSH_C:
                fscanf (inputfile, "%lg",       &value          );
                fprintf(outputfile,"%s %lg\n",  PUSH_S, value   );
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