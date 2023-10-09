#ifndef CALC_CONFIG_H
#define CALC_CONFIG_H

    #include <stddef.h>

    static size_t CODE_START_SIZE = 1000;
    static size_t IP_START_POS = 0;

    #define DEBUG

    #ifdef DEBUG
        #define ON_DEBUG(...) __VA_ARGS__
    #else
        #define ON_DEBUG(...)
    #endif

#endif // !CALC_CONFIG_H