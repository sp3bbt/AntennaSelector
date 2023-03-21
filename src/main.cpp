#include <Arduino.h>
#include "CLI.h"


#include "Utils.h"
#include "Logger.h"
#include "AntennaSelector.h"
volatile static uint8_t intInput{};
volatile static bool isInputReady = false;
// an interrupt function should be as short as possible
// this one is short to the extreame
static void inputReady();

// clang-format off
AntennaConfig config {
  0x3F, // enable all by default
  0xFF,
  0xFF,
  0xFF,
  {
    {"GP KF", "80-10"},
    {"Moxon", "20-15-10"},
    {"Fan Dipole", "160-10"},
    {"Delta KF", "80-6"},
    {"INV-L", "All band"},
    {"Antena 6", "Opis 6"},
  }
};
// clang-format on
#if 1
AntennaSelector antennaSelector;

void loop()
{
    cli();
    if (isInputReady) {
        isInputReady = false;
        sei();
        delay(2);
        uint8_t input = readInput();

        antennaSelector.processInput(input);

        cli();
    }
    sei();

    if (antennaSelector.shouldRedraw()) {
        antennaSelector.redraw();
    }
    CLIreadSerial();
}


#else
LCD lcd;
void loop()
{


}
#endif
static void inputReady()
{
    isInputReady = true;
    intInput = readInput();
}

void setup()
{
    Serial.begin(9600);
    // enable pull-up resistors for PordD pins 2..7
    PORTD |= B11111100;
    /// set PortD pins 2..7 as inputs
    DDRD &= B00000011;

    turnOffAll();
    DDRB |= B00111111;// set PortB pins 0..5 as output
    turnOffAll();

    // setup interrupt on pin-2 -> PortD.2
    attachInterrupt(digitalPinToInterrupt(2), inputReady, CHANGE);
    // setup interrupt on pin-3 -> PortD.3
    attachInterrupt(digitalPinToInterrupt(3), inputReady, CHANGE);
    CLIsetup();

    c_notice("antennaSelector init...");
    antennaSelector.init();
    c_notice("antennaSelector init done");
}

