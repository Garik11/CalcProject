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
    if(tx == NULL)
        return;
    tx -> buffer        = NULL;
    tx -> buffer_size   = TEXT_POISONED_NUM;
    tx -> text          = NULL;
    tx -> lines         = TEXT_POISONED_NUM;
    free(tx);
}

void textOut(const Text* tx){
    if(tx == NULL)
        return;
    for(size_t i = 0; i < tx->lines; i++)
        printf("%s\n", tx->text[i]);
}

void textCheckErrors(TextErrors errors){
    if(errors & TEXT_CALLOC_ERROR           ) print_text_error(TEXT_CALLOC_ERROR        );
    if(errors & TEXT_FILEOPEN_ERROR         ) print_text_error(TEXT_FILEOPEN_ERROR      );
    if(errors & TEXT_FILESTATUS_ERROR       ) print_text_error(TEXT_FILESTATUS_ERROR    );
    if(errors & TEXT_CALLOC_BUFFER_ERROR    ) print_text_error(TEXT_CALLOC_BUFFER_ERROR );
    if(errors & TEXT_FREADSTATUS_ERROR      ) print_text_error(TEXT_FREADSTATUS_ERROR   );
    if(errors & TEXT_CALLOC_TEXT_ERROR      ) print_text_error(TEXT_CALLOC_TEXT_ERROR   );
}