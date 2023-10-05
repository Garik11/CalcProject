#include <stdio.h>

#include "./FileWork/TextReadFile.h"
#include "./Processor/Processor.h"
#include "./Assembler/Assembler.h"
#include "./Disassembler/Disassembler.h"

int main(void){
    const char FILE_NAME_INPUT          [] = "./CommandSequence/1000_7.txt"       ;
    const char FILE_NAME_OUTPUT         [] = "./CommandSequence/1000_7.eq"        ;
    const char FILE_NAME_DISASSEMBLER   [] = "./CommandSequence/1000_7_ASM.txt"   ;

    compile(FILE_NAME_INPUT, FILE_NAME_OUTPUT);
    calc(FILE_NAME_OUTPUT);
    decompile(FILE_NAME_OUTPUT, FILE_NAME_DISASSEMBLER);
}