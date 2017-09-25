## Notes on setup ##

For some reason, I couldnt establish communication with the Bluefruit until
I tied the CTS pin to ground and set the switch on the Bluetooth to
"UART", even though it seems like it should be in command mode...

The RTS pin was unused and still seems to work fine. Will probably have to tie
it either high or low in the future.


### Commands ###

#### ATI ####

Get a bunch of device information

#### AT+HELP ####

Get a list of all possible commands.
