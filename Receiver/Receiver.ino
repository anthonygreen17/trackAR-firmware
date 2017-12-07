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
#include "trackAR_sleep.h"
#include "hc12.h"
#include "serializer.h"

/**
 *  The indicator LEDs care about what device we are
 */
#define IS_RECEIVER 1
#include "leds.h"

unsigned long lastBtWriteTime = 0, lastLedOnTime = 0, lastLedOffTime = 0, now = 0;
const unsigned int btWritePeriod = 1000, ledPeriod = 1000;  // milliseconds
const unsigned int ledOffTime = 750;
const unsigned int ledOnTime = ledPeriod - ledOffTime;

static volatile bool woke_from_wdt = false;

void setup()
{
  UserSerial.begin(115200);
	bluetooth::initialize();
  hc12::initialize();
  leds::initialize(RECEIVER);
}

// sleep for 8 seconds, blinking the LEDs every 1 second.
static void wdtSleepUntilUart()
{

  static bool leds_active = false;

  while (1)
  {
    setupWdtInterrupt(SEC_1);
    sleepUntilUartRX(SLEEP_UNTIL_USART_1, RECEIVER);

    // this means the UART woke us up
    if (!woke_from_wdt)
    {
      UserSerial.println("bailing early, woke up from Beacon transmission");
      return;
    }
    if (leds_active)
      leds::on(RECEIVER);
    else
      leds::off(RECEIVER);
    
    leds_active = !leds_active;
    woke_from_wdt = false;
  }
}

void loop()
{
  while (bt_module.isConnected())
  {
    // now = millis();
    leds::setFlag(BT_CONNECTED_FLAG);

    // handle data if its there
    if (false)
    {
      handleBeaconTransmission();
    }

    // if there's no data, sleep for a bit, waiting for WDT interrupt
    else
    {
      wdtSleepUntilUart();

      if (woke_from_wdt)
      {
        woke_from_wdt = false;
        UserSerial.println("no data received, woke from WDT interrupt");
      }
      else
      {
        leds::on(RECEIVER);
        UserSerial.println("woke up from UART interrupt");
        handleBeaconTransmission();
        leds::off(RECEIVER);
      }
    }
    leds::unsetFlag(HC12_RECEIVING_FLAG); 
  }
}

static void handleBeaconTransmission()
{
  UserSerial.println("got data from beacon!");
  hc12::processBytes();
  hc12::sendRxDataWithFunc(sendBtByteWrapper);
  // leds::on(RECEIVER);
}

// static void handleLeds(unsigned long now)
// {
//   UserSerial.print(now);
//   UserSerial.print(", ");
//   UserSerial.println(lastLedOnTime);
//   UserSerial.println(micros());
//   if (now - lastLedOnTime >= ledPeriod)
//   {
//     leds::on(RECEIVER);
//     lastLedOnTime = millis();
//   }
//   else if (now - lastLedOnTime >= ledOnTime)
//   {
//     leds::off(RECEIVER);
//   }

// }

/**
 *  Wrapper so we can print out the deserialized buffer of received data from the Beacon.
 */
static void sendBtByteWrapper(uint8_t* buf, unsigned int len)
{
  // send NOPOS if we havent received an actual data point
  if (len != hc12::MSG_LEN)
  {
    bluetooth::send("NOPOS");
    return;
  }
  char str_buf[50];
  deserializeIntoStr(buf, str_buf);
  bluetooth::send(str_buf);
}

/**
 *  We don't actually need this to do anything. We just want to wake up from sleep.
 */
ISR(WDT_vect)
{
  woke_from_wdt = true;
}
