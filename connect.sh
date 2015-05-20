#!/bin/bash

# colour assignments, see -> http://linuxtidbits.wordpress.com/2008/08/13/output-color-on-bash-scripts-advanced/
ORANGE="\033[38;5;214m"
GREEN="\033[38;5;46m"
RED="\033[38;5;196m"

PORTNR=0

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
while getopts ":t:p:h" optname; do
	case "$optname" in
		"t")
			TARGET=$OPTARG
			;;
		"p")
			PORTNR=$OPTARG
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

print "connecting to $TARGET on rfcomm$PORTNR ..." $ORANGE

# check if we are connected already
go_on=`rfcomm show $PORTNR 2>/dev/null | grep connected`
if [[ -z $go_on ]]; then
	sudo rfcomm release $PORTNR 2>/dev/null

	# start rfcomm connect in another shell
	sudo rfcomm connect /dev/rfcomm$PORTNR $TARGET > /dev/null &
	
	sleep 1
	
	# wait for connection to be established. wait at most 10 seconds
	declare -i COUNTER
	COUNTER=0
	go_on=`rfcomm show $PORTNR 2>/dev/null | grep connected`
	while [[ -z $go_on ]]; do
		sleep 1
	  let COUNTER=$COUNTER+1
		go_on=`rfcomm show $PORTNR 2>/dev/null | grep connected`
		if [ $COUNTER -eq 10 ]; then
			print "connection failed!" $RED
			exit 1
		fi
	done

	print "-> connection ok" $GREEN
	
else 
	print "-> already connected" $GREEN
fi
