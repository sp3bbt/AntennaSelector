#include "Utils.h"
#include "Logger.h"

uint8_t readInput()
{
    uint8_t inD = PIND;
    return (((inD & B11111100) >> 2) & B00111111);
}

int clipRound(int n, int min, int max)
{
    if (n < min)
        n = max - 1;
    else if (n >= max) {
        n = min;
    }
    return n;
}

#ifdef NEGATIVE_OUTPUT_LOGIC
void turnOffAll()
{
    PORTB |= B00111111;
}

void writeOutput(int idx)
{
    if ((idx > 5) || (idx < 0)) {
        c_warning("writeOutput idx: %d", idx);
        turnOffAll();
    } else {
        byte portb = PORTB;
        byte newportb = (B00111111 & (~static_cast<uint8_t>(1 << idx)));

        if ((portb & B00111111) != newportb) {
            PORTB |= B00111111;
            PORTB &= ~static_cast<uint8_t>(1 << idx);
        }
    }
}
#else
void turnOffAll()
{
    PORTB &= B11000000;
}

void writeOutput(int idx)
{
    if ((idx > 5) || (idx < 0)) {
        turnOffAll();
    } else {
        PORTB &= B11000000;
        PORTB |= static_cast<uint8_t>(1 << idx);
    }
}
#endif
