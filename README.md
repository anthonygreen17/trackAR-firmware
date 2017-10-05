# trackAR-firmware #

All of the firmware things

## Examples ##
If you develop a feature, put a test .ino script in the examples directory so others can easily test your feature, build off of it, etc.


## Submodules ##

Run `git submodule init` and `git submodule update` as soon as you clone!

## INSTALLATION ##

Note: Mac/Linux only

First, download the Arduino IDE onto your machine. Get the latest version -> https://www.arduino.cc/en/Main/Software

### Arduino-Makefile Setup (Optional) ###

Clone the repo: https://github.com/sudar/Arduino-Makefile

Next, figure out where Arduino's default library directory is. Mine is:

~~~
/Users/GreenBean/Downloads/Arduino.app/Contents/Java/hardware/arduino/avr/libraries
~~~

Run the following command to find the base directory under which all this stuff is installed (only tested on mac, may be different for Linux)

~~~
find ~ -type d -name Arduino.app
~~~
For me, that returns `/Users/GreenBean/Downloads/Arduino.app`. Whatever your path is, append "Contents/Java/hardware/arduino/avr/libraries" to it.

Add the following line to the end of your `~/.bashrc` (create one if you dont have it...):

For me (replace with your path):
~~~
export ARDUINO_LIB_PATH=/Users/GreenBean/Downloads/Arduino.app/Contents/Java/hardware/arduino/avr/libraries
~~~

## CONFIGURATION ##

Basically, I've found that the easiest way to maintain a large project is to put the directories directly in your `$ARDUINO_LIB_PATH`, but it would be much nicer to have all of our code live in this repo, so that's what we're gonna do. Use the `update-project.bash` script to create symlinks in `$ARDUINO_LIB_PATH` that point to this repo. Wohoo.

NOTE: Execute this script from the base of this git repo, plz:

~~~
$ ./scripts/update-project.bash
~~~

NOTE: depending on where you installed this, you may run into issues with permissions. If that happens, prepend
`sudo` to the front of the command.

## CONNECTING TO ATMega2560 VIA SERIAL IN ARDUINO IDE ##

Ensure that you have installed the following driver on your computer to enable communication with the CP2102 programmer: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

Under the tools subheader in the Arduino IDE ensure the following are selected and the ATMega2560 is plugged into the CP2102 which is connected to your computer:

Board - Arduino/Genuino Mega or Mega 2560  
Processor - ATMega2560 (Mega 2560)  
Port - /dev/c.SLAB_USBtoUART  
Programmer - USBasp  



