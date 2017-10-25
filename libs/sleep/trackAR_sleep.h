#ifndef TRACKAR_SLEEP_H_
#define TRACKAR_SLEEP_H_

#include <Arduino.h>

typedef enum USART_WAKE_RX : uint8_t {
	SLEEP_UNTIL_USART_1,
	SLEEP_UNTIL_USART_2,
} USART_WAKE_RX;

/**
 *  Sleep until we see RX data on one of the UART ports.
 */
void sleepUntilUartRX();

#endif