#ifndef HC_12_H_
#define HC_12_H_
#include <Arduino.h>
#include "serializer.h"

namespace hc12
{
	constexpr unsigned int BAUD        = 9600;
	constexpr unsigned int MAX_MSG_LEN = 512;
	constexpr unsigned int SET_PIN     = A1;
	constexpr unsigned int MSG_LEN = sizeof(gps_vals_t);

	// use this to put the Xbee to sleep.
	constexpr unsigned int SLEEP_PIN = A4;

	/**
	 *  After this amount of time, assume that we stopped receiving messages.
	 */
	constexpr unsigned int MSG_TIMEOUT_MS = 3000;

	/**
	 * Send this over bluetooth if we connect and havent received any transceiver data yet.
	 */
	constexpr const uint8_t noPosReceivedMsg = 14;

	/**
	 * Carry out any necessary configuration tasks.
	 */
	void initialize();

	/**
	 * Process all characters in the RX buffer, repeatedly calling packet_process_byte() on each byte. The packet
	 * interface will call the "rx_func" passed to packet_init() after a full message is received.
	 */
	void processBytes();

	/**
	 * Send the message, adding the delimiting characters first. This ends up calling packet_send_packet(), which
	 * wraps the data in headers/footers/checksum, and sends it through the function passed as "send_func" in
	 * packet_init().
	 */
	void send(const char* msg);
	void send(uint8_t* data, unsigned int length);
	
	/**
     * (NOTE): This function must be called while the transceiver is in command mode
	 * Send AT Command ("AT-SLEEP") to put transceiver into Sleep mode, then flush the serial to ensure all
	 * bytes are sent before setting pin to HIGH, which takes the transceiver out of command mode.
	 */
	void sleep();
	
	/**
     * Set pin LOW, then back to HIGH, to take it out of sleep mode
	 */
	void unsleep();

	/**
	 *  Send the transceiver message with the provided function. This allows us an easy way
	 *  to internally send either the last received message, or the "noPosReceivedMessage",
	 *  depending on if we have received data or not.
	 *
	 *  NOTE: intended for bluetooth module send function
	 */
	void sendRxDataWithFunc( void (*func)(uint8_t*, unsigned int) );
}

#define hc12PrepareSleep() (digitalWrite(hc12::SET_PIN, LOW))

#endif // HC_12_H_

