#pragma once
#include <Arduino.h>
#include "Print.h"

static inline uint8_t readInput()
{
    uint8_t inD = PIND;
    return (((inD & B11111100) >> 2) & B00111111);
}

static int clip_round(int n, int min, int max)
{
    if (n < min)
        n = max - 1;
    else if (n >= max) {
        n = min;
    }
    return n;
}

#ifdef NEGATIVE_OUTPUT_LOGIC
static inline void turnOffAll()
{
    PORTB |= B00111111;
}

static inline void writeOutput(int idx)
{
    if ((idx > 5) || (idx < 0)) {
        turnOffAll();
    } else {
        PORTB |= B00111111;
        PORTB &= ~static_cast<uint8_t>(1 << idx);
    }
}
#else
static inline void turnOffAll()
{
    PORTB &= B11000000;
}

static inline void writeOutput(int idx)
{
    if ((idx > 5) || (idx < 0)) {
        turnOffAll();
    } else {
        PORTB &= B11000000;
        PORTB |= static_cast<uint8_t>(1 << idx);
    }
}
#endif
