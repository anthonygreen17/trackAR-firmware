#include "hc12.h"
#include "TinyGPS++.h"
#include "trackAR_gps.h"
#include "general_config.h"
#include "trackAR_sleep.h"
#include "avr/wdt.h"

char gps_message[150];
USART_WAKE_RX usart_wake = SLEEP_UNTIL_USART_2;

void setup()
{ 
  disableUnneededPeriphs(BEACON);
  gps::boot();  
  hc12::initialize();
  UserSerial.begin(115200);
  GPS_SERIAL.begin(gps::BAUD);
  if (!gps::sync())
    UserSerial.println("Problem syncing GPS!");
  else
    UserSerial.println("GPS properly synced");
  UserSerial.flush();
}

/**
 *  The call to gps::sync() should return immediately as a transmission is completed. So, when we
 *  call sleepUntilUartRx(), that means we should sleep for almost an entire second, until the 
 *  very first byte of the next transmission is received. At that point, we need to bail out of
 *  sleep mode and process all of the bytes of the transmission using gps::smartDelay().
 *  Then, simply format and send out the latest received GPS data.
 *  
 *  In order to save power, we're going to leverage a WDT interrupt. This allows use to go into the deepest
 *  sleep mode, SLEEP_MODE_PWR_DOWN, which saves a LOT more power than the SLEEP_MODE_IDLE that is needed
 *  to wake up via UART RX. The time between the first RX byte from the GPS and the last TX byte sent 
 *  out to the transceiver has been found to be ~400ms. To be safe, we'll use the WDT to sleep for 
 *  around 500ms before going into SLEEP_MODE_IDLE to let the GPS wake us up.
 */
void loop()
{
  setupWdtInterrupt(MS_500);
  sleepPwrDown(BEACON);
  disableWdt();
  /* hc12::exitSleep(); */
  sleepUntilUartRX(usart_wake, BEACON);

  /**
   * The typical time from this point to the end of the loop was 
   * found to be ~400ms
   */
  if (!gps::smartDelay(1500))
  {
    UserSerial.println("Problem syncing GPS. Resyncing...");
    gps::sync();
  }
  gps::formatInto(gps_message);
  hc12::send(gps_message);
  HC12_SERIAL.flush();
  /* hc12::sleep() */
}


ISR(WDT_vect)
{

}


