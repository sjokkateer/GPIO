#pragma once
#include "avr/io.h"

class Button
{
    private:
        volatile uint8_t *ddr;
        volatile uint8_t *port;
        volatile uint8_t *pin;

        uint8_t pinNumber;

        void setRegistersFor(char);
    public:
        Button(char, uint8_t);
        void inbuiltResistor();
        bool isPressed();
};