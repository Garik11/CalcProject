#include "Assembler.h"

void toupper_all(char *str){
    for(char* p = str; *p != 0; p++) 
        *p = (char)toupper(*p);
}

#define DEF_CMD(name, rname, num, args, ...)                                                                        \
                if(strcmp(asmfunc, #name) == 0){                                                                    \
                    if(args == 0){                                                                                  \
                            int64_t spucommand = num;                                                               \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));       \
                            buffer_offset += sizeof(ProcessorContainer);                                            \
                    }                                                                                               \
                    else if(args == 1){                                                                             \
                        sscanf(inbuffer + inbuffer_offset, "%s%n", argument, &asm_offset);                          \
                        inbuffer_offset += (size_t)asm_offset;                                                      \
                        printf("YA TYT %d\n", args);                                                                \
                        if(*argument == 'R'){                                                                       \
                            int64_t spucommand = ((int64_t)(*(argument + 1) - 'A')) << 32 | rname##_C;              \
                            printf("MY DOUBLE:%s\n", argument);                                                     \
                            printf("MY DOUBLE NUM:%ld\n", (int64_t)(*(argument + 1) - 'A'));                        \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));       \
                            buffer_offset += sizeof(ProcessorContainer);                                            \
                        }                                                                                           \
                        else {                                                                                      \
                            double value = atof(argument);                                                          \
                            printf("MY DOUBLE:%lf\n", value);                                                       \
                            printf("MY DOUBLE:%s\n", argument);                                                     \
                            int64_t spucommand = num;                                                               \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));       \
                            buffer_offset += sizeof(ProcessorContainer);                                            \
                            memcpy((void*)(buffer + buffer_offset), &value, sizeof(ProcessorContainer));            \
                            buffer_offset += sizeof(ProcessorContainer);                                            \
                        }                                                                                           \
                    }                                                                                               \
                }                                                                                                   \
                else

void assembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        printf("%s %s\n", FILE_NAME_INPUT, FILE_NAME_OUTPUT);

        size_t inbuffersize = 0;
        assert(fsize(FILE_NAME_INPUT, &inbuffersize) == FSIZE_SUCCES);
        char* inbuffer = (char*)calloc(sizeof(char), inbuffersize + 2);
        assert(inbuffer != NULL);
        FILE* infile = fopen(FILE_NAME_INPUT, "r");
        assert(infile != NULL);
        assert(fread(inbuffer, inbuffersize, sizeof(char), infile) == FREAD_SUCCES);
        size_t inbuffer_offset = 0;
        toupper_all(inbuffer);
        fclose(infile);

        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "wb");

        assert(outputfile != NULL);

        static const size_t MAX_FUNC_NAME_SIZE = 10;
        static const size_t MAX_ARGUMENT_SIZE = 300;
        static char asmfunc[MAX_FUNC_NAME_SIZE] = {};

        static const size_t START_BUFFER_SIZE = 1000;
        size_t buffer_size = START_BUFFER_SIZE;

        char* buffer = (char*) calloc(buffer_size, sizeof(char));
        size_t buffer_offset = 0;

        int asm_offset = 0;
        while(sscanf(inbuffer + inbuffer_offset, "%s%n", asmfunc, &asm_offset) != EOF){
            
            inbuffer_offset += (size_t)asm_offset;

            static char argument[MAX_ARGUMENT_SIZE] = {};

            if(buffer_offset + sizeof(ProcessorContainer) * 4 >= buffer_size){
                void* new_data = recalloc(buffer, buffer_size * 2, sizeof(char), buffer_size, sizeof(char));
                assert(new_data != NULL);
                buffer_size *= 2;
                buffer = (char*)new_data;
            }
            printf("asmfunc = %s\n", asmfunc);
            #include "../GlobalHeaders/DSL.h"
            /*else*/ void(0);
    }

    fwrite(buffer, buffer_offset, sizeof(char), outputfile);

    free(buffer);
    free(inbuffer);
    fclose(outputfile);
}