# Bluetooth #

Adafruit BLE UART friend: https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend?view=all

## Wiring ##

NakedMega -> Bluefruit

~~~
TX1       -> RXI
RX1       -> TXO
5V        -> VIN
GND       -> GND
Pin12     -> MOD
GND       -> CTS
~~~

At this point, it seems like RTS is unneeded and can also be pulled to ground (it wasnt pulled to ground when testing this, but it probably should be.)

## Notes ##

We can set up callbacks that can be executed on a connection/disconnect:

~~~
typedef void (*conenctFunctionPointer)(void);
~~~

we can do the same for a data buffer received from the iPhone app:

~~~
typedef void (*rxBufReceived)(char*, uint16_t);
~~~

Pass function pointers to initializeBluetooth() (see `ble_friend.h`) to get this functionality.

**NOTE**
In order for these functions to actually be called, you need to call `bt_module.update()` repeatedly to process events.