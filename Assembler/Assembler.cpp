#include "Assembler.h"

void assembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        FILE* inputfile  = fopen(FILE_NAME_INPUT,  "r");
        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "wb");

        assert(inputfile  != NULL);
        assert(outputfile != NULL);

        static const size_t MAX_FUNC_NAME_SIZE = 10;
        static char asmfunc[MAX_FUNC_NAME_SIZE] = {};

        while(fscanf(inputfile, "%s", asmfunc) != EOF){
            if(strncmp(asmfunc, PUSH_S, PUSH_N) == 0){
                double value = 0;
                fscanf (inputfile, "%lg", &value);
                double vr = PUSH_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
                fwrite(&value, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, IN_S, IN_N) == 0){
                double vr = IN_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, ADD_S, ADD_N) == 0){
                double vr = ADD_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, SUB_S, SUB_N) == 0){
                double vr = SUB_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, MUL_S, MUL_N) == 0){
                double vr = MUL_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, DIV_S, DIV_N) == 0){
                double vr = DIV_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, SQRT_S, SQRT_N) == 0){
                double vr = SQRT_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, SIN_S, SIN_N) == 0){
                double vr = SIN_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, COS_S, COS_N) == 0){
                double vr = COS_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, OUT_S, OUT_N) == 0){
                double vr = OUT_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
            else if(strncmp(asmfunc, HLT_S, HLT_N) == 0){
                double vr = HLT_C;
                fwrite(&vr, 1, sizeof(double), outputfile);
            }
    }

    fclose(inputfile );
    fclose(outputfile);
}