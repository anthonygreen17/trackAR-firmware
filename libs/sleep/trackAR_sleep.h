#ifndef TRACKAR_SLEEP_H_
#define TRACKAR_SLEEP_H_

#include <Arduino.h>
#include "general_config.h"

typedef enum USART_WAKE_RX : uint8_t {
	SLEEP_UNTIL_USART_1,
	SLEEP_UNTIL_USART_2,
} USART_WAKE_RX;

/**
 *  This enum is structured to correspond
 */
typedef enum WDT_SLEEP_LENGTH : uint8_t {
	MS_16 =            0,
	MS_32 =  (1 << WDP0),
	MS_64 =  (1 << WDP1),
	MS_125 = (1 << WDP0) | (1 << WDP1),
	MS_250 = (1 << WDP2),
	MS_500 = (1 << WDP2) | (1 << WDP0),
	SEC_1  = (1 << WDP2) | (1 << WDP1),
	SEC_2  = (1 << WDP2) | (1 << WDP1) | (1 << WDP0),
	SEC_4  = (1 << WDP3),
	SEC_8  = (1 << WDP3) | (1 << WDP1),
} WDT_SLEEP_LENGTH;

/**
 *  Sleep until we see RX data on one of the UART ports. We will always sleep Serial0 (UserSerial)
 *  and we will also sleep the USART module NOT specified by "usart_wake" - we basically want to
 *  disable everything except the USART module we'll want to wake from.
 */
void sleepUntilUartRX(USART_WAKE_RX usart_wake, TrackARDevice dev);

/**
 *  Enter SLEEP_MODE_PWR_DOWN.
 */
void sleepPwrDown(TrackARDevice dev);

/**
 *  Set up the watchdog timer to interrupt in the desired amount of time. Make sure to define an
 *  ISR(WDT_vect) function to handle the interrupt.
 */
void setupWdtInterrupt(WDT_SLEEP_LENGTH length);

#define disableWdt() (WDTCSR = 0x00)

/**
 *  Disable things that we dont need.
 */
void disableUnneededPeriphs(TrackARDevice dev);

#endif