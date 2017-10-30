#include "pin_change_interrupt.h"

/**
 * Demonstrate a pin change interrupt on pin 2 (labeled "2" on the board). The indicator LED
 * on the board will change state every time you apply a logic HIGH voltage to the pin.
 * 
 * Setup: MAKE SURE YOU USE A PULLDOWN RESISTOR TO GROUND FOR PIN 2.
 */

#define LED     13
#define ISR_PIN  2
#define USE_RISING_EDGE  1
#define TRIGGERED_DELAY 100  // wait 200ms after checking ISR state to avoid debounce

int counter = 0;
unsigned long int lastInterrupt = 0;

void setup()
{
  Serial.begin(115200);

  // disable interrupts until we are done configuring interrupts
  noInterrupts();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  setupPinChangeInterrupt(USE_RISING_EDGE);
  interrupts();
  Serial.println("starting up...");
}

void loop()
{
  if (ISR_PIN_STATE)
  {
    delay(TRIGGERED_DELAY);
    ISR_PIN_STATE = false;
    if (digitalRead(ISR_PIN))
    {
      digitalWrite(LED, !digitalRead(LED));
      Serial.println(++counter);
    }
  }
}

/**
 * This is just here in case we want to do bitwise things later.
void setupInterruptBitwise()
{
  PCICR |= 0b00000011; // Enables PB* pin change interrupts
  PCMSK0 |= 0b00000001; // PCINT0 - PB0 -> pin 53
}
*/
