#include "LCD.h"

LCD::LCD()
#if defined(STANDARD_LCD)
    : lcd(0x27)// I2C address
#elif defined(GROVE_LCD)
    : lcd()// 0x3E
#endif
{
}

LCD::LCD(uint8_t address)
#if defined(STANDARD_LCD)
    : lcd(address)// I2C address
#elif defined(GROVE_LCD)
    : lcd()// 0x3E
#endif
{
}

void LCD::setCursor(uint8_t col, uint8_t row)
{
#if defined(STANDARD_LCD)
    lcd.locate(col, row);
#elif defined(GROVE_LCD)
    lcd.setCursor(col, row);
#endif
}

void LCD::print(const char* s)
{
    lcd.print(s);
}

void LCD::println(const char* s)
{
    lcd.println(s);
}

void LCD::begin(uint8_t col, uint8_t row)
{
    lcd.begin(col, row);
}

void LCD::clear()
{
#if defined(STANDARD_LCD)
    lcd.cls();
#elif defined(GROVE_LCD)
    lcd.clear();
#endif
}

#if 0
uint8_t num_1[8] = {
    0b11011,
    0b10011,
    0b11011,
    0b11011,
    0b11011,
    0b11011,
    0b10001,
};
uint8_t num_2[8] = {
    0b10001,
    0b01110,
    0b11110,
    0b11101,
    0b11011,
    0b10111,
    0b00000,
};
uint8_t num_3[8] = {
    0b00000,
    0b11101,
    0b11011,
    0b11101,
    0b11110,
    0b01110,
    0b10001,
};
uint8_t num_4[8] = {
    0b11101,
    0b11001,
    0b10101,
    0b01101,
    0b00000,
    0b11101,
    0b11101,
};
uint8_t num_5[8] = {
    0b00000,
    0b01111,
    0b00001,
    0b11100,
    0b11110,
    0b01110,
    0b10001,
};
uint8_t num_6[8] = {
    0b11001,
    0b10111,
    0b01111,
    0b00001,
    0b01110,
    0b01110,
    0b10001,
};
uint8_t arr_right[8] = {
    0b10000,
    0b11000,
    0b11100,
    0b11110,
    0b11100,
    0b11000,
    0b10000,
};
uint8_t arr_left[8] = {
    0b00001,
    0b00011,
    0b00111,
    0b01111,
    0b00111,
    0b00011,
    0b00001,
};
#endif
