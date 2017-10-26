#include "hc12.h"
#include "TinyGPS++.h"
#include "trackAR_gps.h"
#include "general_config.h"
#include "trackAR_sleep.h"

char gps_message[150];

USART_WAKE_RX usart_wake = SLEEP_UNTIL_USART_2;

void setup()
{  
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
 *
 *  Then, simply format and send out the latest received GPS data.
 */
void loop()
{
  sleepUntilUartRX(usart_wake);
  if (!gps::smartDelay(1500))
  {
    UserSerial.println("Problem syncing GPS. Resyncing...");
    gps::sync();
  }
  gps::formatInto(gps_message);
  hc12::send(gps_message);
  HC12_SERIAL.flush();
}


