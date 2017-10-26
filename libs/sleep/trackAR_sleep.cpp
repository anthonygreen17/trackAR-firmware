#include "trackAR_sleep.h"
#include "avr/sleep.h"
#include "avr/power.h"
#include "general_config.h"
#include "Arduino.h"


void disableUnneededPeriphs(TrackARDevice dev)
{
	switch(dev)
	{
		case BEACON:
			power_adc_disable();
			power_spi_disable();
			power_twi_disable();
			break;

		case RECEIVER:
			break;
	}
}

void sleepUntilUartRX(USART_WAKE_RX usart_wake, TrackARDevice dev)
{
	/*
	 * Sleep modes from avr/sleep.h:
	 *
	 * SLEEP_MODE_IDLE -the least power savings
	 * SLEEP_MODE_ADC
	 * SLEEP_MODE_PWR_SAVE
	 * SLEEP_MODE_STANDBY
	 * SLEEP_MODE_PWR_DOWN -the most power savings
	 *
	 * the power reduction management <avr/power.h> is described in
	 * http://www.nongnu.org/avr-libc/user-manual/group_avr_power.html
	 */

	set_sleep_mode(SLEEP_MODE_IDLE);

	// enables the sleep bit in the mcucr register, doesnt sleep yet
	sleep_enable();
	power_all_disable();

	switch (usart_wake)
	{
		case SLEEP_UNTIL_USART_1:
			power_usart1_enable();
			break;
		case SLEEP_UNTIL_USART_2:
			power_usart2_enable();
			break;
	}

	// put the device to sleep
	sleep_mode(); 

	// THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
	sleep_disable();
	power_all_enable();
	disableUnneededPeriphs(dev);
}
