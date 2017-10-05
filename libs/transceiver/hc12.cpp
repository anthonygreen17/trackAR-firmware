#include "hc12.h"


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

	/**
	 * Copy the received message over from our internal buffer to lastRxMsg. May or may not discard the delimiting
	 * chars.
	 */
	static void copyMsgFromBuf()
	{
		// leave space for null terminating char
		int bytesToCopy = trimDelims ? bufWriteIndex + 1 - strlen(msgDelim) : bufWriteIndex + 1;
		memcpy(lastRxMsg, buf, bytesToCopy);
		lastRxMsg[bytesToCopy] = '\0';
	}

	void initialize()
	{
		HC12_SERIAL.begin(BAUD);

		// until we receive a message, set this up to send the noPosReceivedMsg to bluetooth module
		strcpy(lastRxMsg, noPosReceivedMsg);
	}

	bool processBytes()
	{
		while (HC12_SERIAL.available() > 0)
		{
			buf[bufWriteIndex] = HC12_SERIAL.read();

			/**
			 * Keep track of state with regards to receiving the end of the message.
			 */
			if (buf[bufWriteIndex] == msgDelim[0])
			{
				firstDelimReceived = true;
			}

			/**
			 * If the last character we received was the first character in the message delimiting sequence, check to see if this
			 * is the second character in the delimiting sequence. If it is, that means we received a full message. If it's not,
			 * reset the states of stuff.
			 */
			else if (firstDelimReceived)
			{
				if (buf[bufWriteIndex] == msgDelim[1])
				{
					copyMsgFromBuf();
					newMsgReceived = true;
					bufWriteIndex = 0;
					return true;
				}
				else
				{
					// reset state - this was NOT the end of a message
					firstDelimReceived = false;
				}
			}

			bufWriteIndex = bufWriteIndex + 1;
		}
		return false;
	}
}