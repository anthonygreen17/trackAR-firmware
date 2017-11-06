/**
 * The entry point for the receiver's executable. Should perform the following tasks AFTER THE FINAL REVISION:
 * 
 *   1) Receive GPS data from the Beacon, via the transceiver. Store the latest transceiver data in some variable.
 *   2) Every 1 second, publish the latest transceiver received from the Beacon. If we haven't received any data at all,
 *      send "NOPOS" to indicate that we haven't yet acquired a position from any Beacon.
 *
 *  On powerup, the bluetooth module should be energized for a certain period of time, awaiting a bluetooth connection.
 *  After this period, bluetooth should be turned off until the user presses the "Bluetooth Pairing Button", which will
 *  then cause the Receiver to energize the bluetooth module again for a certain period of time. If a connection is made,
 *  leave bluetooth active. If not, turn it off.
 *
 *  The transceiver should always be energized since we need to always be ready to accept incoming data.
 *
 *  NOTE: The process above is intended for the final revision of the Receiver, and does not necessarily have to be
 *  carried out at first (ie: first revision, leave the bluetooth module always energized).
 */

#include <Arduino.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "ble_friend.h"
#include "general_config.h"
#include "hc12.h"
#include "serializer.h"

unsigned long lastBtWriteTime = 0, now = 0;
unsigned int btWritePeriod = 1000;  // milliseconds

void setup()
{
  UserSerial.begin(115200);
	bluetooth::initialize(&bt_module);
  hc12::initialize();
}

void loop()
{
	now = millis();
  hc12::processBytes();

  if (bt_module.isConnected())
  {
  	if (now - lastBtWriteTime > btWritePeriod)
  	{
      hc12::sendRxDataWithFunc(sendBtByteWrapper);
	  	lastBtWriteTime = millis();
	  }
  }
}

void sendBtByteWrapper(uint8_t* buf, unsigned int len)
{
  deserialize(buf);
  // use to verify that the Beacon send hc12::noPosReceivedMsg until it gets GPS data
//  UserSerial.println(buf[0]);
  bluetooth::send(&bt_module, buf, len);
}
