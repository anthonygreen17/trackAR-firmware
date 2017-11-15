#ifndef BLE_FRIEND_H_
#define BLE_FRIEND_H_
#include "Arduino.h"
#include "Adafruit_BluefruitLE_UART.h"

/**
 *  Wiring is as follows:
 *
 *  NakedMega -> Bluefruit
 *
 *  TX1       -> RXI
 *  RX1       -> TXO
 *  5V        -> VIN
 *  GND       -> GND
 *  Pin12     -> MOD
 *  GND       -> CTS
 *
 *  At this point, it seems like RTS is unneeded and can also be pulled to ground (it wasnt pulled to ground
 *  when testing this, but it probably should be.)
 */
extern Adafruit_BluefruitLE_UART bt_module;

#define BLE_FRIEND_MODE_PIN       12

// max bufsize for incoming data to the serial port
#define BUFSIZE            128

// enables debug output if set to true
#define VERBOSE_MODE                 false
#define BLE_FACTORY_RESET_ON_STARTUP false

// set to true if you want the bluefruit to echo commands given
#define ECHO_COMMANDS            false

// the max time that the bluetooth module will wait on Receiver start-up until we turn off bluetooth
#define BT_POWERUP_SEARCH_PERIOD_SEC 30 


/**
 * Do any necessary initialization steps here. For now, this just sets up
 * callbacks to be executed on connect/disconnect with a BLE device.
 *
 * NOTE -> This will only work if you loop, calling bt->update(). 
 */

namespace bluetooth
{

void initialize( 
  void (*onConnect)(void) = NULL, void (*onDisconnect)(void) = NULL,
  void (*onRxBufReceive)(char*, uint16_t) = NULL
);

/**
 * Send the buffer to the BT module. Return true if the module returns "OK", and return
 * false otherwise.
 */
bool send(const char* buf, const char* delim = "\r\n");

/**
 *  Send a raw byte array to the bluetooth module.
 */
// bool send(const uint8_t* buf, const unsigned int length);

// dont allow the BT module to connect to any other components
bool disableConnections();

} // bluetooth

#endif  //BLE_FRIEND_H_


/**

Notes:

- UART at 9600 baud, CTS + RTS required
- 5V tolerant pins, but can take 3V3
- So, supply with 3V3 to conserve power?


PINS:
- CTS -> pulled high by default, need to pull low to tell the chip that it can
				 send data out of its TX0 pin to the micro

- RTS -> this pin will be low when its fine to send data to the Bluefruit (unneeded?)


FCR - factory reset, on back of board
DFU - DFU pin, pull low and flash FW


*/
