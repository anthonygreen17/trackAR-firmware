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

void formatMessage(char* gps_message)
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
  
  sprintf(gps_message, "%s,%s,%s,%s,%s,%s,%s,%s,%s", 
    latitude.c_str(), 
    longitude.c_str(), 
    date.c_str(), 
    time.c_str(),
    speed.c_str(),
    course.c_str(),
    altitude.c_str(),
    satellites.c_str(),
    hdop.c_str());

  UserSerial.print("GPS message..."); UserSerial.println(gps_message);
  UserSerial.println();
}




} // gps