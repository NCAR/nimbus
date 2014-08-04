##!/bin/sh
#
#This script launches nagios, compares the real-time psql feed to vardb, then pipes passive checks to nagios
#When exiting this script, use ctrl+c, not ctrl+z
#Author: Julian Quick, SUPER Intern 2014

#This loop indefinatly compares psql data to vardb and pipes passive 
#checks to nagios every 30 seconds
while :
do
    compare.py
    piper.sh
    sleep 30
done
