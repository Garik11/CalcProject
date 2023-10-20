#include "Assembler.h"

void toupper_all(char *str){
    for(char* p = str; *p != 0; p++) 
        *p = (char)toupper(*p);
}
//функция аргумент
#define DEF_CMD(name, rnum, num, args, ...)                                                                                                    \
                if(strcmp(asmfunc, #name) == 0){                                                                                                \
                    if(args == 0){                                                                                                              \
                            ProcessorContainer spucommand = num;                                                                                \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));                                   \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                    }                                                                                                                           \
                    else if(args == 1){                                                                                                         \
                         /* TODO move to function */                                                                                            \
                        sscanf(inbuffer + inbuffer_offset, "%s%n", argument, &asm_offset);                                                      \
                        inbuffer_offset += (size_t)asm_offset;                                                                                  \
                        ip++;                                                                                                                   \
                        if(*argument == 'R'){                                                                                                   \
                            ProcessorContainer spucommand = (((*(argument + 1) - 'A' + REG_OFFSET)) << REG_BITS) | rnum;                        \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));                                   \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                        }                                                                                                                       \
                        else if(isdigit(*argument)){                                                                                            \                      
                            double value = atof(argument);                                                                                      \
                            ProcessorContainer spucommand = num;                                                                                \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));                                   \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                            memcpy((void*)(buffer + buffer_offset), &value, sizeof(ProcessorContainer));                                        \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                        }                                                                                                                       \
                        else {                                                                                                                  \
                            printf("HUI ZNAET KAK ETO BUDET COMPILICA\n");                                                                       \
                            ProcessorContainer spucommand = num;                                                                                \
                            memcpy((void*)(buffer + buffer_offset), &spucommand, sizeof(ProcessorContainer));                                   \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                            undeflabels[undeflabelspos++] = {argument, buffer_offset};                                                          \
                            buffer_offset += sizeof(ProcessorContainer);                                                                        \
                        }                                                                                                                       \
                    }                                                                                                                           \
                }                                                                                                                               \
                else

struct Lashes
{
    char    LABEL_NAME[100];
    size_t  label_ip;
};

struct UndefLashes
{
    char*   UNDEF_LABEL_NAME;
    size_t  UNDEF_BUFFER_POS;
};

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
        /*MAX LABEL SIZE*/
        static const size_t MAX_LABEL_NUMBER    = 1000;
        /*the extra byte for the last character*/
        static const size_t EXTRA_BYTE = 1;
        /*Size of input buffer*/
        size_t inbuffersize = 0;

        Lashes      labels      [MAX_LABEL_NUMBER] = {};
        UndefLashes undeflabels [MAX_LABEL_NUMBER] = {};
        size_t labelspos        = 0;
        size_t undeflabelspos   = 0;

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

        size_t ip = 0;

        while(sscanf(inbuffer + inbuffer_offset, "%s%n", asmfunc, &asm_offset) != EOF){

            inbuffer_offset += (size_t)asm_offset;
            
            /*processing comments*/
            if(*asmfunc == ';'){
                while(  *(inbuffer + inbuffer_offset) != '\n' && 
                        *(inbuffer + inbuffer_offset) != 0)
                    inbuffer_offset++;
                continue;
            }
            if(*(asmfunc + asm_offset - 2) == ':'){
                strncpy(labels[labelspos].LABEL_NAME, asmfunc, asm_offset - 2);
                labels[labelspos].label_ip = ip + 1;
                labelspos++;
                continue;
                //Массив запоминающий лабелы
                // ip add
                // структура char* -> addr = label
                // можно пройтись два раза
                // либо можно записывать незнакомые лэйблы в новыую структуру и после всего соотнести их с найденными, или ошибка

                //jnl jne je jl DSL-> написать код, stak pop pop
            }
            if(labelspos)
            printf("def = %s\n", labels[0].LABEL_NAME);
            //Incrase command pos
            ip++;

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
    for(size_t udpos = 0; udpos < undeflabelspos; udpos++){
        for(size_t dpos = 0; dpos < labelspos; dpos++){
            printf("undef = %s, def = %s names\n", undeflabels[udpos].UNDEF_LABEL_NAME, labels[dpos].LABEL_NAME);
            if(strcmp(undeflabels[udpos].UNDEF_LABEL_NAME, labels[dpos].LABEL_NAME) == 0){
                printf("Writed ip:%lu\n", labels[dpos].label_ip);
                memcpy((void*)(buffer + undeflabels[udpos].UNDEF_BUFFER_POS), &labels[dpos].label_ip, sizeof(size_t));
            }
            else if(dpos == labelspos - 1){
                printf("Bad label %s!\n", undeflabels[udpos].UNDEF_LABEL_NAME);
                exit(-1);
            }
        }
    }

    fwrite(buffer, buffer_offset, sizeof(char), outputfile);

    free(buffer);
    free(inbuffer);
    fclose(outputfile);
}