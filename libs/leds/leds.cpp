#include "leds.h"

volatile uint8_t led_indicator_flag = 0;

void indicatorLedsOff()
{
	digitalWrite(BT_CONNECTED_PIN,  0);
	digitalWrite(GPS_INDICATOR_PIN, 0);
	digitalWrite(BT_SEARCHING_PIN,  0);
}

void indicatorLedsOn()
{
	if (led_indicator_flag & (1 << BT_CONNECTED_FLAG) )
	{
		digitalWrite(BT_CONNECTED_PIN,  1);
	}
	if (led_indicator_flag & (1 << RECEIVING_GPS_DATA_FLAG) )
	{
		digitalWrite(GPS_INDICATOR_PIN, 1);
	}
	if (led_indicator_flag & (1 << BT_SEARCHING_FLAG) )
	{
		digitalWrite(BT_SEARCHING_PIN,  1);
	}
}
void setIndicatorFlag(indicator_t flag)
{
	led_indicator_flag |= (1 << flag);
}

void unsetIndicatorFlag(indicator_t flag)
{
	led_indicator_flag &= ~(1 << flag);
}