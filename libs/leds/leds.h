#ifndef LEDS_H_
#define LEDS_H_

// TODO - make these actual pins
#define BT_CONNECTED_PIN   13
#define GPS_INDICATOR_PIN  13
#define BT_SEARCHING_PIN   13

/**
 * Use this flag along with the enum to decide which LEDs to flash.
 */
typedef enum indicator : uint8_t {

	BT_CONNECTED_FLAG = 1,
	RECEIVING_GPS_DATA_FLAG = 2,
	BT_SEARCHING_FLAG = 3,
} indicator_t;

volatile extern uint8_t led_indicator_flag;

void indicatorLedsOff();
void indicatorLedsOn();
void setIndicatorFlag(indicator_t flag);
void unsetIndicatorFlag(indicator_t flag);

#endif
