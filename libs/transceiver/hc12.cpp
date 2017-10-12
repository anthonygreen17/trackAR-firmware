#include "hc12.h"
#include "general_config.h"

extern "C" {
	#include "packet.h"
}


namespace hc12
{
	volatile char lastRxMsg[MAX_MSG_LEN] = "\0";
	volatile bool newMsgReceived = false;
	volatile unsigned int lastRxMsgLength = 0;

	/**
	 * Hold data in here until we receive a full message, then copy it over to the lastRxMsg buffer.
	 */
	static char buf[MAX_MSG_LEN];
	static volatile unsigned int bufWriteIndex = 0;
	static volatile bool firstDelimReceived = false;

	static const int PACKET_HANDLER = 0;

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
	}

	/**
	 * Copy the received message over from our internal buffer to lastRxMsg. May or may not discard the delimiting
	 * chars.
	 */
	static void copyMsgFromBuf()
	{
		// leave space for null terminating char
		int bytesToCopy = trimDelims ? bufWriteIndex + 1 - strlen(msgDelim) : bufWriteIndex + 1;
		memcpy((void*)lastRxMsg, buf, bytesToCopy);
		lastRxMsg[bytesToCopy] = '\0';
	}

	void initialize()
	{
		HC12_SERIAL.begin(BAUD);

		// until we receive a message, set this up to send the noPosReceivedMsg to bluetooth module
		strcpy((char*)lastRxMsg, noPosReceivedMsg);

		// initialize the packet interface to do the packet header/checksum/footer stuff for us
		packet_init(sendPacket, processPacket, PACKET_HANDLER);
	}

	void send(const char* msg)
	{
		char msgWithDelim[strlen(msg) + strlen(msgDelim)];
		strcat(msgWithDelim, msg);
		strcat(msgWithDelim, msgDelim);
		send((uint8_t *)msgWithDelim, strlen(msgWithDelim));
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