#! /usr/bin/env bash

function logError() {
	>&2 echo $1
}

LowPowerLib=Low-Power

# these are the directories within the trackAR-firmware repo that we'd like to use in Arduino IDE compilation
declare -a LIB_DIRS=( $LowPowerLib )

if [ ! -d $LowPowerLib ]; then
	printf "Run this from the base of the trackAR-firmware repo, please and thank u\n"
	exit 1
fi

RepoDir=$(pwd)

# iterate through the list of directories, add symlinks to ARDUINO_LIB_PATH
# first, make sure the proper symbols are defined and stuff
[ -z "$ARDUINO_LIB_PATH" ] && echo "Need to set ARDUINO_LIB_PATH" && exit 1;
if [ ! -d $ARDUINO_LIB_PATH ]; then
	printf "Your ARDUINO_LIB_PATH does not exist...\n"
	exit 1
fi

for lib_dir in "${LIB_DIRS[@]}"
do
	# add symlink for each directory to point to our repo
	ln -s $RepoDir/$lib_dir $ARDUINO_LIB_PATH
done