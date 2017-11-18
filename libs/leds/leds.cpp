#include "leds.h"



namespace leds
{

volatile uint8_t indicator_flag = 0;
static bool active = false;

/**
 *  These pins are silkscreen labeled as such
 */
static const unsigned long GPS_INDICATOR_PIN  = A10;
static const unsigned long BT_CONNECTED_PIN   = A10;
static const unsigned long BT_SEARCHING_PIN   = A12;
static const unsigned long HC12_RECEIVING_PIN = A14;

void initialize(TrackARDevice dev)
{
	switch (dev)
	{
		case BEACON:
			pinMode(GPS_INDICATOR_PIN,  OUTPUT);
			break;

		case RECEIVER:
			pinMode(BT_CONNECTED_PIN,    OUTPUT);
			pinMode(BT_SEARCHING_PIN,    OUTPUT);
			pinMode(HC12_RECEIVING_PIN,  OUTPUT);
			break;
	}
}

void off(TrackARDevice dev)
{
	if (!active)
		return;

	switch (dev)
	{
		case BEACON:
			digitalWrite(GPS_INDICATOR_PIN,  0);
			break;

		case RECEIVER:
			digitalWrite(BT_CONNECTED_PIN,   0);
			digitalWrite(BT_SEARCHING_PIN,   0);
			digitalWrite(HC12_RECEIVING_PIN, 0);
			break;
	}
	active = false;
}

void on(TrackARDevice dev)
{
	if (active)
		return;
	uint8_t f = indicator_flag;

	switch (dev)
	{
		case BEACON:
			digitalWrite(GPS_INDICATOR_PIN,  f & (1 << RECEIVING_GPS_DATA_FLAG));
			break;

		case RECEIVER:
			digitalWrite(BT_CONNECTED_PIN,   f & (1 << BT_CONNECTED_FLAG));
			digitalWrite(HC12_RECEIVING_PIN, f & (1 << HC12_RECEIVING_FLAG));
			digitalWrite(BT_SEARCHING_PIN,   f & (1 << BT_SEARCHING_FLAG));
			break;
	}
	active = true;
}
void setFlag(indicator_t flag)
{
	indicator_flag |= (1 << flag);
}

void unsetFlag(indicator_t flag)
{
	indicator_flag &= ~(1 << flag);
}

} //leds