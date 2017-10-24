#include "hc12.h"
#include "TinyGPS++.h"
#include "trackAR_gps.h"
#include "general_config.h"
#include "trackAR_sleep.h"

char gps_message[150];

void setup()
{  
  gps::boot();  
  hc12::initialize();
  UserSerial.begin(115200);
  GPS_SERIAL.begin(gps::BAUD);
  smartdelay(10000);
  UserSerial.println("received 6 sentences..");
}

void loop()
{
  sleepUntilUartRX();
  smartdelay(10000);
  gps::formatMessage(gps_message);
  hc12::send(gps_message);
  UserSerial.println("received 6 sentences..");
  delay(25);
}

/**
 *  Instead of sleeping, continually loop for "ms" milliseconds, checking for data on the GPS_SERIAL line.
 */
static void smartdelay(unsigned long ms)
{
  int numSentences = 0;
  unsigned long start = millis();
  do 
  {
//    UserSerial.println("no bytes");
    while (GPS_SERIAL.available() > 0)
    {
      char c = GPS_SERIAL.read();
      if (c == '\n')
      {
        UserSerial.println("NL---");
        numSentences++;
      }
      else if (c == '\r')
      {
        UserSerial.println("CR-----");
        
      }
      else if (c == '$')
        UserSerial.println("MESSAGE START");
//      else
//        UserSerial.println(c);
    tinyGps.encode(c);
    }
  } while ((millis() - start < ms) && numSentences < 6);
}
