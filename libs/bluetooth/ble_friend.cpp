#include "ble_friend.h"


Adafruit_BluefruitLE_UART bt_module(BLE_FRIEND_HW_SERIAL, BLE_FRIEND_MODE_PIN);

void initializeBtModule(Adafruit_BLE *bt, void (*onConnect)(void), void (*onDisconnect)(void) )
{
	// see Adafruit_BLE.cpp
	if (onConnect != NULL)
		bt->setConnectCallback(onConnect);
	if (onDisconnect != NULL)
		bt->setDisconnectCallback(onDisconnect);
}

bool sendToBtModule(Adafruit_BLE *bt, const char* buf)
{
	bt->print("AT+BLEUARTTX=");
	bt->println(buf);
	return bt->waitForOK();
}

bool disableBtConnections(Adafruit_BLE *bt)
{
	bt->println("AT+GAPCONNECTABLE=0");
	return bt->waitForOK();
}

