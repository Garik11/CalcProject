#ifndef Text_read_file
#define Text_read_file

#include "Text.h"

//ADDITIONAL_BYTE for last symbol != \n
const size_t ADDITIONAL_BYTE = 1;

const int FSIZE_SUCCES = 0;
const int FREAD_SUCCES = 1;

size_t  fsize(const char* const FILE_NAME, size_t *FILE_SIZE);
Text*   readTextFromFile(const char* const FILE_NAME, enum TextErrors* error);

size_t  calculateRows(char* text, const size_t BSIZE);

void    makePointers(char* buffer, char** text, const size_t BSIZE);

#endif