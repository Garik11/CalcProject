#include "Text.h"

void textNullification(Text* tx){
    tx -> buffer        = NULL;
    tx -> buffer_size   = 0;
    tx -> text          = NULL;
    tx -> lines         = 0;
}

Text* textCtor(
    char*   buffer, 
    size_t  buffer_size, 
    char**  text, 
    size_t  lines)
{
    Text* tx = (Text*)calloc(sizeof(Text), 1);
    if(tx == NULL)
        return NULL;
    tx->buffer          = buffer;
    tx->buffer_size     = buffer_size;
    tx->text            = text;
    tx->lines           = lines;
    return tx;
}

void textDtor(Text* tx){
    tx -> buffer        = NULL;
    tx -> buffer_size   = TEXT_POISONED_NUM;
    tx -> text          = NULL;
    tx -> lines         = TEXT_POISONED_NUM;
    free(tx);
}

void textOut(const Text* tx){
    for(size_t i = 0; i < tx->lines; i++)
        printf("%s\n", tx->text[i]);
}