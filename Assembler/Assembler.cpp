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
        static const size_t MAX_ARGUMENT_SIZE = 300;

        static const size_t START_BUFFER_SIZE = 1000;
        size_t buffer_size = START_BUFFER_SIZE;

        char* buffer = (char*) calloc(buffer_size, sizeof(char));
        size_t buffer_offset = 0;

        while(fscanf(inputfile, "%s", asmfunc) != EOF){
            printf("off = %llu\n", buffer_offset);;
            printf("func = %s\n",asmfunc);
            static char argument[MAX_ARGUMENT_SIZE] = {};

            if(buffer_offset + sizeof(ProcessorContainer) * 4 >= buffer_size){
                void* new_data = recalloc(buffer, buffer_size * 2, sizeof(char), buffer_size, sizeof(char));
                assert(new_data != NULL);
                buffer_size *= 2;
                buffer = (char*)new_data;
            }

            if(strncmp(asmfunc, PUSH_S, PUSH_N) == 0){
                fscanf (inputfile, "%s", argument);
                if(*argument == 'r'){
                    int64_t vr = ((int64_t)(*(argument + 1) - 'a')) << 32 | RPUSH_C;
                                    int64_t i = vr;
                printf("NEEDED\n");
                for(int j = 0; j < 64; j++){
                    printf("%d", i & 1);
                    i >>= 1;
                }
                    memcpy((void*)(buffer + buffer_offset), &vr, sizeof(ProcessorContainer));
                    buffer_offset += sizeof(ProcessorContainer);
                } 
                else {
                    double value = atof(argument);
                    int64_t vr = PUSH_C;

                int64_t i = vr;
                for(int j = 0; j < 64; j++){
                    printf("%d", i & 1);
                    i >>= 1;
                }
                putchar('\n');
                    memcpy((void*)(buffer + buffer_offset), &vr, sizeof(ProcessorContainer));
                    buffer_offset += sizeof(ProcessorContainer);
                    memcpy((void*)(buffer + buffer_offset), &value, sizeof(ProcessorContainer));
                    buffer_offset += sizeof(ProcessorContainer);
                }
            }
            else if(strncmp(asmfunc, POP_S, POP_N) == 0){
                fscanf (inputfile, "%s", argument);
                int64_t vr = ((int64_t)(*(argument + 1) - 'a')) << 32 | RPOP_C;

                int64_t i = vr;
                for(int j = 0; j < 64; j++){
                    printf("%d", i & 1);
                    i >>= 1;
                }
                putchar('\n');

                memcpy((void*)(buffer + buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, IN_S, IN_N) == 0){
                int64_t vr = IN_C;
                memcpy((void*)(buffer + buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, ADD_S, ADD_N) == 0){
                int64_t vr = ADD_C;
                memcpy((void*)(buffer + buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, SUB_S, SUB_N) == 0){
                int64_t vr = SUB_C;
                memcpy((void*)(buffer + buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, MUL_S, MUL_N) == 0){
                int64_t vr = MUL_C;
                memcpy((void*)(buffer+ buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, DIV_S, DIV_N) == 0){
                int64_t vr = DIV_C;
                memcpy((void*)(buffer+ buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, SQRT_S, SQRT_N) == 0){
                int64_t vr = SQRT_C;
                memcpy((void*)(buffer+ buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, SIN_S, SIN_N) == 0){
                int64_t vr = SIN_C;
                memcpy((void*)(buffer+ buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, COS_S, COS_N) == 0){
                int64_t vr = COS_C;
                memcpy((void*)(buffer+ buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, OUT_S, OUT_N) == 0){
                int64_t vr = OUT_C;
                memcpy((void*)(buffer+ buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
            else if(strncmp(asmfunc, HLT_S, HLT_N) == 0){
                int64_t vr = HLT_C;
                memcpy((void*)(buffer+ buffer_offset), &vr, sizeof(ProcessorContainer));
                buffer_offset += sizeof(ProcessorContainer);            
                }
    }

    fwrite(buffer, buffer_offset, sizeof(char), outputfile);

    free(buffer);
    fclose(inputfile );
    fclose(outputfile);
}