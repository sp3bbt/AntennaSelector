#pragma once
#include <Arduino.h>

#define c_print(c_str)       \
    do {                     \
        Serial.print(c_str); \
    } while (0)

#define c_println(c_str)       \
    do {                       \
        Serial.println(c_str); \
    } while (0)

#ifndef PRINT_LINE_SIZE_MAX
#define PRINT_LINE_SIZE_MAX 64
#endif

#define c_printf(args...)                   \
    do {                                    \
        char line[PRINT_LINE_SIZE_MAX];     \
        snprintf(line, sizeof(line), args); \
        c_print(line);                      \
    } while (0)

#define c_printfln(args...)                   \
    do {                                    \
        char line[PRINT_LINE_SIZE_MAX];     \
        snprintf(line, sizeof(line), args); \
        c_println(line);                      \
    } while (0)
