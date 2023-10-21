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

    static const int64_t REG_BITS       = 24    ;

    static const int64_t MASK_CODE          = (int64_t)0x000000000000FFFF;
    static const int64_t MASK_STATUS        = (int64_t)0x0000000000FF0000;
    static const int64_t MASK_REG           = (int64_t)0x00000000FF000000;

    static const int64_t REG_BIT = 1 << 16;
    static const int64_t NUM_BIT = 1 << 17;
    static const int64_t MEM_BIT = 1 << 18;
    
    static const int64_t ONE_VALUE      = 1     ;

    static const double epsilan = 0.0000001;


typedef uint64_t    ProcessorError;

    #define DEBUG

    #ifdef DEBUG
        #define ON_DEBUG(...) __VA_ARGS__
    #else
        #define ON_DEBUG(...)
    #endif

#endif // !CALC_CONFIG_H