#include "button.h"
#include "button-state.h"

Button::Button(char port, uint8_t pinNumber)
{
    this->setRegistersFor(port);
    this->pinNumber = pinNumber;

    *this->ddr &= ~(1 << this->pinNumber);

    this->initialPinValue = this->getPinBitValue();
    this->previousPinvalue = this->initialPinValue;

    this->pressTime = 0;
    this->releaseTime = 0;

    this->determineState();
}

uint8_t Button::getPinBitValue()
{
    return (*this->pin & (1 << this->pinNumber)) >> this->pinNumber;
}

void Button::setRegistersFor(char port)
{
    switch(port)
    {
        case 'B':
            this->ddr = &DDRB;
            this->pin = &PINB;
            break;
        case 'C':
            this->ddr = &DDRC;
            this->pin = &PINC;
            break;
        case 'D':
            this->ddr = &DDRD;
            this->pin = &PIND;
            break;
    }
}

void Button::determineState()
{
    unsigned long timePassed = this->releaseTime - this->pressTime;

    if (timePassed <= 10)
    {
        this->state = NO_PRESS;
    }
    else if (timePassed <= 500)
    {
        this->state = SHORT_PRESS;
    }
    else
    {
        this->state = LONG_PRESS;
    }
}

void Button::setPressTime(unsigned long pressTime)
{
    this->pressTime = pressTime;
}

void Button::setReleaseTime(unsigned long releaseTime)
{
    this->releaseTime = releaseTime;
}

void Button::reset()
{
    this->pressTime = 0;
    this->releaseTime = 0;

    this->state = NO_PRESS;
}

ButtonState Button::getState()
{
    return this->state;
}

bool Button::isUnchanged()
{
    return this->getPinBitValue() == this->previousPinvalue;
}

bool Button::isPressed()
{
    // So if pullup, initial was 1 thus 1 -> 0 is pressed
    // alternatively pulldown, initial was 0 thus 0 -> 1 is pressed
    // if same bit value as initial it's in neutral/not pressed state.
    return this->getPinBitValue() != this->initialPinValue;
}

void Button::setPreviousPinValue()
{
    this->previousPinvalue = this->getPinBitValue();
}

void Button::controls(Led *led)
{
    this->led = led;
}

Led* Button::getLed()
{
    return this->led;
}
