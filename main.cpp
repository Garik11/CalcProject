#include <stdio.h>

#include "./FileWork/TextReadFile.h"
#include "./Processor/Processor.h"
#include "./Assembler/Assembler.h"
#include "./Disassembler/Disassembler.h"

int main(void){
    const char FILE_NAME_INPUT          [] = "./CommandSequence/kvad.txt"       ;
    const char FILE_NAME_OUTPUT         [] = "./CommandSequence/kvad.eq"        ;
    const char FILE_NAME_DISASSEMBLER   [] = "./CommandSequence/kvad_ASM.txt"   ;

    assembler     (FILE_NAME_INPUT, FILE_NAME_OUTPUT            );
    processor     (FILE_NAME_OUTPUT                             );
    disassembler  (FILE_NAME_OUTPUT, FILE_NAME_DISASSEMBLER     );
}