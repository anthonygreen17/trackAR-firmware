# trackAR-firmware #

All of the firmware things


## INSTALLATION ##

Note: Mac/Linux only

First, download the Arduino IDE onto your machine. I have version 1.8, not sure how much that matters. Just get one of the latest.

Next, figure out where Arduino's default library directory is. Open your Arduino IDE, navigate to `Preferences`. You will see your sketchbook path there. For example, mine is:

~~~
/Users/GreenBean/Documents/Arduino
~~~

Append `/libraries` to that path and add the following line to the end of your `~/.bashrc` (make on if you dont have it...):

~~~
export ARDUINO_LIB_PATH=$YOUR_PATH_FROM_ABOVE/libraries
~~~

For me:
~~~
export ARDUINO_LIB_PATH=/Users/GreenBean/Documents/Arduino/libraries
~~~

## CONFIGURATION ##

Basically, I've found that the easiest way to maintain a large project is to put the directories directly in your `$ARDUINO_LIB_PATH`, but it would be much nicer to have all of our code live in this repo, so that's what we're gonna do. Use the `update-project.bash` script to create symlinks in `$ARDUINO_LIB_PATH` that point to this repo. Wohoo.



