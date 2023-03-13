#include <stdint.h>


#if defined(STANDARD_LCD)
#include <LCDi2c.h>
#elif defined(GROVE_LCD)
#include "rgb_lcd.h"
#endif

class LCD
{
    public:
        LCD();
        LCD(uint8_t address);
        void begin(uint8_t col, uint8_t row);
        void clear();
        void setCursor(uint8_t col, uint8_t row);
        void print(const char* s);
        void println(const char* s);
    private:
#if defined(STANDARD_LCD)
    LCDi2c lcd; // I2C address
#elif defined(GROVE_LCD)
    rgb_lcd lcd; // 0x3E
#endif
};
