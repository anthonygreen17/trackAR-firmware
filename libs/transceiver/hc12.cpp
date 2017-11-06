#include "hc12.h"
#include "general_config.h"

extern "C" {
	#include "packet.h"
}


namespace hc12
{
	volatile uint8_t lastRxMsg[MAX_MSG_LEN]; 
	volatile bool receiving = false;
	static const int PACKET_HANDLER = 0;

	// various private functions
	static void sendPacket(uint8_t *data, unsigned int length);
	static void processPacket(uint8_t *data, unsigned int length);
	static void copyMsgFromBuf();

	/**
	 * This will get called after the packet interface properly packages a data buffer for us.
	 */
	static void sendPacket(uint8_t *data, unsigned int length)
	{
		HC12_SERIAL.write(data, length);
	}

	/**
	 *  This gets called after repeatedly calling packet_process_byte() until a full message
	 *  is received.
	 *
	 *  What should we do here...leave it as an unsigned char array or cast to char? hmmm
	 */
	static void processPacket(uint8_t *data, unsigned int length)
	{
		UserSerial.println("Received message from Beacon:");
		UserSerial.write(data, length);
		UserSerial.print("\n");

		// copy the received payload into lastRxMsg
		memcpy((void*)lastRxMsg, data, length);
		receiving = true;
	}

	void initialize()
	{
		HC12_SERIAL.begin(BAUD);

		// until we receive a message, set this up to send the noPosReceivedMsg to bluetooth module
		lastRxMsg[0] = noPosReceivedMsg;

		// initialize the packet interface to do the packet header/checksum/footer stuff for us
		packet_init(sendPacket, processPacket, PACKET_HANDLER);
		receiving = false;
	}

	void send(const char* msg)
	{
		send((uint8_t *)msg, strlen(msg));
	}

	void send(uint8_t *data, unsigned int length)
	{
		packet_send_packet(data, length, PACKET_HANDLER);
	}

	void processBytes()
	{
		while (HC12_SERIAL.available() > 0)
		{
			packet_process_byte(HC12_SERIAL.read(), PACKET_HANDLER);
		}
	}

	// bool processBytes()
	// {
	// 	while (HC12_SERIAL.available() > 0)
	// 	{
	// 		buf[bufWriteIndex++] = HC12_SERIAL.read();

	// 		/**
	// 		 * Keep track of state with regards to receiving the end of the message.
	// 		 */
	// 		if (buf[bufWriteIndex] == msgDelim[0])
	// 		{
	// 			firstDelimReceived = true;
	// 		}

	// 		*
	// 		 * If the last character we received was the first character in the message delimiting sequence, check to see if this
	// 		 * is the second character in the delimiting sequence. If it is, that means we received a full message. If it's not,
	// 		 * reset the states of stuff.
			 
	// 		else if (firstDelimReceived)
	// 		{
	// 			if (buf[bufWriteIndex] == msgDelim[1])
	// 			{
	// 				copyMsgFromBuf();
	// 				newMsgReceived = true;
	// 				bufWriteIndex = 0;
	// 				return true;
	// 			}
	// 			else
	// 			{
	// 				// reset state - this was NOT the end of a message
	// 				firstDelimReceived = false;
	// 			}
	// 		}
	// 	}
	// 	return false;
	// }
}