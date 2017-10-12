#! /usr/bin/env bash

function logError() {
	>&2 echo $1
	printf "\nFailed!\n"
	exit 1
}

LowPowerLib=Low-Power

# these are the directories within the trackAR-firmware repo that we'd like to use in Arduino IDE compilation
declare -a LIB_DIRS=( $LowPowerLib libs/trackAR_Interrupts Adafruit_BluefruitLE_nRF51 libs/bluetooth libs/trackAR_Config libs/packet)

if [ ! -d $LowPowerLib ]; then
	logError "Run this from the base of the trackAR-firmware repo, please and thank u"
fi

RepoDir=$(pwd)

# iterate through the list of directories, add symlinks to ARDUINO_LIB_PATH
# first, make sure the proper symbols are defined and stuff
[ -z "$ARDUINO_LIB_PATH" ] && logError "Need to set ARDUINO_LIB_PATH";

if [ ! -d $ARDUINO_LIB_PATH ]; then
	logError "Your ARDUINO_LIB_PATH does not exist..."
fi

printf "\nCreating symlinks...\n"
for lib_dir in "${LIB_DIRS[@]}"
do
	if [ ! -L $ARDUINO_LIB_PATH/$lib_dir ]; then

		# add symlink for each directory to point to our repo
		ln -s $RepoDir/$lib_dir $ARDUINO_LIB_PATH
	else
		printf "%s symlink already exists...skipping\n" "$lib_dir"
	fi
done

printf "\nSuccess!\n"
