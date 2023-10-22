#include "Assembler.h"

void toupper_all(char *str)
{
    for (char *p = str; *p != 0; p++)
        *p = (char)toupper(*p);
}

// функция аргумент
void argument_determinant(  
                            char *pointer_to_write      , 
                            size_t *offset              , 
                            int64_t bytecode            , 
                            char *argument              , 
                            int argument_size           , 
                            UndefLashes *undeflabels    , 
                            size_t* undeflabelspos      , 
                            size_t *ip
                        )
{
    ProcessorContainer spucommand;
    if (argument[0] == 'R' && argument[2] == 'X') /*if argument*/
    {
        spucommand = (((int64_t)((argument[1] - 'A'))) << REG_BITS) | (bytecode | REG_BIT);
        memcpy((void *)(pointer_to_write + (*offset)), &spucommand, sizeof(ProcessorContainer));
        (*offset) += sizeof(ProcessorContainer);
    }
    else if (isdigit(*argument)) /*if num*/
    {
        spucommand = bytecode | NUM_BIT;
        (*ip)++;
        double value = atof(argument);
        memcpy((void *)(pointer_to_write + (*offset)), &spucommand, sizeof(ProcessorContainer));
        (*offset) += sizeof(ProcessorContainer);
        memcpy((void *)(pointer_to_write + (*offset)), &value, sizeof(ProcessorContainer));
        (*offset) += sizeof(ProcessorContainer);
    } else if((*argument) == '[' && *(argument + argument_size - 2) == ']'){
        if(argument[1] == 'R' && argument[3] == 'X'){
            spucommand = (((int64_t)((argument[2] - 'A'))) << REG_BITS) | (bytecode | MEM_BIT | REG_BIT);
            memcpy((void *)(pointer_to_write + (*offset)), &spucommand, sizeof(ProcessorContainer));
            (*offset) += sizeof(ProcessorContainer);
        }
        else {
            spucommand = bytecode | MEM_BIT | NUM_BIT;
            (*ip)++;
            double value = atof(argument + ONE_SYMBOL_SKIP);
            memcpy((void *)(pointer_to_write + (*offset)), &spucommand, sizeof(ProcessorContainer));
            (*offset) += sizeof(ProcessorContainer);
            memcpy((void *)(pointer_to_write + (*offset)), &value, sizeof(ProcessorContainer));
            (*offset) += sizeof(ProcessorContainer);
        }
    }
    else /*if jmp mark*/
    {
        spucommand = bytecode;
        memcpy((void *)(pointer_to_write + (*offset)), &spucommand, sizeof(ProcessorContainer));
        (*offset) += sizeof(ProcessorContainer);
        strncpy(undeflabels[*undeflabelspos].UNDEF_LABEL_NAME, argument, (size_t)(argument_size - ONE_SYMBOL_SKIP));
        undeflabels[*undeflabelspos].UNDEF_BUFFER_POS = (*offset);
        (*undeflabelspos)++;
        (*offset) += sizeof(ProcessorContainer);
        (*ip)++;
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
            /* TODO move to function */                                                             \
            sscanf(inbuffer + inbuffer_offset, "%s%n", argument, &asm_offset);                      \
            inbuffer_offset += (size_t)asm_offset;                                                  \
            argument_determinant(                                                                   \
                                    outbuffer,                                                      \
                                    &outbuffer_offset,                                              \
                                    num,                                                            \
                                    argument,                                                       \
                                    asm_offset,                                                     \
                                    undeflabels,                                                    \
                                    &undeflabelspos,                                                \
                                    &ip                                                             \
                                );                                                                  \
        }                                                                                           \
    }                                                                                               \
    else


void assembler(const char *FILE_NAME_INPUT, const char *FILE_NAME_OUTPUT)
{
    assert(FILE_NAME_INPUT != NULL);
    assert(FILE_NAME_OUTPUT != NULL);

    static char asmfunc[MAX_FUNC_NAME_SIZE] = {};

    /*Size of input buffer*/
    size_t inbuffer_size = 0;
    size_t fsize_status = fsize(FILE_NAME_INPUT, &inbuffer_size);
    assert(fsize_status == FSIZE_SUCCES);

    char *inbuffer = (char *)calloc(sizeof(char), inbuffer_size + EXTRA_BYTE);
    assert(inbuffer != NULL);

    FILE *infile = fopen(FILE_NAME_INPUT, "r");
    assert(infile != NULL);

    size_t fread_size = fread(inbuffer, inbuffer_size, sizeof(char), infile);
    assert(fread_size == FREAD_SUCCES);

    toupper_all(inbuffer);
    fclose(infile);

    Lashes      labels      [MAX_LABEL_NUMBER] = {};
    UndefLashes undeflabels [MAX_LABEL_NUMBER] = {};
    size_t labelspos        = 0;
    size_t undeflabelspos   = 0;

    FILE *outputfile = fopen(FILE_NAME_OUTPUT, "wb");

    assert(outputfile != NULL);

    size_t outbuffer_size = START_BUFFER_SIZE;
    char *outbuffer = (char *)calloc(outbuffer_size, sizeof(char));
    assert(outbuffer != NULL);

    size_t  outbuffer_offset    = 0;
    size_t  inbuffer_offset     = 0;
    int     asm_offset          = 0;

    size_t ip = 0;

    memcpy(outbuffer + outbuffer_offset, AUTHORS_NAME,  sizeof(AUTHORS_NAME));
    outbuffer_offset += sizeof(AUTHORS_NAME);
    memcpy(outbuffer + outbuffer_offset, VERSION,       sizeof(VERSION)     );
    outbuffer_offset += sizeof(VERSION);

    while (sscanf(inbuffer + inbuffer_offset, "%s%n", asmfunc, &asm_offset) != EOF)
    {

        inbuffer_offset += (size_t)asm_offset;

        /*processing comments*/
        if (*asmfunc == ';')
        {
            while (*(inbuffer + inbuffer_offset) != '\n' &&
                   *(inbuffer + inbuffer_offset) != 0)
                inbuffer_offset++;
            continue;
        }
        /*processing marks*/
        if (*(asmfunc + asm_offset - 2) == ':')
        {
            strncpy(labels[labelspos].LABEL_NAME, asmfunc, (size_t)(asm_offset - 2));
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
    for (size_t udpos = 0; udpos < undeflabelspos; udpos++)
    {
        for (size_t dpos = 0; dpos < labelspos; dpos++)
        {
            if (strcmp(undeflabels[udpos].UNDEF_LABEL_NAME, labels[dpos].LABEL_NAME) == 0)
            {
                memcpy((void *)(outbuffer + undeflabels[udpos].UNDEF_BUFFER_POS), &labels[dpos].label_ip, sizeof(size_t));
                break;
            }
            else if (dpos == labelspos - 1)
            {
                exit(-1);
            }
        }
    }

    fwrite(outbuffer, outbuffer_offset, sizeof(char), outputfile);

    free(outbuffer);
    free(inbuffer);
    fclose(outputfile);
}