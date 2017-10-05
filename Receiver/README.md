# Receiver #

## Overview ##

This is the entry point for the Receiver's executable. Should perform the following tasks AFTER THE FINAL REVISION:

	- Receive GPS data from the Beacon, via the transceiver. Store the latest transceiver data in some **volatile** variable.

 	- Every 1 second, publish the latest transceiver received from the Beacon. If we haven't received any data at all, send "NOPOS" to indicate that we haven't yet acquired a position from any Beacon.


 ## Power-up ## 
 
 On powerup, the bluetooth module should be energized for a certain period of time, awaiting a bluetooth connection. After this period, if the user doesn't connect, bluetooth should be turned off until the user presses the "Bluetooth Pairing Button", which will then cause the Receiver to energize the bluetooth module again for a certain period of time. If a connection is made, leave bluetooth active. If not, turn it off.
 
 The transceiver should always be energized since we need to always be ready to accept incoming data.
 
 NOTE: The process above is intended for the final revision of the Receiver, and does not necessarily have to be
 carried out at first (ie: first revision, leave the bluetooth module always energized).
 