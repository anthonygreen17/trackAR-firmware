#ifndef TRACKAR_GPS_H_
#define TRACKAR_GPS_H_

#include "TinyGPS++.h"

extern TinyGPSPlus tinyGps;

namespace gps {

static const int ONOFFPin = A3;
static const int SYSONPin = A2;
static const int BAUD = 9600;
static const int RATE_MS = 1000;  // default
static const int SENTENCES_PER_TRANSMISSION = 6;

/**
 *  Initialize the GPS module to command mode.
 */
void boot();

/**
 *  Format the latest received GPS data into the message.
 */
void formatInto(char* message, bool user_serial_debug = true);

/**
 *  The Tinyshield GPS sends 6 NMEA sentences per transmission. To start off our program, 
 *  we should receive a full transmission, send it to the Receiver, and sleep IMMEDIATELY.
 *  This will be the most power-efficient since sleep mode will be entered for almost the entire
 *  duration defined by gps::RATE_MS.
 *
 *  Return true if this process was successful.
 */
bool sync(unsigned int max_wait_ms = 5000);

/**
 *  Wait for the desired amount of time by looping and reading data as it comes in 
 *  from the GPS. Do NOT call delay().
 */
void smartDelay(unsigned int ms);

} //gps

#endif
