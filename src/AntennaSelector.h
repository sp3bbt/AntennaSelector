#pragma once

#include <LCD.h>

#include "Logger.h"
#include "AntennaConfig.h"
#include "InputState.h"

#define LCD_CHARS 16
#define LCD_LINES 2

extern AntennaConfig config;
extern void writeOutput(int index);
#define IS_EVENT(input, event) (((input) & (1 << (event))) != 0)

class AntennaSelector
{
  private:

    void updateIndex();
    void initLCD();
  public:
    AntennaSelector();
    void init();
    void processInput(uint8_t input);
    int findIndex(int idx, int inc) const;
    int findEnabledIndex(int idx, int inc) const;
    void processNormalState(uint16_t input_events);
    void setOutput(int idx);
    void processEditState(uint16_t input_events);
    bool shouldRedraw(bool reset = true);
    void redraw();

  private:
    enum class State
    {
        normal,
        edit
    };

    LCD lcd;
    InputState inputState;
    State state{};
    bool alt{};
    int index;

    bool _shouldRedraw{true};
    void render();
    void renderLine1(int cnt);
    void renderLine2(int cnt);
};
