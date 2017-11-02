#include "serializer.h"
#include "general_config.h"
#include "string.h"
#include "stdio.h"


// void deserialize(uint8_t buf[HC12_TRANSMIT_SIZE])
void deserialize(uint8_t *buf)
{
	gps_vals_union vals;
	memcpy(vals.data_bytes, buf, HC12_TRANSMIT_SIZE);

	char msg[100];
	char str_temp1[10];
	char str_temp2[10];
	char str_temp3[10];
	// 4 is mininum width, 5 is precision;
	dtostrf(vals.vals.latitude, 4, 5, str_temp1);
	dtostrf(vals.vals.longitude, 4, 5, str_temp2);
	dtostrf(vals.vals.altitude, 4, 5, str_temp3);
	sprintf(msg, "deserialized values: %s, %s, %s",
		str_temp1, str_temp2, str_temp3);
	UserSerial.println(msg);
}
