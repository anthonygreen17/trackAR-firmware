#include "trackAR_sleep.h"
#include "general_config.h"

volatile int wdt_flag = 0;
/**
 * Sleep for 1 second 10 times, then sleep forever.
 */
int sleeps = 0;
#define NUM_SLEEPS 10
#define SLEEP_LENGTH SEC_1

ISR(WDT_vect)
{
  wdt_flag = 1;
}

void setup() {
  
  setupWdtInterrupt(SLEEP_LENGTH);
  UserSerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  UserSerial.flush();
  sleepPwrDown(BEACON);
  if (wdt_flag)
  {
    UserSerial.println("WDT triggered!");
    wdt_flag = 0;
    if ((++sleeps) == NUM_SLEEPS)
    {
      UserSerial.println("Sleeping forever pce");
      UserSerial.flush();
      disableWdt();
    }
  }
}
