#!/bin/sh
#
# Daemon script to run passive nagios check for data quality.  Launched from
# ensure_running.
# Author: Julian Quick, SUPER Intern 2014

# This loop indefinatly compares psql data to vardb and pipes passive 
# checks to nagios every 30 seconds
while :
do
    /home/local/raf/nagios-qc/compare.py
    /home/local/raf/nagios-qc/piper.sh
    sleep 30
done
