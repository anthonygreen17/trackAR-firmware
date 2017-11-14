#include "ble_friend.h"
#include "general_config.h"


Adafruit_BluefruitLE_UART bt_module(BT_SERIAL, BLE_FRIEND_MODE_PIN);

namespace bluetooth
{

// A small helper for printing errors
static void bluetoothError(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void initialize(
	void (*onConnect)(void),
	void (*onDisconnect)(void),
	void (*onRxBufReceive)(char*, uint16_t))
{
	while (!BT_SERIAL);  // just to be sure :)
  delay(100);
  if ( !bt_module.begin(VERBOSE_MODE) )
  {
    bluetoothError(F("Failed to initialize bluetooth, make sure it's in CoMmanD mode & check wiring?"));
  }
  UserSerial.println( F("Bluetooth initialized!") );

  if ( BLE_FACTORY_RESET_ON_STARTUP)
  {
    /* Perform a factory reset to make sure everything is in a known state */
    UserSerial.println(F("Performing a factory reset: "));
    if ( ! bt_module.factoryReset() ){
      bluetoothError(F("Couldn't factory reset"));
    }
  }

	// see Adafruit_BLE.cpp
	if (onConnect != NULL)
		bt_module.setConnectCallback(onConnect);
	if (onDisconnect != NULL)
		bt_module.setDisconnectCallback(onDisconnect);
	if (onRxBufReceive != NULL)
		bt_module.setBleUartRxCallback(onRxBufReceive);
}

// bool send(const char* buf, const char* delim)
// {
// 	char msg[strlen(buf) + strlen(delim) + 1];
// 	sprintf(msg, "%s%s", buf, delim);
// 	bt_module.print("AT+BLEUARTTX=");
// 	bt_module.println(msg);
// 	return bt_module.waitForOK();
// }

bool send(const uint8_t* buf, const unsigned int length)
{
	bt_module.print("AT+BLEUARTTX=");
	bt_module.write(buf, length);
	bt_module.println();
	return bt_module.waitForOK();
}

bool disableConnections()
{
	bt_module.println("AT+GAPCONNECTABLE=0");
	return bt_module.waitForOK();
}

} // bluetooth

