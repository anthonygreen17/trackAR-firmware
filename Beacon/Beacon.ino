#include "hc12.h"
#include "TinyGPS++.h"
#include "trackAR_gps.h"
#include "general_config.h"

char gps_message[150];

void setup()
{  
  gps::boot();
  GPS_SERIAL.begin(gps::BAUD);  
  hc12::initialize();
  UserSerial.begin(9600);     
}

void loop()
{ 
  gps::formatMessage(gps_message);
  hc12::send(gps_message);
  smartdelay(1000);
}

/**
 *  Instead of sleeping, continually loop for "ms" milliseconds, checking for data on the GPS_SERIAL line.
 */
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SERIAL.available() > 0)
      tinyGps.encode(GPS_SERIAL.read());
  } while (millis() - start < ms);
}
