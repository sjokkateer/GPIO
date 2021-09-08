#include "led.h"
#include "avr/io.h"
#include "util/delay.h"

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
    *this->port &= ~(1 << this->pinNumber);

    this->state = OFF;
}

void Led::updateState(ButtonState buttonState)
{
    switch (buttonState)
    {
    case SHORT_PRESS:
        this->state = (this->state != OFF) ? OFF : ON;
        break;
    case LONG_PRESS:
        this->state = BLINK;
    default:
        break;
    }
}

void Led::act()
{
    switch(this->state)
    {
        case OFF:
            this->off();
            break;
        case ON:
            this->on();
            break;
        case BLINK:
            this->blink();
            this->blink();
            break;
    }
}

void Led::on()
{
    *this->port |= (1 << this->pinNumber);
}

void Led::off()
{
    *this->port &= ~(1 << this->pinNumber);
}

void Led::blink()
{
    this->on();
    _delay_ms(250);
    this->off();
    _delay_ms(250);
}
