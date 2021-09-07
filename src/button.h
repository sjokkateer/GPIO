#pragma once
#include "avr/io.h"
#include "led.h"
#include "button-state.h"

class Button
{
    private:
        volatile uint8_t *ddr;
        volatile uint8_t *pin;
        ButtonState state;
        
        uint8_t pinNumber;
        Led *led;

        void setRegistersFor(char);
    public:
        volatile unsigned long pressTime;
        unsigned long releaseTime;
        
        Button(char, uint8_t);
        void determineState();
        void setPressTime(unsigned long);
        void setReleaseTime(unsigned long);
        ButtonState getState();
        void reset();
        bool isPressed();
        void controls(Led*);
};
