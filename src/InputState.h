#pragma once
#include <Arduino.h>
#include "Utils.h"
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

    Event update(State newState)
    {
        Event event{};
        if (state != newState) {
            state = newState;
            if (state == pressed) {
                millisWhenPressed = millis();
                event = pressed_0;
            } else {
                unsigned long now = millis();
                unsigned long diff = now - millisWhenPressed;
                if (diff < CLICK_LIMIT) {
                    event = clicked;
                } else if (diff < PRESSED_1_LIMIT) {
                    event = pressed_1;
                } else {
                    event = pressed_2;
                }
            }
        }
        return event;
    }
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

    void init()
    {
        lastState = currState = readInput();
    }

    uint16_t update()
    {
        lastState = currState;
        int cnt = 0;
        uint8_t input = readInput();
        do {
            // delay(2);
            uint8_t tmp = readInput();
            if (tmp == input)
                cnt++;
            else
                cnt = 0;
            input = tmp;
        } while (cnt < 10);
        currState = input;
#if 0
        c_printf("currState: %02x", (unsigned)currState);
#endif
        return process();
    }

  private:
    uint16_t processSwitch(int sw)
    {
        static const byte bits[]{InputBit::sw0, InputBit::sw1, InputBit::sw2};
        static const byte offsets[] = {sw0_clicked, sw1_clicked, sw2_clicked};
        uint16_t events = 0;

        // c_printf("last: %04x curr: %04x", lastState, currState);

        if (DIFF_BIT(lastState, currState, bits[sw])) {
            SwitchState::State swState = GET_BIT(currState, bits[sw]) ? SwitchState::pressed : SwitchState::depressed;
            SwitchState::Event swEvent = switches[sw].update(swState);

            if (swEvent != SwitchState::Event::none) {
                events = 1 << (static_cast<uint16_t>(swEvent) - 1 + offsets[sw]);
            }
            /*
            c_printf("sw: %d: swEvt: %02x inEvt: %04x", sw, (unsigned)swEvent, events);
            */
        }
        return events;
    }
    uint16_t processEncoder()
    {
        uint16_t event = 0;
        if (DIFF_BIT(lastState, currState, InputBit::clk) && GET_BIT(currState, InputBit::clk)) {
#if 0
            c_print("clk: ");
            c_print(GET_BIT(currState, InputBit::clk));
            c_print(" dt: ");
            c_println(GET_BIT(currState, InputBit::dt));
#endif
            if (GET_BIT(currState, InputBit::clk) != GET_BIT(currState, InputBit::dt)) {
                event = 1 << encoder_left;
                // c_println("enc: left");
            } else {
                event = 1 << encoder_right;
                // c_println("enc: right");
            }
        } else {
            // c_println("no encoder");
        }
        return event;
    }

    uint16_t process()
    {
        uint16_t events = 0;
        events |= processEncoder();
        events |= processSwitch(0);
        events |= processSwitch(1);
        events |= processSwitch(2);
        return events;
    }
};

