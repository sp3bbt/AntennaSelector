#include <Arduino.h>
#include <EEPROM.h>
#include "CLI.h"


#define NEGATIVE_OUTPUT_LOGIC 1
#include "Utils.h"
#include "Logger.h"
#include "AntennaSelector.h"

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

AntennaSelector antennaSelector;

void loop()
{
    cli();
    if (isInputReady) {
        isInputReady = false;
        sei();

        antennaSelector.processInput();

        c_debug("==========\n\n");
        cli();
    }
    sei();
    if (antennaSelector.shouldRedraw()) {
        antennaSelector.redraw();
    }
    CLIreadSerial();
}

static void inputReady()
{
    // antennaSelector.readInput();
    isInputReady = true;
}

void setup()
{
    Serial.begin(9600);
    // enable pull-up resistors for PordD pins 2..7
    PORTD |= B11111100;
    /// set PortD pins 2..7 as inputs
    DDRD &= B00000011;

    DDRB |= B00111111;// set PortB pins 0..5 as output
    turnOffAll();

    // setup interrupt on pin-2 -> PortD.2
    attachInterrupt(digitalPinToInterrupt(2), inputReady, CHANGE);
    // setup interrupt on pin-3 -> PortD.3
    attachInterrupt(digitalPinToInterrupt(3), inputReady, CHANGE);
    CLIsetup();

    antennaSelector.init();
}

