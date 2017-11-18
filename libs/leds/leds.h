/**
 *  Note: This file depends on you defining either the symbol IS_BEACON or the symbol
 *  IS_RECEIVER, BEFORE you include this file.
 */

#ifndef TRACKAR_LEDS_H_
#define TRACKAR_LEDS_H_
#include <Arduino.h>
#include "general_config.h"

/**
 * Use this flag along with the enum to decide which LEDs to flash.
 */
typedef enum INDICATOR : uint8_t {
	BT_CONNECTED_FLAG = 1,
	RECEIVING_GPS_DATA_FLAG = 2,
	BT_SEARCHING_FLAG = 3,
	HC12_RECEIVING_FLAG = 4,
} indicator_t;

namespace leds
{

volatile extern uint8_t indicator_flag;

void initialize(TrackARDevice dev);
void off(TrackARDevice dev);
void on(TrackARDevice dev);
void setFlag(indicator_t flag);
void unsetFlag(indicator_t flag);

#endif

} // leds
