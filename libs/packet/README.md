# Packet Interface #

Big thanks to `vedderb`, one of the masterminds behind the VESC open source motor controller platform (check out https://github.com/vedderb/bldc if you'd like to his firmware work) for supplying this system which ensures packet integrity.

## Overview ##

Packet format is as follows:

- 1 start byte ("2" for short packets, "3" for packets with length > 255)
- 1 or 2 bytes for packet length
- packet payload
- 2 CRC bytes
- stop byte

All we have to do is send a raw `unsigned char` (`uint8_t`) buffer, along with it's length, to the packet interface. It will wrap the payload in everything above. On the receive side, call `packet_process_byte()` on every received byte and the packet interface will call a function (passed to `packet_init()`) to process the received packet. Nice.


## Timeouts ##

The packet interface is configured to timeout when receiving packets if there is more than a 5 millisecond gap between packets. That seems reasonable. In order to keep track of the timing, we need to call `packet_timerfunc()` every millisecond. This is probably a job for a timer interrupt.

If we don't want the timeout functionality, we can avoid it by just literally never calling that function.