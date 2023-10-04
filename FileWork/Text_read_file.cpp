#include "Text_read_file.h"
size_t fsize(const char* const FILE_NAME, size_t *FILE_SIZE){
    assert(FILE_SIZE != NULL);

    struct stat st;
    size_t stat_status = (size_t)stat(FILE_NAME, &st);
    *FILE_SIZE = (size_t)st.st_size;
    return stat_status;
}

size_t calculateRows(char* text, const size_t BSIZE){
    assert(text != NULL);

    size_t output = 0;
    for(size_t i = 0; i < BSIZE; i++){
        if(text[i] == '\n')
            output++;
    }
    return output;
}

void makePointers(char* buffer, char** text, const size_t BSIZE){
    assert(buffer != NULL);
    assert(text != NULL);

    text[0] = buffer;
    for(size_t i = 0, lines = 0; i < BSIZE; i++){
        if(buffer[i] == '\n'){
            buffer[i] = '\0';
            text[++lines] = buffer + i + 1;
        }
    }
}

Text* readTextFromFile(const char* const FILE_NAME, enum TextErrors* error){
    assert(error != NULL);

    *error = TEXT_ALL_OK;
    Text* output = (Text*)calloc(1, sizeof(Text));
    if(output == NULL){
        *error = TEXT_CALLOC_ERROR;
        return NULL;
    }

    textNullification(output);

    FILE* fp = fopen(FILE_NAME, "r");
    if(fp == NULL){
        *error = TEXT_FILEOPEN_ERROR;
        textDtor(output);
        return NULL;
    }

    size_t fsize_status = fsize(FILE_NAME, &output->buffer_size);
    if(fsize_status != FSIZE_SUCCES){
        *error = TEXT_FILESTATUS_ERROR;
        textDtor(output);
        return NULL;
    }
    output->buffer = (char*)calloc(output->buffer_size, sizeof(char));
    if(output->buffer == NULL){
        *error = TEXT_CALLOC_BUFFER_ERROR;
        textDtor(output);
        return NULL;
    }
    size_t fread_status = fread(output->buffer, output->buffer_size, sizeof(char), fp);
    if(fread_status != FREAD_SUCCES){
        *error = TEXT_FREADSTATUS_ERROR;
        textDtor(output);
        return NULL;
    }

    fclose(fp);

    output->lines = calculateRows(output->buffer, output->buffer_size);

    output->text = (char**)calloc(output->lines + 1, sizeof(char*));
    if(output->text == NULL){
        *error = TEXT_CALLOC_TEXT_ERROR;
        textDtor(output);
        return NULL;
    }

    makePointers(output->buffer, output->text, output->buffer_size);

    return output;
}