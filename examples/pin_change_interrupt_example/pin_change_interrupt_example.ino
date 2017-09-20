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
#define TRIGGERED_DELAY 500  // wait 500ms between ISRs to avoid debouncing

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  // disable interrupts until we are done configuring interrupts
  noInterrupts();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  setupPinChangeInterrupt(USE_RISING_EDGE);
  interrupts();
}

void loop()
{
  if (ISR_PIN_STATE)
  {
    disablePinChangeInterrupt();
    digitalWrite(LED, !digitalRead(LED));
    Serial.println("suhh");
    ISR_PIN_STATE = false;
    delay(TRIGGERED_DELAY);
    Serial.println("---");
    setupPinChangeInterrupt(USE_RISING_EDGE);
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
