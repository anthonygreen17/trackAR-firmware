#include "ble_friend.h"
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"

// A small helper for printing errors
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// callback to execute when we establish connection to a bluetooth device
void onBluetoothDeviceConnect()
{
  Serial.println("Just connected to a device");
}

void onBluetoothDeviceDisconnect()
{
  Serial.println("Just disconnected from a device :(");
}

void setup(void)
{
  while (!Serial);  // just to be sure :)
  delay(100);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F ("Initialising the Bluefruit LE module: ") );

  if ( !bt_module.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORY_RESET_ON_STARTUP)
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! bt_module.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  bt_module.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  bt_module.info();
  bt_module.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! bt_module.isConnected()) {
      delay(500);
  }
  onBluetoothDeviceConnect();
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  // Check for user input
  char inputs[BUFSIZE+1];

  if ( getUserInput(inputs, BUFSIZE) )
  {
    // Send characters to Bluefruit! 
    
    // print on serial monitor...
    Serial.print("[Send] ");
    Serial.println(inputs);

    // ... and send over UART to the Bluefruit
    sendToBtModule(&bt_module, inputs);
  }

  checkForIncomingData();
}

// /**
//  * Check for incoming data from the bluetooth module.
//  */
// void checkForIncomingData()
// {
//   bt_module.println("AT+BLEUARTRX");
//   bt_module.readline();
//   if (strcmp(bt_module.buffer, "OK") == 0) {
//     // no data
//     return;
//   }
//   // Some data was found, its in the buffer
//   Serial.print(F("[Recv] ")); Serial.println(bt_module.buffer);
//   bt_module.waitForOK(); 
// }

/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
*/
/**************************************************************************/
bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while( (!Serial.available()) && !timeout.expired() ) { delay(1); }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && (Serial.available()) );

  return true;
}
