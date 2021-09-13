#include <Arduino.h>
#include "led.h"
#include "button.h"

const char *buttonStateMapping[] = {"NO_PRESS", "SHORT_PRESS", "LONG_PRESS"};
const int NUMBER_OF_BUTTONS = 2;

volatile unsigned long time;

volatile bool interrupted = false;

// D7 and D13
Led leds[] = {Led('C', 5), Led('C', 4)};
Button buttons[] = {Button('B', 0), Button('D', 2)};

void actOnButton(Button *);

void setup()
{
  Serial.begin(9600);

  buttons[0].controls(&leds[0]);
  buttons[1].controls(&leds[1]);

  // Set up the interrupt registers.
  sei();

  // activate interrupts for pins 0 to 7
  PCICR |= (1 << 0);
  // // activate pcint0 aka PB0 (D8)
  PCMSK0 |= (1 << 0);

  // Another one.
  // activate interrupts for pins 16 to 23 (PCIE2)
  PCICR |= (1 << 2);
  // activate pcint18 aka PD2 (D2)
  PCMSK2 |= (1 << 2);
}

ISR(PCINT0_vect)
{
  actOnButton(&buttons[0]);
}

ISR(PCINT2_vect)
{
  actOnButton(&buttons[1]);
}

void actOnButton(Button *button)
{
  time = millis();

  if (button->isPressed())
  {
    button->setPressTime(time);
  }
  else
  {
    button->setReleaseTime(time);
    button->determineState();
  }
}

void loop()
{
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    if (buttons[i].getState() != NO_PRESS)
    {
      Serial.print("BUTTON PRESS: ");
      Serial.println(buttonStateMapping[buttons[i].getState()]);

      buttons[i].getLed()->updateState(buttons[i].getState());

      buttons[i].reset();
    }

    buttons[i].getLed()->act();
  }
}
