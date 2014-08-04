#!/bin/sh
# This script writes passive checks to the Nagios command file to cause
# it to process service check results.  Called by qc_check.

echocmd="/bin/echo"
CommandFile="/var/spool/nagios/cmd/nagios.cmd"
while read line
do
  # get the current date/time in seconds since UNIX epoch
  datetime=`date +%s`

  # create the command line to add to the command file
  cmdline="[$datetime] PROCESS_SERVICE_CHECK_RESULT;$line"
  #echo "$cmdline"

  # append the command to the end of the command file
  `$echocmd $cmdline >> $CommandFile`
#  echo $cmdline
done <commands
