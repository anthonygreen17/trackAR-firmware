#include "hc12.h"
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
  Serial1.begin(9600);  
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

  Serial.print("Sending... "); Serial.println(message);
  Serial.println();

  Serial1.println(message);
  
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


