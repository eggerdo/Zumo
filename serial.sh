#!/bin/bash

# colour assignments, see -> http://linuxtidbits.wordpress.com/2008/08/13/output-color-on-bash-scripts-advanced/
ORANGE="\033[38;5;214m"
GREEN="\033[38;5;46m"
RED="\033[38;5;196m"

PORT=/dev/rfcomm0
BAUD=115200

# print function, first parameter is the string, second parameter the colour.
# colour is optional
function print {
	if [ -z $2 ]; then
		echo -e $1
	else
		COLOR_RESET="\033[39m"
		echo -e $2$1$COLOR_RESET
	fi
}

# show usage description
function showUsage {
	echo
	echo -e "Usage:"
	echo
	echo -e "    -t <target>      specify the target either as the bluetooth"
	echo -e "                     MAC address or the name of the device"
	echo -e "    -p <port>        define the port number on which the connection"
	echo -e "                     should be established. default is 0"
	echo -e "                     Note: only specify the number, e.g. 0 and not"
	echo -e "                     /dev/rfcomm0"
	echo
	echo -e "Note:"
	echo -e "If no target is defined, the file 'arduino_ip.txt' is checked by default."
	echo
}

echo "$@"

# check for option arguments
while getopts ":t:p:b:h" optname; do
	case "$optname" in
		"t")
			TARGET=$OPTARG
			;;
		"p")
			PORT=$OPTARG
			;;
		"b")
			BAUD=$OPTARG
			;;
		"h")
			showUsage
			exit
			;;
		*)
			print "Unknown error while processing options, use -h for usage information" $RED
			exit
			;;
	esac
done

if [ -z $TARGET ]; then
	print "FATAL: need to provide a address as parameter -t, use -h for usage information" $RED
	exit 1
fi

. ./connect.sh -t $TARGET && picocom -b $BAUD $PORT