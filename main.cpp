#include <stdio.h>
#include "./FileWork/Text_read_file.h"
#include "./Processor/Processor.h"

int main(void){
    const char FILE_NAME[] = "./CommandSequence/test.eq";

    TextErrors err = {};
    Text *text = readTextFromFile(FILE_NAME, &err);
    
    calc(text->text, text->lines);

    printf("%lf\n", sin(4));

    //textOut(text);
    textDtor(text);
}