#ifndef TRACKAR_SLEEP_H_
#define TRACKAR_SLEEP_H_

#include <Arduino.h>

typedef enum USART_WAKE_RX : uint8_t {
	SLEEP_UNTIL_USART_1,
	SLEEP_UNTIL_USART_2,
} USART_WAKE_RX;

/**
 *  Sleep until we see RX data on one of the UART ports. We will always sleep Serial0 (UserSerial)
 *  and we will also sleep the USART module NOT specified by "usart_wake" - we basically want to
 *  disable everything except the USART module we'll want to wake from.
 */
void sleepUntilUartRX(USART_WAKE_RX usart_wake);

#endif