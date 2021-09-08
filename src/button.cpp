#include "button.h"
#include "button-state.h"

Button::Button(char port, uint8_t pinNumber)
{
    this->setRegistersFor(port);
    this->pinNumber = pinNumber;

    *this->ddr &= ~(1 << this->pinNumber);

    this->pressTime = 0;
    this->releaseTime = 0;

    this->determineState();
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

bool Button::isPressed()
{
    return (*this->pin & (1 << this->pinNumber)) == 0;
}

void Button::controls(Led *led)
{
    this->led = led;
}

Led* Button::getLed()
{
    return this->led;
}
