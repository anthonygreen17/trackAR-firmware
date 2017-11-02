#include "trackAR_sleep.h"
#include "avr/sleep.h"
#include "avr/power.h"
#include "avr/wdt.h"
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

void setupWdtInterrupt(WDT_SLEEP_LENGTH length)
{
	// clear reset flag so that we're able to modify register contents
  // MCUSR &= ~(1<<WDRF);
  MCUSR = 0;
  
  // setting these enable bits allow us to change WDT parameters within the next 4 clock cycles 
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  //set new watchdog timeout prescaler value
  WDTCSR = length;
  
  // Set the wdt to interrupt, not reset the MCU
  WDTCSR |= 1 << WDIE;
  wdt_reset();
}


void sleepPwrDown(TrackARDevice dev)
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();

	sleep_disable();
	power_all_enable();
	disableUnneededPeriphs(dev);
}
