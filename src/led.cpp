#include "led.h"
#include "avr/io.h"

void Led::setRegistersFor(char port)
{
    switch(port)
    {
        case 'B':
            this->ddr = &DDRB;
            this->port = &PORTB;
            break;
        case 'C':
            this->ddr = &DDRC;
            this->port = &PORTC;
            break;
        case 'D':
            this->ddr = &DDRD;
            this->port = &PORTD;
            break;
    }
}

Led::Led(char port, uint8_t pinNumber)
{
    this->setRegistersFor(port);
    this->pinNumber = pinNumber;

    // set pin as output
    *this->ddr |= (1 << this->pinNumber);
    this->off();
}

void Led::on()
{ 
    *this->port |= (1 << this->pinNumber);
}

void Led::off()
{
    *this->port &= ~(1 << this->pinNumber);
}

void Led::toggle()
{
    *this->port ^= (1 << this->pinNumber);
}
