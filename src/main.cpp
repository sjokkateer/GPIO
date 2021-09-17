#include <Arduino.h>
#include "led.h"
#include "button.h"

const char *buttonStateMapping[] = {"NO_PRESS", "SHORT_PRESS", "LONG_PRESS"};

// Important to update when a button and led get added
const int NUMBER_OF_BUTTONS = 3;
Led leds[] = {Led('C', 5), Led('C', 4), Led('C', 3)};
Button buttons[] = {Button('B', 0), Button('B', 1), Button('B', 2)};

void setup()
{
  Serial.begin(9600);

  // Enable interrupts
  sei();

  // activate interrupts for pins 0 to 7 (PCINT0..7)
  PCICR |= (1 << 0);

  // Assign leds to the buttons controlling them.
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    buttons[i].controls(&leds[i]);
    
    // For simplicity, all pins in one vector such that we
    // can dynamically enable pin change interrupts through 
    // a loop for all corresponding pins.
    PCMSK0 |= (1 << i);
  }
}

ISR(PCINT0_vect)
{
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    if (buttons[i].isUnchanged()) continue;

    buttons[i].setPreviousPinValue();

    if (buttons[i].isPressed())
    {
      buttons[i].setPressTime(millis());
    }
    else
    {
      buttons[i].setReleaseTime(millis());
      buttons[i].determineState();
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
