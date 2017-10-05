#ifndef HC_12_H_
#define HC_12_H_
#include <Arduino.h>


#define HC12_SERIAL Serial2

namespace hc12
{
	// constexpr HardwareSerial& serial  = Serial2;
	constexpr unsigned int BAUD        = 9600;
	constexpr unsigned int MAX_MSG_LEN = 100;

	/**
	 * Send this over bluetooth if we connect and havent received any transceiver data yet.
	 */
	constexpr const char* noPosReceivedMsg = "NOPOS";

	/**
	 * Statically allocate a buffer to hold the last received message.
	 */
	extern volatile char lastRxMsg[MAX_MSG_LEN];
	extern volatile unsigned int lastRxMsgLength;
	extern volatile bool newMsgReceived;

	/**
	 * Each received message is expected to be delimited by this character sequence.
	 */
	constexpr const char* msgDelim = "\r\n";

	/**
	 * If true, lastRxMsg will contain the entirety of the received message, without the delimiting characters.
	 */
	constexpr const bool trimDelims = true;

	/**
	 * Carry out any necessary configuration tasks.
	 */
	void initialize();

	/**
	 * Process all characters received from the HC12. Return true if a new message was received as the result of this
	 * function call, and return false otherwise.
	 *
	 * NOTE: will return if a complete message is detected.
	 */
	bool processBytes();

}



#endif // HC_12_H_