#include "Assembler.h"

void compile(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        FILE* inputfile  = fopen(FILE_NAME_INPUT,  "r");
        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "w");

        static const size_t MAX_FUNC_NAME_SIZE = 10;
        static char asmfunc[MAX_FUNC_NAME_SIZE] = {};

        while(fscanf(inputfile, "%s", asmfunc) != EOF){
            if(strncmp(asmfunc, PUSH_S, PUSH_N) == 0){
                double value = 0;
                fscanf (inputfile, "%lf", &value);
                fprintf(outputfile,"%d %lf\n",  PUSH_C, value);
            }
            else if(strncmp(asmfunc, IN_S, IN_N) == 0){
                fprintf(outputfile,"%d\n",  IN_C);
            }
            else if(strncmp(asmfunc, ADD_S, ADD_N) == 0){
                fprintf(outputfile,"%d\n",  ADD_C);
            }
            else if(strncmp(asmfunc, SUB_S, SUB_N) == 0){
                fprintf(outputfile,"%d\n",  SUB_C);
            }
            else if(strncmp(asmfunc, MUL_S, MUL_N) == 0){
                fprintf(outputfile,"%d\n",  MUL_C);
            }
            else if(strncmp(asmfunc, DIV_S, DIV_N) == 0){
                fprintf(outputfile,"%d\n",  DIV_C);
            }
            else if(strncmp(asmfunc, SQRT_S, SQRT_N) == 0){
                fprintf(outputfile,"%d\n",  SQRT_C);
            }
            else if(strncmp(asmfunc, SIN_S, SIN_N) == 0){
                fprintf(outputfile,"%d\n",  SIN_C);
            }
            else if(strncmp(asmfunc, COS_S, COS_N) == 0){
                fprintf(outputfile,"%d\n",  COS_C);
            }
            else if(strncmp(asmfunc, OUT_S, OUT_N) == 0){
                fprintf(outputfile,"%d\n",  OUT_C);
            }
            else if(strncmp(asmfunc, HLT_S, HLT_N) == 0){
                fprintf(outputfile,"%d\n",  HLT_C);
            }
    }

    fclose(inputfile );
    fclose(outputfile);
}