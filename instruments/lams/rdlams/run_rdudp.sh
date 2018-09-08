#!/bin/sh
#
# Start rdudp to read UDP packets from the aircraft and write
# the data to the database
#

# This is where the log will go
logfile="/tmp/rdudp.log"

# Move the previous logfile, if any
if [[ -e $logfile ]]; then
	mv $logfile $logfile.old
fi

# Kill any running instances of rdudp
killall rdudp

# Start the new rdudp in background, logging to $logfile.  Assume that rdudp
# lives in the same place as this script.
fullpath=`which $0`
homedir=${fullpath%/*}
$homedir/rdudp </dev/null >$logfile 2>&1 &
