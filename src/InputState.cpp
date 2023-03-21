#include "InputState.h"

#define CLICK_LIMIT 1000
#define PRESSED_1_LIMIT 2000

SwitchState::Event SwitchState::update(State newState)
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
            if (diff < 5) {
                c_info("switch event: ignored");
            } else if (diff < CLICK_LIMIT) {
                event = clicked;
                c_info("switch event clicked");
            } else if (diff < PRESSED_1_LIMIT) {
                event = pressed_1;
                c_info("switch event pressed_1");
            } else {
                event = pressed_2;
                c_info("switch event pressed_2");
            }
        }
    }
    return event;
}

void InputState::init()
{
    lastState = currState = readInput();
}

uint16_t InputState::update(uint8_t input)
{
    lastState = currState;
    c_info("input: %02x", (unsigned)input);
    currState = input;
    c_debug("currState: %02x", (unsigned)currState);
    return process();
}

uint16_t InputState::processSwitch(int sw)
{
    static const byte bits[]{InputBit::sw0, InputBit::sw1, InputBit::sw2};
    static const byte offsets[] = {sw0_clicked, sw1_clicked, sw2_clicked};
    uint16_t events = 0;

    c_info("last: %02x curr: %02x bits: %02x", lastState, currState, (1 << bits[sw]));

    if (DIFF_BIT(lastState, currState, bits[sw])) {
        SwitchState::State swState = GET_BIT(currState, bits[sw]) ? SwitchState::pressed : SwitchState::depressed;
        SwitchState::Event swEvent = switches[sw].update(swState);

        if (swEvent != SwitchState::Event::none) {
            events = 1 << (static_cast<uint16_t>(swEvent) - 1 + offsets[sw]);
            c_info("sw: %d: swEvt: %02x inEvt: %04x", sw, (unsigned)swEvent, events);
        }
    }
    if (events) {
        c_info("events: %04x", events);
    }
    return events;
}
uint16_t InputState::processEncoder()
{
    uint16_t event = 0;
    if (DIFF_BIT(lastState, currState, InputBit::clk) && GET_BIT(currState, InputBit::clk)) {
        c_debug("clk: %d dt: %d", GET_BIT(currState, InputBit::clk), GET_BIT(currState, InputBit::dt));
        if (GET_BIT(currState, InputBit::clk) != GET_BIT(currState, InputBit::dt)) {
            event = 1 << encoder_left;
            c_info("enc: left");
        } else {
            event = 1 << encoder_right;
            c_info("enc: right");
        }
    } else {
        c_debug("no encoder");
    }
    return event;
}

uint16_t InputState::process()
{
    uint16_t events = 0;
    events |= processEncoder();
    events |= processSwitch(0);
    events |= processSwitch(1);
    events |= processSwitch(2);
    return events;
}
