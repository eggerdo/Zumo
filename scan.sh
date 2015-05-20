#!/bin/bash

if [ "$#" -eq 1 ]; then
	DEVICE_FILTER=$1

	echo 'Scanning ...'

	# scans for bluetooth devices but displays only results from RN42 bluetooth devices
	sudo hcitool -i hci0 scan | grep $DEVICE_FILTER
else
	sudo hcitool -i hci0 scan
fi

