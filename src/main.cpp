#include <Arduino.h>
#include "led.h"
#include "button.h"

const char *buttonStateMapping[] = {"NO_PRESS", "SHORT_PRESS", "LONG_PRESS"};
const int NUMBER_OF_BUTTONS = 2;

volatile unsigned long time;

volatile bool interrupted = false;

Led leds[] = {Led('C', 5), Led('C', 4)};
Button buttons[] = {Button('B', 0), Button('B', 1)};
int previousPinValue[] = {1, 1};

void actOnButton(Button *);

void setup()
{
  Serial.begin(9600);

  // Assign leds to the buttons controlling them.
  buttons[0].controls(&leds[0]);
  buttons[1].controls(&leds[1]);

  // Enable interrupts
  sei();

  // activate interrupts for pins 0 to 7 (PCINT0..7)
  PCICR |= (1 << 0);
  // // activate pcint1 PB1 (D9) and pcint0 PB0 (D8)
  PCMSK0 |= (1 << 1) | (1 << 0);
}

volatile bool test = false;

ISR(PCINT0_vect)
{
  test = true;

  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    if (buttons[i].getPinValue() != previousPinValue[i])
    {
      if (buttons[i].isPressed())
      {
        buttons[i].setPressTime(millis());
   
        previousPinValue[i] = 0;
      }
      else
      {
        buttons[i].setReleaseTime(millis());
        buttons[i].determineState();

        previousPinValue[i] = 1;
      }
    }
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
