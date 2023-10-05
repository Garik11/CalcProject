#include "Disassembler.h"

void decompile(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        FILE* inputfile  = fopen(FILE_NAME_INPUT,  "r");
        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "w");

        assert(inputfile  != NULL);
        assert(outputfile != NULL);

        static const size_t MAX_FUNC_NAME_SIZE = 10;
        static char asmfunc[MAX_FUNC_NAME_SIZE] = {};

        static int asmint = {};

        while(fscanf(inputfile, "%d", &asmint) != EOF){
            double value = 0;
            switch (asmint){
            case PUSH_C:
                fscanf (inputfile, "%lg", &value);
                fprintf(outputfile,"%s %lg\n",  PUSH_S, value);
                break;
            case IN_C:
                fprintf(outputfile,"%s\n",  IN_S);
                break;
            case ADD_C:
                fprintf(outputfile,"%s\n",  ADD_S);
                break;
            case SUB_C:
                fprintf(outputfile,"%s\n",  SUB_S);
                break;
            case MUL_C:
                fprintf(outputfile,"%s\n",  MUL_S);
                break;
            case DIV_C:
                fprintf(outputfile,"%s\n",  DIV_S);
                break;
            case SQRT_C:
                fprintf(outputfile,"%s\n",  SQRT_S);
                break;
            case SIN_C:
                fprintf(outputfile,"%s\n",  SIN_S);
                break;
            case COS_C:
                fprintf(outputfile,"%s\n",  COS_S);
                break;
            case OUT_C:
                fprintf(outputfile,"%s\n",  OUT_S);
                break;
            case HLT_C:
                fprintf(outputfile,"%s\n",  HLT_S);
                break;
            default:
                break;
            }
    }

    fclose(inputfile);
    fclose(outputfile);
}