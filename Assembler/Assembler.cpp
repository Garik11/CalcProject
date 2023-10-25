#include "Assembler.h"

#warning jump to num

void toupper_all(char *str){
    assert(str != NULL);
    for (char *p = str; *p != 0; p++)
        *p = (char)toupper(*p);
}

int sscanf_s_fidex_n(const char* input, char* output, size_t* input_offset, size_t* output_size){

    static const int SCNAF_SUCCES = 2;

    assert(input        != NULL);
    assert(output       != NULL);
    assert(input_offset != NULL);
    assert(output_size  != NULL);

    *input_offset   = 0;
    *output_size    = 0;

    while(isspace(*input) && *input != 0){
        (*input_offset)++;
        input++;
    }
    while(isspace(*input) == 0 && *input != 0){
        *output = *input;
        output  ++;
        input   ++;
        (*input_offset) ++;
        (*output_size)  ++;
    }

    *output = 0;//Nuller last symbol

    if(*output_size == 0)   return EOF          ;
    else                    return SCNAF_SUCCES ;
}

#warning outbuffer-> pointer, del ip
// функция аргумент
void argument_determinant(  
                            char*           outbuffer           , 
                            size_t*         outbuffer_offset    , 
                            int64_t         bytecode            , 
                            char*           argument            , 
                            size_t          argument_size       , 
                            UndefLashes*    undeflabels         , 
                            size_t*         undeflabelspos      , 
                            size_t*         ip
                        )
{

    ProcessorContainer spucommand = {};

    if (argument[0] == 'R' && argument[2] == 'X') /*if register*/
    {
        spucommand = (((int64_t)((argument[1] - 'A'))) << REG_BITS) | (bytecode | REG_BIT);
        memcpy((void *)(outbuffer + (*outbuffer_offset)), &spucommand, sizeof(ProcessorContainer));
        (*outbuffer_offset) += sizeof(ProcessorContainer);
    }
    else if (isdigit(*argument)) /*if num*/
    {
        (*ip)++;
        spucommand = bytecode | NUM_BIT;

        ProcessorArgumentType value = atof(argument);
        memcpy((void *)(outbuffer + (*outbuffer_offset)), &spucommand, sizeof(ProcessorContainer));
        (*outbuffer_offset) += sizeof(ProcessorContainer);
        memcpy((void *)(outbuffer + (*outbuffer_offset)), &value, sizeof(ProcessorContainer));
        (*outbuffer_offset) += sizeof(ProcessorContainer);

    } else if((*argument) == '[' && *(argument + argument_size - 2) == ']'){ /*if mem*/
        #warning deduplicate
        if(argument[1] == 'R' && argument[3] == 'X'){   /*if mem [register]*/

            spucommand = (((int64_t)((argument[2] - 'A'))) << REG_BITS) | (bytecode | MEM_BIT | REG_BIT);

            memcpy((void *)(outbuffer + (*outbuffer_offset)), &spucommand, sizeof(ProcessorContainer));
            (*outbuffer_offset) += sizeof(ProcessorContainer);

        }
        else {  /*if mem [num]*/
            #warning deduplicate
            (*ip)++;
            spucommand = bytecode | MEM_BIT | NUM_BIT;

            ProcessorArgumentType value = atof(argument + ONE_SYMBOL_SKIP);

            memcpy((void *)(outbuffer + (*outbuffer_offset)), &spucommand, sizeof(ProcessorContainer));
            (*outbuffer_offset) += sizeof(ProcessorContainer);
            memcpy((void *)(outbuffer + (*outbuffer_offset)), &value, sizeof(ProcessorContainer));
            (*outbuffer_offset) += sizeof(ProcessorContainer);

        }
    }
    else /*if jmp mark*/
    {
        #warning label is num. move to function and handle like num
        (*ip)++;
        spucommand = bytecode;

        memcpy((void *)(outbuffer + (*outbuffer_offset)), &spucommand, sizeof(ProcessorContainer));
        (*outbuffer_offset) += sizeof(ProcessorContainer);
        strncpy(undeflabels[*undeflabelspos].UNDEF_LABEL_NAME, argument, argument_size - ONE_SYMBOL_SKIP);
        undeflabels[*undeflabelspos].UNDEF_BUFFER_POS = (*outbuffer_offset);
        (*undeflabelspos)++;


        (*outbuffer_offset) += sizeof(ProcessorContainer);
    }
}

#define DEF_CMD(name, num, args, ...)                                                               \
    if (strcmp(asmfunc, #name) == 0)                                                                \
    {                                                                                               \
        if (args == 0)                                                                              \
        {                                                                                           \
            ProcessorContainer spucommand = num;                                                    \
            memcpy((void *)(outbuffer + outbuffer_offset), &spucommand, sizeof(ProcessorContainer));\
            outbuffer_offset += sizeof(ProcessorContainer);                                         \
        }                                                                                           \
        else if (args == 1)                                                                         \
        {                                                                                           \
            sscanf_s_fidex_n(                                                                       \
                                inbuffer + inbuffer_offset  ,                                       \
                                argument                    ,                                       \
                                &inbuffer_additional_offset ,                                       \
                                &asmfunc_size                                                       \
                            );                                                                      \
            inbuffer_offset += inbuffer_additional_offset;                                          \
            argument_determinant(                                                                   \
                                    outbuffer,                                                      \
                                    &outbuffer_offset,                                              \
                                    num,                                                            \
                                    argument,                                                       \
                                    asmfunc_size,                                                   \
                                    undeflabels,                                                    \
                                    &undeflabelspos,                                                \
                                    &ip                                                             \
                                );                                                                  \
        }                                                                                           \
    }                                                                                               \
    else


void assembler(const char *FILE_NAME_INPUT, const char *FILE_NAME_OUTPUT)
{
    assert(FILE_NAME_INPUT  != NULL);
    assert(FILE_NAME_OUTPUT != NULL);

    /*asm instruction buffer*/
    static char asmfunc[MAX_FUNC_NAME_SIZE] = {};

    /*Size of input buffer*/
    size_t inbuffer_size = 0;
    /*File size*/
    size_t fsize_status = fsize(FILE_NAME_INPUT, &inbuffer_size);
    assert(fsize_status == FSIZE_SUCCES);

    /*inbuffer*/
    char *inbuffer = (char *)calloc(sizeof(char), inbuffer_size + EXTRA_BYTE);
    assert(inbuffer != NULL);

    /*open file for inbuffer*/
    FILE *infile = fopen(FILE_NAME_INPUT, "r");
    assert(infile != NULL);

    /*file size*/
    size_t fread_status = fread(inbuffer, inbuffer_size, sizeof(char), infile);
    assert(fread_status == FREAD_SUCCES);

    fclose(infile);

    /*toupper all symbols in text*/
    toupper_all(inbuffer);

    Lashes      labels      [MAX_LABEL_NUMBER] = {};
    UndefLashes undeflabels [MAX_LABEL_NUMBER] = {};
    size_t labelspos        = 0;
    size_t undeflabelspos   = 0;

    FILE *outputfile = fopen(FILE_NAME_OUTPUT, "wb");

    assert(outputfile != NULL);

    size_t outbuffer_size = START_BUFFER_SIZE;
    char *outbuffer = (char*)calloc(outbuffer_size, sizeof(char));
    assert(outbuffer != NULL);

    size_t  outbuffer_offset            = 0;
    size_t  inbuffer_offset             = 0;
    size_t  inbuffer_additional_offset  = {};
    size_t  asmfunc_size                = {};

    size_t ip = 0;

    memcpy(outbuffer + outbuffer_offset, AUTHORS_NAME,  sizeof(AUTHORS_NAME));
    outbuffer_offset += sizeof(AUTHORS_NAME);
    memcpy(outbuffer + outbuffer_offset, VERSION,       sizeof(VERSION)     );
    outbuffer_offset += sizeof(VERSION);    
    
    while(sscanf_s_fidex_n(
                            inbuffer + inbuffer_offset  , 
                            asmfunc                     , 
                            &inbuffer_additional_offset , 
                            &asmfunc_size                 ) != EOF
        )
    {
        
        inbuffer_offset += inbuffer_additional_offset;

        /*processing comments*/
        if (*asmfunc == ';')
        {
            while (*(inbuffer + inbuffer_offset) != '\n' &&
                   *(inbuffer + inbuffer_offset) != 0)
                inbuffer_offset++;
            continue;
        }

        /*processing marks*/
        printf("sze = %lu, command = %s\n", asmfunc_size, asmfunc);
        assert(asmfunc_size != 0);
        if (*(asmfunc + asmfunc_size - 1) == ':' && asmfunc_size > 1)
        {
            strncpy(labels[labelspos].LABEL_NAME, asmfunc, asmfunc_size - 2);
            labels[labelspos].label_ip = ip;
            labelspos++;
            continue;
        }

        /*incrase now pos*/
        ip++;

        /*instuction argument*/
        static char argument[MAX_ARGUMENT_SIZE] = {};

        /*Calculate new outbuffer size*/
        if (outbuffer_offset + sizeof(ProcessorContainer) * INACCURACY >= outbuffer_size)
        {
            void *new_data = recalloc(outbuffer, outbuffer_size * BUFFER_MULTIPLIER, sizeof(char), outbuffer_size, sizeof(char));
            assert(new_data != NULL);
            outbuffer_size *= BUFFER_MULTIPLIER;
            outbuffer = (char *)new_data;
        }
        #include "../GlobalHeaders/DSL.h"
        /*else*/ void(0);
    }

    size_t counter_verified_marks = 0;
    for (size_t udpos = 0; udpos < undeflabelspos; udpos++)
    {
        for (size_t dpos = 0; dpos < labelspos; dpos++)
        {
            if (strcmp(undeflabels[udpos].UNDEF_LABEL_NAME, labels[dpos].LABEL_NAME) == 0)
            {
                counter_verified_marks++;
                double argument = (float)labels[dpos].label_ip;
                memcpy((void *)(outbuffer + undeflabels[udpos].UNDEF_BUFFER_POS), &argument, sizeof(ProcessorContainer));
                break;
            }
        }
    }

    assert(counter_verified_marks == undeflabelspos);

    fwrite(outbuffer, outbuffer_offset, sizeof(char), outputfile);

    free    (outbuffer  );
    free    (inbuffer   );
    fclose  (outputfile );
}