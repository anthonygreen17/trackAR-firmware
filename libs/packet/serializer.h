#ifndef TRACKAR_SERIALIZER_H_
#define TRACKAR_SERIALIZER_H_
#include "inttypes.h"

/**

 example GPS string transmitted:

size: 58 bytes
GPS message...42.339012,-71.088715,21117,20151900,0.00,224.48,0.02,5,180

index range      type
0-3              float        (latitude          
4-7              float        (longitude) 
8-12             unsigned long (date)
13-17            unsigned long (time)
18-21            float        (speed)
22-25            float        (course)
26-29            float        (altitude)
30-31            unsigned int (satellites)
32-33            unsigned int (hdop)
*/

/**
 *  These are the fields that we actually need from the GPS.
 */
typedef struct {
	float latitude;
	float longitude;
	float altitude;
} gps_vals_t;

typedef union {
	gps_vals_t vals;
	uint8_t data_bytes[sizeof(gps_vals_t)];
} gps_vals_union;

#define HC12_TRANSMIT_SIZE sizeof(gps_vals_t)

/**
 *  Format the GPS data, in the order specified above, into
 */
// void serializeInto(uint8_t dest_buf[HC12_TRANSMIT_SIZE]);

/**
 *  Purely for testing purposes, to verify that the serialized values
 *  actually correspond to the raw values, once they are deserialized.
 */
// void deserialize(uint8_t buf[HC12_TRANSMIT_SIZE]);
void deserialize(uint8_t *buf);

/**
 *  Deserialize a transceivier byte array into a string representing the values.
 */
void deserializeIntoStr(uint8_t *buf, char* dest_str, bool log_to_user = true);

#endif