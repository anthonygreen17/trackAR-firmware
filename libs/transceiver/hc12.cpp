#include "hc12.h"
#include "general_config.h"
#include "serializer.h"
#include "leds.h"

extern "C" {
	#include "packet.h"
}


namespace hc12
{
	static volatile uint8_t lastRxMsg[MSG_LEN]; 
	static volatile bool receiving = false;
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
		UserSerial.print("Received message from Beacon with length ");
		UserSerial.println(length);

		// copy the received payload into lastRxMsg
		memcpy((void*)lastRxMsg, data, length);
		receiving = true;
		leds::setFlag(HC12_RECEIVING_FLAG);
	}

	void initialize()
	{
		HC12_SERIAL.begin(BAUD);

		// until we receive a message, set this up to send the noPosReceivedMsg to bluetooth module
		lastRxMsg[0] = noPosReceivedMsg;

		// initialize the packet interface to do the packet header/checksum/footer stuff for us
		packet_init(sendPacket, processPacket, PACKET_HANDLER);
		
		//initialize pin that puts transceiver into command mode, set it to LOW to put into command mode
		pinMode(SET_PIN, OUTPUT);
		
		//Transceiver is not in command mode at start
		digitalWrite(SET_PIN, HIGH); 

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

	void sendRxDataWithFunc( void (*func)(uint8_t*, unsigned int) )
	{
		if (receiving)
			func(lastRxMsg, MSG_LEN);
		else
			func(&noPosReceivedMsg, 1);
	}

	void processBytes()
	{
		while (HC12_SERIAL.available() > 0)
		{
			packet_process_byte(HC12_SERIAL.read(), PACKET_HANDLER);
		}
	}

	void sleep(){
		HC12_SERIAL.write("AT+SLEEP");
		HC12_SERIAL.flush();
		digitalWrite(SET_PIN, HIGH); 
	}

	void unsleep(){
		digitalWrite(SET_PIN, LOW); 
		digitalWrite(SET_PIN, HIGH); 
	}
}