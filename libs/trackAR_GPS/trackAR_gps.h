#ifndef TRACKAR_GPS_H_
#define TRACKAR_GPS_H_

#include "TinyGPS++.h"

extern TinyGPSPlus tinyGps;

namespace gps {


static const int ONOFFPin = A3;
static const int SYSONPin = A2;
static const int BAUD = 9600;

/**
 *  Initialize the GPS module to command mode.
 */
void boot();

/**
 *  Format the latest received GPS data into the message.
 */
void formatMessage(char* message);

} //gps

#endif
