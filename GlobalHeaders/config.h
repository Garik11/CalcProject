#ifndef CALC_CONFIG_H
#define CALC_CONFIG_H

    #define DEBUG

    #ifdef DEBUG
        #define ON_DEBUG(...) __VA_ARGS__
    #else
        #define ON_DEBUG(...)
    #endif

#endif // !CALC_CONFIG_H