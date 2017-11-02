#ifndef HC_12_H_
#define HC_12_H_
#include <Arduino.h>

namespace hc12
{
	constexpr unsigned int BAUD        = 9600;
	constexpr unsigned int MAX_MSG_LEN = 512;
	constexpr unsigned int SET_PIN     = A1;

	/**
	 * Send this over bluetooth if we connect and havent received any transceiver data yet.
	 */
	constexpr const char* noPosReceivedMsg = "NOPOS";

	/**
	 * Statically allocate a buffer to hold the last received message.
	 */
	extern volatile char lastRxMsg[MAX_MSG_LEN];
	extern volatile unsigned int lastRxMsgLength;

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
	void sleep();
	void unsleep();
}

#define hc12PrepareSleep() (digitalWrite(hc12::SET_PIN, LOW))

#endif // HC_12_H_

