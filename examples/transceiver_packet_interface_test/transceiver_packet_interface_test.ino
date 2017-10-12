/**
 * Test file for sending and receiving trasnceiver messages with the packet interface.
 */

#include "hc12.h"

#define SEND_RATE_MS 1000

void setup() {
  // put your setup code here, to run once:
  hc12::initialize();
  Serial.begin(9600);
  unsigned long last_write = 0;
}

void loop() {
  // when a message is received, a message should be printed to the serial console
  unsigned long now = millis();
  hc12::processBytes();

  if ((now - SEND_RATE_MS) > last_write)
    hc12::send("hello there fagboi"); 
}
