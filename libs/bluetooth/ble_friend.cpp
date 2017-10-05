#include "ble_friend.h"
#include "general_config.h"


Adafruit_BluefruitLE_UART bt_module(BLE_FRIEND_HW_SERIAL, BLE_FRIEND_MODE_PIN);

namespace bluetooth
{

// A small helper for printing errors
static void bluetoothError(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void initialize(
	Adafruit_BLE *bt,
	void (*onConnect)(void),
	void (*onDisconnect)(void),
	void (*onRxBufReceive)(char*, uint16_t))
{
	while (!BLE_FRIEND_HW_SERIAL);  // just to be sure :)
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
		bt->setConnectCallback(onConnect);
	if (onDisconnect != NULL)
		bt->setDisconnectCallback(onDisconnect);
	if (onRxBufReceive != NULL)
		bt->setBleUartRxCallback(onRxBufReceive);
}

bool send(Adafruit_BLE *bt, const char* buf)
{
	bt->print("AT+BLEUARTTX=");
	bt->println(buf);
	return bt->waitForOK();
}

bool disableConnections(Adafruit_BLE *bt)
{
	bt->println("AT+GAPCONNECTABLE=0");
	return bt->waitForOK();
}

} // bluetooth

