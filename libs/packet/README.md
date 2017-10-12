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