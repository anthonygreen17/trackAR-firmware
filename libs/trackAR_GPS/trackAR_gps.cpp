#include "trackAR_gps.h"
#include "general_config.h"

TinyGPSPlus tinyGps;

namespace gps {

void boot()
{
  // Init the GPS Module to wake mode
  pinMode(SYSONPin, INPUT);
  pinMode(ONOFFPin, OUTPUT);
  digitalWrite(ONOFFPin, LOW);   
  delay(5); 
  if( digitalRead(SYSONPin) == LOW )
  {
     // Need to wake the module
    digitalWrite(ONOFFPin, HIGH); 
    delay(5); 
    digitalWrite(ONOFFPin, LOW);      
  } 
}

void formatInto(char* gps_message, bool user_serial_debug)
{
  String latitude = String(tinyGps.location.lat(), 6);
  String longitude = String(tinyGps.location.lng(), 6);
  String date = String(tinyGps.date.value());
  String time = String(tinyGps.time.value());
  String speed = String(tinyGps.speed.mph());
  String course = String(tinyGps.course.deg());
  String altitude = String(tinyGps.altitude.miles());
  String satellites = String(tinyGps.satellites.value());
  String hdop = String(tinyGps.hdop.value());
  
  sprintf(gps_message, "%f,%s,%s,%s,%s,%s,%s,%s,%s", 
    tinyGps.location.lat(), 
    longitude.c_str(), 
    date.c_str(), 
    time.c_str(),
    speed.c_str(),
    course.c_str(),
    altitude.c_str(),
    satellites.c_str(),
    hdop.c_str());

  if (user_serial_debug)
  {
    UserSerial.print("GPS message..."); UserSerial.println(gps_message);
  }
}

void serializeInto(uint8_t buf[HC12_TRANSMIT_SIZE], bool user_serial_debug)
{
  gps_vals_union vals;
  vals.vals.latitude =  (float)tinyGps.location.lat();
  vals.vals.longitude = (float)tinyGps.location.lng();
  vals.vals.altitude =  (float)tinyGps.altitude.miles();
  memcpy(buf, vals.data_bytes, sizeof(gps_vals_t));

  if (!user_serial_debug)
    return;

  String latitude = String(tinyGps.location.lat(), 6);
  String longitude = String(tinyGps.location.lng(), 6);
  String altitude = String(tinyGps.altitude.miles(), 6);
    char msg[80];
    sprintf(msg, "%s,%s,%s", 
      latitude.c_str(),
      longitude.c_str(),
      altitude.c_str()
    );
    UserSerial.print("GPS message ||"); UserSerial.println(msg);
}

/**
 *  A "full message" consists of 6 NMEA strings delimited by "\r\n"
 */
static bool receiveFullMsg(unsigned int max_wait_ms)
{
  int numSentences = 0;
  unsigned long start = millis();

  /**
   *  0 = default state, 1 = received "\r"
   *  Used to detect '\r' directly followed by '\n'
   */
  unsigned int state = 0; 
  do 
  {
    while (GPS_SERIAL.available() > 0)
    {
      char c = GPS_SERIAL.read();
      if ( (c == '\n') && state == 1)
      {
        state = 0;
        if (++numSentences == SENTENCES_PER_TRANSMISSION)
          return true;
      }
      else if (c == '\r')
      {
        state++;
      }
      else
      {
        state = 0;
      }
    tinyGps.encode(c);
    }
  } while ((millis() - start < max_wait_ms));
  return false;
}

/**
 *  Try to find the end of the transmission within "ms" seconds. Return true if found.
 */
static bool findTransmissionEnd(unsigned int ms)
{
  unsigned int lastRxChar, start = millis();
  do
  {
    while (GPS_SERIAL.available() > 0) 
    {
      lastRxChar = millis();
    }
    if ( (millis() - lastRxChar) > ((float)RATE_MS / 10.0))
    {
      return true;
    }
  } while ((millis() - start < ms));
  return false;
}

bool smartDelay(unsigned int ms)
{
  return receiveFullMsg(ms);
}

bool sync(unsigned int max_wait_ms)
{
  if (findTransmissionEnd(RATE_MS * 2))  // max 2 iterations to find transmission end
    return receiveFullMsg(max_wait_ms);
  else
    UserSerial.println("Couldnt find the end of the GPS transmission!");
}

} // gps
