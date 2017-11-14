#include "hc12.h"
#include "TinyGPS++.h"
#include "trackAR_gps.h"
#include "general_config.h"
#include "trackAR_sleep.h"
#include "serializer.h"
#include "avr/wdt.h"

char gps_message[150];
USART_WAKE_RX usart_wake = SLEEP_UNTIL_USART_2;
uint8_t serialized_gps_data[HC12_TRANSMIT_SIZE];

/**
 *  Set this to true for maaaaaad debug printed to UserSerial
 */
#define USER_DEBUG true

void setup()
{ 
  disableUnneededPeriphs(BEACON);
  gps::boot();  
  hc12::initialize();
  UserSerial.begin(115200);
  GPS_SERIAL.begin(gps::BAUD);
  if (!gps::sync())
    UserSerial.println("Problem syncing GPS!");
  else
    UserSerial.println("GPS properly synced");
  UserSerial.flush();
}
/**
 *  The call to gps::sync() should return immediately as a transmission is completed. So, when we
 *  call sleepUntilUartRx(), that means we should sleep for almost an entire second, until the 
 *  very first byte of the next transmission is received. At that point, we need to bail out of
 *  sleep mode and process all of the bytes of the transmission using gps::smartDelay().
 *  Then, simply format and send out the latest received GPS data.
 *  
 *  In order to save power, we're going to leverage a WDT interrupt. This allows use to go into the deepest
 *  sleep mode, SLEEP_MODE_PWR_DOWN, which saves a LOT more power than the SLEEP_MODE_IDLE that is needed
 *  to wake up via UART RX. The time between the first RX byte from the GPS and the last TX byte sent 
 *  out to the transceiver has been found to be ~400ms. To be safe, we'll use the WDT to sleep for 
 *  around 375ms before going into SLEEP_MODE_IDLE to let the GPS wake us up.
 */
void loop()
{
  setupWdtInterrupt(MS_250);
  sleepPwrDown(BEACON);
  disableWdt();
  setupWdtInterrupt(MS_125);
  sleepPwrDown(BEACON);
  disableWdt();
  hc12::unsleep();
  sleepUntilUartRX(usart_wake, BEACON);

  /**
   * The typical time from this point to the end of the loop was 
   * found to be ~400ms, when sending a ~60byte string at 9600 baud
   */
   
  if (!gps::smartDelay(2000))
  {
    UserSerial.println("Problem syncing GPS. Resyncing...");
    gps::sync();
  }
  
  gps::serializeInto(serialized_gps_data, USER_DEBUG);

  if (USER_DEBUG)
  {
    UserSerial.flush();
    printSerializedDataBytes(serialized_gps_data);
    deserialize(serialized_gps_data);
    UserSerial.flush();
  }

  hc12::send(serialized_gps_data, HC12_TRANSMIT_SIZE);
  HC12_SERIAL.flush();

  if (USER_DEBUG)
  {
    UserSerial.println("\n");
    UserSerial.flush();
  }

  //Wait for all bytes to be transmitted
  delay(10);

  //Set transceiver set pin to low to put into command mode
  hc12PrepareSleep();

  //Wait for transceiver to enter command mode
  setupWdtInterrupt(MS_32);
  sleepPwrDown(BEACON);
  disableWdt();
  setupWdtInterrupt(MS_16);
  sleepPwrDown(BEACON);
  disableWdt();

  hc12::sleep();  
}

/**
 * Test function to print out deserialized bytes.
 */
void printSerializedDataBytes(uint8_t data[12])
{
  UserSerial.print("Raw string: [");
  for (int i = 0; i < HC12_TRANSMIT_SIZE; ++i)
  {
    UserSerial.print(data[i]);
    UserSerial.print(", ");
  }
  UserSerial.println("]");
  UserSerial.flush();
}

/**
 *  We don't actually need this to do anything. We just want to wake up from sleep.
 */
ISR(WDT_vect)
{
}


