#pragma once
#include "avr/io.h"
#include "led.h"
#include "button-state.h"

class Button
{
    private:
        volatile uint8_t *ddr;
        volatile uint8_t *pin;
        uint8_t initialPinValue;
        uint8_t previousPinvalue;
        ButtonState state;
        uint8_t pinNumber;
        Led *led;
        unsigned long pressTime;
        unsigned long releaseTime;
        
        void setRegistersFor(char);
        uint8_t getPinBitValue();
    public:
        Button(char, uint8_t);
        void controls(Led*);
        bool isUnchanged();
        void setPressTime(unsigned long);
        void setReleaseTime(unsigned long);
        void determineState();
        void setPreviousPinValue();
        ButtonState getState();
        void reset();
        bool isPressed();
        Led* getLed();
};
