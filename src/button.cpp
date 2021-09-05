#include "button.h"

void Button::setRegistersFor(char port)
{
    switch(port)
    {
        case 'B':
            this->ddr = &DDRB;
            this->port = &PORTB;
            this->pin = &PINB;
            break;
        case 'C':
            this->ddr = &DDRC;
            this->port = &PORTC;
            this->pin = &PINC;
            break;
        case 'D':
            this->ddr = &DDRD;
            this->port = &PORTD;
            this->pin = &PIND;
            break;
    }
}

Button::Button(char port, uint8_t pinNumber)
{
    this->setRegistersFor(port);
    this->pinNumber = pinNumber;

    *this->ddr &= ~(1 << this->pinNumber);
}

void Button::inbuiltResistor()
{
    *this->port |= (1 << this->pinNumber);
}

bool Button::isPressed()
{
    return (*this->pin & (1 << this->pinNumber)) == 0;
}
