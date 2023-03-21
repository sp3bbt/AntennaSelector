#pragma once
#include <Arduino.h>
#include "Utils.h"
#include "Logger.h"

#define CLICK_LIMIT 1000
#define PRESSED_1_LIMIT 2000

struct SwitchState
{
    enum State
    {
        depressed,
        pressed
    };

    enum Event
    {
        none,// when no event was generated
        clicked,// when press was shorter than CLICK_LIMIT
        pressed_0,// when switch was pressed
        pressed_1,// when switch was depressed after time: CLICK_LIMIT <= time < PRESSED_1_LIMIT
        pressed_2,// when switch was depressed afret time: PRESSED_1_LIMIT <= time
        event_last// just for convinience
    };

    State state{};
    unsigned long millisWhenPressed{};

    Event update(State newState);
};

// Input PINOUT
enum InputBit
{
    int0,
    clk,
    dt,
    sw0,
    sw1,
    sw2,
};

#define GET_BIT(byte, bit) ((byte & (1 << bit)) == 0)
#define DIFF_BIT(oldbyte, newbyte, bit) (GET_BIT(oldbyte, bit) != GET_BIT(newbyte, bit))

class InputState
{
  private:
    volatile uint8_t newState{};
    uint8_t lastState{};
    uint8_t currState{};
    SwitchState switches[3];

  public:
    enum InputEvent : uint16_t
    {
        none,// 0
        encoder_left,// 1
        encoder_right,// 2
        sw0_clicked,// 3
        sw0_pressed_0,// 4
        sw0_pressed_1,// 5
        sw0_pressed_2,// 6
        sw1_clicked,// 7
        sw1_pressed_0,// 8
        sw1_pressed_1,// 9
        sw1_pressed_2,// 10
        sw2_clicked,// 11
        sw2_pressed_0,// 12
        sw2_pressed_1,// 13
        sw2_pressed_2,// 14
        input_event_last// 15
    };

    void init();
    uint16_t update(uint8_t input);
  private:
    uint16_t processSwitch(int sw);
    uint16_t processEncoder();
    uint16_t process();
};

