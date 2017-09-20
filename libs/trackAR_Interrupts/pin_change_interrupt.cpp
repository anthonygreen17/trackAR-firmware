#include "Arduino.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "pin_change_interrupt.h"

// forward declare ISR so we can use it in functions below
void pinChangeISR();

// keep track of state
bool configured = false;
volatile bool ISR_PIN_STATE = false;

void disablePinChangeInterrupt()
{
  detachInterrupt(digitalPinToInterrupt(ISR_PIN));
}

void setupPinChangeInterrupt(bool rising)
{
	if (!configured)
	{
		pinMode(ISR_PIN, INPUT);
		configured = true;
	}
  attachInterrupt(
    digitalPinToInterrupt(ISR_PIN), pinChangeISR, rising ? RISING : FALLING
   );
}

void pinChangeISR()
{
	ISR_PIN_STATE = true;
}
