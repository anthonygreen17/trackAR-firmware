//-------------------------------------------------------------------------------
//  TinyCircuits GPS TinyShield Display Example
//  Last updated 7 April 2016
//  
//  This example uses the TinyGPS library, originally written by Mikal Hart and 
//  currently forked and updated at https://github.com/florind/TinyGPS to support
//  Glonass, slightly modified to work with the GPS TinyShield.
//
//  Some GPS modules have been shipped with 4800 baud instead of 9600- try this if
//  you see bad data.
//
//  TinyCircuits, http://TinyCircuits.com
//
//-------------------------------------------------------------------------------

//#include "hc12.h"
#include "TinyGPS++.h"

static const int GPS_ONOFFPin = A3;
static const int GPS_SYSONPin = A2;
static const int GPSBaud = 9600;

TinyGPSPlus gps;

char message[100];

void setup()
{  
  bootGPS();
  
  Serial.begin(9600);      
  Serial2.begin(GPSBaud);
}

void loop()
{ 
  String latitude = String(gps.location.lat(), 6);
  String longitude = String(gps.location.lng(), 6);
  String date = String(gps.date.value());
  String time = String(gps.time.value());
  String speed = String(gps.speed.mph());
  String course = String(gps.course.deg());
  String altitude = String(gps.altitude.miles());
  String satellites = String(gps.satellites.value());
  String hdop = String(gps.hdop.value());
  
  sprintf(message, "%s,%s,%s,%s,%s,%s,%s,%s,%s", 
    latitude.c_str(), 
    longitude.c_str(), 
    date.c_str(), 
    time.c_str(),
    speed.c_str(),
    course.c_str(),
    altitude.c_str(),
    satellites.c_str(),
    hdop.c_str());

  Serial.print("String To Send: ------- "); Serial.println(message);

  Serial.print("Longitude (degrees): -- "); Serial.println(latitude); // Latitude in degrees (double)
  Serial.print("Latitude (degrees): --- "); Serial.println(longitude); // Longitude in degrees (double)
  Serial.print("Date: ----------------- "); Serial.println(date); // Raw date in DDMMYY format (u32)
  Serial.print("Time: ----------------- "); Serial.println(time); // Raw time in HHMMSSCC format (u32)
  Serial.print("Speed (mph): ---------- "); Serial.println(speed); // Speed in miles per hour (double)
  Serial.print("Course (degrees): ----- "); Serial.println(course); // Course in degrees (double)
  Serial.print("Altitude (miles): ----- "); Serial.println(altitude); // Altitude in miles (double)
  Serial.print("Number Of Satellites: - "); Serial.println(satellites); // Number of satellites in use (u32)
  Serial.print("HDOP: ----------------- "); Serial.println(hdop); // Horizontal Dim. of Precision (100ths-i32)
  
  Serial.println();
  
  smartdelay(1000);
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial2.available() > 0)
      gps.encode(Serial2.read());
  } while (millis() - start < ms);
}

void bootGPS()
{
  // Init the GPS Module to wake mode
  pinMode(GPS_SYSONPin, INPUT);
  pinMode(GPS_ONOFFPin, OUTPUT);
  digitalWrite( GPS_ONOFFPin, LOW );   
  delay(5); 
  if( digitalRead( GPS_SYSONPin ) == LOW )
  {
     // Need to wake the module
    digitalWrite( GPS_ONOFFPin, HIGH ); 
    delay(5); 
    digitalWrite( GPS_ONOFFPin, LOW );      
  } 
}


