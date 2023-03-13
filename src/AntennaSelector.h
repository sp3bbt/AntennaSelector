#pragma once

#include <LCD.h>
#include "Logger.h"
#include <EEPROM.h>
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
    void updateIndex()
    {
        if ((config.enable & (1 << index)) == 0) {
            index = findEnabledIndex(index, +1);
        }
        writeOutput(index);
        _shouldRedraw = true;
    }
    void initLCD()
    {
        lcd.begin(LCD_CHARS, LCD_LINES);
        lcd.clear();
    }
  public:

    void init()
    {
        inputState.init();
        config.enable = 0x3F & EEPROM.read(0);
        updateIndex();
    }

    void processInput()
    {
        uint16_t input_events = inputState.update();
        switch (state) {
            case State::normal:
                processNormalState(input_events);
                break;
            case State::edit:
                processEditState(input_events);
                break;
        }
    }
    int findIndex(int idx, int inc) const
    {
        return clip_round(idx + inc, 0, ANTENNA_COUNT);
    }
    int findEnabledIndex(int idx, int inc) const
    {
        if (idx == -1)
            idx = 0;

        int i = idx;

        do {
            int oldi = i;
            i = findIndex(i, inc);
            c_debug("idx: %d -> %d", oldi, i);
            if ((config.enable & (1 << i)) != 0) {
                c_debug(" enabled");
                return i;
            } else {
                c_debug(" disabled");
            }

        } while (i != idx);
        return -1;
    }


    void processNormalState(uint16_t input_events)
    {
        int lastIndex = index;
        if (IS_EVENT(input_events, InputState::sw0_pressed_2)) {
            state = State::edit;
            c_info("sw0 pressed 2");
            index = clip_round(index, 0, ANTENNA_COUNT);
        } else if (IS_EVENT(input_events, InputState::sw0_clicked)) {
            c_info("sw0 clicked");
            alt = !alt;
        } else if (IS_EVENT(input_events, InputState::encoder_left) ||
                   IS_EVENT(input_events, InputState::sw1_clicked)) {
            c_info("left");
            index = findEnabledIndex(index, -1);
        } else if (IS_EVENT(input_events, InputState::encoder_right) ||
                   IS_EVENT(input_events, InputState::sw2_clicked)) {
            c_info("right");
            index = findEnabledIndex(index, +1);
        } else {
            c_debug("ignore inEnt: %04x", (unsigned)input_events);
        }
        if (index != lastIndex) {
            setOutput(index);
        }
        _shouldRedraw = true;
    }
    void setOutput(int idx)
    {
        writeOutput(idx);
    }
    void processEditState(uint16_t input_events)
    {
        if (IS_EVENT(input_events, InputState::sw0_pressed_2)) {
            c_info("sw0 pressed 2");
            state = State::normal;
            EEPROM.update(0, config.enable | 0xC0);
            updateIndex();
        } else if (IS_EVENT(input_events, InputState::sw0_clicked)) {
            c_info("sw0 clicked");
            bool en = (config.enable & (1 << index)) != 0;
            if (en) {
                config.enable &= ~static_cast<uint8_t>(1 << index);
            } else {
                config.enable |= static_cast<uint8_t>(1 << index);
            }
        } else if (IS_EVENT(input_events, InputState::encoder_left) ||
                   IS_EVENT(input_events, InputState::sw1_clicked)) {
            c_info("left");
            index = findIndex(index, -1);

        } else if (IS_EVENT(input_events, InputState::encoder_right) ||
                   IS_EVENT(input_events, InputState::sw2_clicked)) {
            c_info("right");

            index = findIndex(index, +1);
        }
        _shouldRedraw = true;
    }

    bool shouldRedraw(bool reset = true)
    {
        bool rd = _shouldRedraw;
        if (reset)
            _shouldRedraw = false;
        return rd;
    }

    void redraw()
    {
        render();
    }

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
const char all_disabled_str[] = "--all disabled--";
void AntennaSelector::renderLine1(int cnt)
{
    char line[LCD_CHARS + 1];
    if (cnt >= 0 && cnt < ANTENNA_COUNT) {
        snprintf(line, sizeof(line), "%d:%s", (cnt + 1), config.antennas[cnt].name);
    } else {
        snprintf(line, sizeof(line), all_disabled_str);
    }
    lcd.setCursor(0, 0);
    lcd.print(line);
}

void AntennaSelector::renderLine2(int n)
{
    char line[LCD_CHARS + 1];
    const char digits[] = "0123456789";
    const int N = ANTENNA_COUNT;
    memset(line, ' ', sizeof(line));
    line[sizeof(line) - 1] = 0;
    if (state == State::normal && alt) {
        if (n >= 0 && n < ANTENNA_COUNT) {
            snprintf(line, sizeof(line), "%s", config.antennas[n].desc);
        } else {
            snprintf(line, sizeof(line), all_disabled_str);
        }
    } else {
        if (state == State::edit) {
            if (n >= 0 && n < ANTENNA_COUNT) {
                snprintf(line, sizeof(line), "[%d] %s", (n + 1), ((config.enable & (1 << n)) != 0) ? "On" : "Off");
            } else {
                snprintf(line, sizeof(line), all_disabled_str);
            }
        } else {
            char* ptr = line + 2;
            for (int i = 0; i < N; ++i) {
                if (i != n) {
                    if (i != (n + 1))
                        *ptr++ = ' ';
                    if ((config.enable & (1 << i)) != 0) {
                        *ptr++ = digits[i + 1];
                    } else {
                        *ptr++ = ' ';
                    }
                } else {
                    *ptr++ = '[';
                    *ptr++ = digits[i + 1];
                    *ptr++ = ']';
                }
            }
        }
    }
    lcd.setCursor(0, 1);
    lcd.print(line);
}

void AntennaSelector::render()
{
    lcd.clear();
    renderLine1(index);
    renderLine2(index);
}

