#include "Assembler.h"

void toupper_all(char *str){
    for(char* p = str; *p != 0; p++) 
        *p = (char)toupper(*p);
}

#define DEF_CMD(name, rname, num, args, ...)                                                                                                    \
                if(strcmp(asmfunc, #name) == 0){                                                                                                \
                    if(args == 0){                                                                                                              \
                            ProcessorContainer spucommand = num;                                                                                \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));                                   \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                    }                                                                                                                           \
                    else if(args == 1){                                                                                                         \
                        sscanf(inbuffer + inbuffer_offset, "%s%n", argument, &asm_offset);                                                      \
                        inbuffer_offset += (size_t)asm_offset;                                                                                  \
                        if(*argument == 'R'){                                                                                                   \
                            ProcessorContainer spucommand = (((*(argument + 1) - 'A' + REG_OFFSET)) << REG_BITS) | rname;                       \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));                                   \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                        }                                                                                                                       \
                        else {                                                                                                                  \
                            double value = atof(argument);                                                                                      \
                            ProcessorContainer spucommand = num;                                                                                \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));                                   \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                            memcpy((void*)(buffer + buffer_offset), &value, sizeof(ProcessorContainer));                                        \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                        }                                                                                                                       \
                    }                                                                                                                           \
                }                                                                                                                               \
                else

void assembler(const char* FILE_NAME_INPUT, const char* FILE_NAME_OUTPUT){
        assert(FILE_NAME_INPUT  != NULL);
        assert(FILE_NAME_OUTPUT != NULL);

        /*Max asm fubc name size*/
        static const size_t MAX_FUNC_NAME_SIZE  = 10    ;
        /*Max argument size (double can take up a lot of space)*/
        static const size_t MAX_ARGUMENT_SIZE   = 300   ;
        /*Start buffer size, if the size is not enough,
        it is multiplied by a constant : Buffer multiplier*/
        static const size_t START_BUFFER_SIZE   = 1000  ;
        /*Buffer multiplier*/
        static const size_t BUFFER_MULTIPLIER   = 2     ;
        /*Buffer for reading the function name*/
        static char asmfunc[MAX_FUNC_NAME_SIZE] = {}    ;
        /*INACCURACY of buffer size for mult*/
        static const size_t INACCURACY = 4;
        /*the extra byte for the last character*/
        static const size_t EXTRA_BYTE = 1;
        /*Size of input buffer*/
        size_t inbuffersize = 0;

        size_t fsize_status = fsize(FILE_NAME_INPUT, &inbuffersize);
        assert(fsize_status == FSIZE_SUCCES);

        char* inbuffer = (char*)calloc(sizeof(char), inbuffersize + EXTRA_BYTE);
        assert(inbuffer != NULL);

        FILE* infile = fopen(FILE_NAME_INPUT, "r");
        assert(infile != NULL);

        size_t fread_size = fread(inbuffer, inbuffersize, sizeof(char), infile);
        assert(fread_size == FREAD_SUCCES);

        /**/
        size_t inbuffer_offset = 0;
        toupper_all(inbuffer);
        fclose(infile);

        FILE* outputfile = fopen(FILE_NAME_OUTPUT, "wb");

        assert(outputfile != NULL);
        
        size_t  buffer_size = START_BUFFER_SIZE;
        char*   buffer = (char*) calloc(buffer_size, sizeof(char));
        size_t  buffer_offset = 0;

        int asm_offset = 0;
        while(sscanf(inbuffer + inbuffer_offset, "%s%n", asmfunc, &asm_offset) != EOF){

            inbuffer_offset += (size_t)asm_offset;
            
            /*processing comments*/
            if(*asmfunc == ';'){
                while(  *(inbuffer + inbuffer_offset) != '\n' && 
                        *(inbuffer + inbuffer_offset) != 0)
                    inbuffer_offset++;
                continue;
            }

            static char argument[MAX_ARGUMENT_SIZE] = {};

            if(buffer_offset + sizeof(ProcessorContainer) * INACCURACY >= buffer_size){
                void* new_data = recalloc(buffer, buffer_size * BUFFER_MULTIPLIER, sizeof(char), buffer_size, sizeof(char));
                assert(new_data != NULL);
                buffer_size *= BUFFER_MULTIPLIER;
                buffer = (char*)new_data;
            }
            #include "../GlobalHeaders/DSL.h"
            /*else*/ void(0);
    }

    fwrite(buffer, buffer_offset, sizeof(char), outputfile);

    free(buffer);
    free(inbuffer);
    fclose(outputfile);
}