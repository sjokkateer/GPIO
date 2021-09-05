#pragma once
#include "stdint.h"

class Led
{
private:
    volatile uint8_t *ddr;
    volatile uint8_t *port;

    uint8_t pinNumber;

    void setRegistersFor(char);
public:
    Led(char, uint8_t);
    void toggle();
    void on();
    void off();
};
