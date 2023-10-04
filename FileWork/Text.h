#ifndef Text_F
#define Text_F

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <malloc.h>

static const size_t TEXT_POISONED_NUM = -1ull / 2;

struct Text
{
    char*   buffer;
    size_t  buffer_size;
    char**  text;
    size_t  lines;
};

enum TextErrors{
    TEXT_ALL_OK             = 0,
    TEXT_CALLOC_ERROR       = 1,
    TEXT_FILEOPEN_ERROR     = 2,
    TEXT_FILESTATUS_ERROR   = 4,
    TEXT_CALLOC_BUFFER_ERROR= 8,
    TEXT_FREADSTATUS_ERROR  = 16,
    TEXT_CALLOC_TEXT_ERROR  = 32,
};

void textNullification(Text* tx);

Text* textCtor(
    char*   buffer, 
    size_t  buffer_size, 
    char**  text, 
    size_t  lines);

void textDtor(Text* tx);

void textOut(const Text* tx);

#endif