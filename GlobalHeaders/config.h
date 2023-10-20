#ifndef CALC_CONFIG_H
#define CALC_CONFIG_H

    #include <stddef.h>

    static const size_t CODE_START_SIZE = 1000;
    static const size_t IP_START_POS = 0;

    typedef int64_t ProcessorContainer;
    typedef double ProcessorArgumentType;

    static uint64_t PROC_POISON_HEX_NUM = 0xDEADBABE;
    static ProcessorArgumentType PROC_POISON_NUM = *((ProcessorArgumentType*)&PROC_POISON_HEX_NUM);
    static const size_t PROC_CODE_MULT = 2;

    #define ProcesseorSpecificator "ld"
    #define SpecificatorSize        "4"

    static const int64_t MASK_CODE      = (int64_t)0x00000000FFFFFFFF;
    static const int64_t MASK_REG       = (int64_t)0xFFFFFFFF00000000;

    static const int64_t REG_BITS       = 32    ;
    static const int64_t REG_OFFSET     = 1     ;
    static const int64_t ONE_VALUE      = 1     ;

typedef uint64_t    ProcessorError;

    #define DEBUG

    #ifdef DEBUG
        #define ON_DEBUG(...) __VA_ARGS__
    #else
        #define ON_DEBUG(...)
    #endif

#endif // !CALC_CONFIG_H