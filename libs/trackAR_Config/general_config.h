#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

typedef enum TrackARDevice : uint8_t {
	BEACON,
	RECEIVER,
} TrackARDevice;


#define UserSerial  Serial 
#define HC12_SERIAL Serial1

// bluetooth and gps will use same serial, since GPS is only on Beacon and bluetooth is only on receiver
#define GPS_SERIAL  Serial2
#define BT_SERIAL   Serial2
#define TRANSCEIVER_SET A1


#endif // CONFG_H_