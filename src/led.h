#pragma once
#include "stdint.h"
#include "led-state.h"
#include "button-state.h"

class Led
{
private:
    volatile uint8_t *ddr;
    volatile uint8_t *port;

    uint8_t pinNumber;
    LedState state;

    void setRegistersFor(char);

    void on();
    void off();
    void blink();
    void blinkTwice();
public:
    Led(char, uint8_t);
    void updateState(ButtonState);
    void act();
};
