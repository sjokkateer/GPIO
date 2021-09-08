#include <Arduino.h>
#include "led.h"
#include "button.h"

const char* buttonStateMapping[] = { "NO_PRESS", "SHORT_PRESS", "LONG_PRESS" };

volatile unsigned long time;

Led led('D', 7);
Button button('B', 0);

void setup() {
  Serial.begin(9600);

  button.controls(&led);

  // Set up the interrupt registers.
  sei();

  // Button one setup. //
  // activate interrupts for pins 0 to 7
  PCICR |= (1 << 0);
  // activate pcint0 aka PB0
  PCMSK0 |= (1 << 0);
}

ISR(PCINT0_vect)
{
  time = millis();

  if (button.isPressed())
  {
    button.setPressTime(time);
  }
  else
  {
    button.setReleaseTime(time);
    button.determineState();
  }
}

void loop() {
  if (button.getState() != NO_PRESS)
  {
    Serial.print("BUTTON PRESS: ");
    Serial.println(buttonStateMapping[button.getState()]);

    button.getLed()->updateState(button.getState());

    button.reset();
  }

  button.getLed()->act();
}
