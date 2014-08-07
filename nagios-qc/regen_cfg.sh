#!/bin/sh

# Generate new nagios configuration files based on sample_checks.xml.  This is currently
# more a template of how to regenerate nagios checks after changing the checks.xml file.
# Author: Julian Quick, SUPER Intern 2014

# Erase information from last execution
sudo rm /etc/nagios/raf/*
sudo /sbin/service nagios stop

# Create configuration files (make sure to pass ads environment to sudo).
sudo -E configMaker.py

cat /etc/nagios/raf/*

# Restart nagios with new configuration files
sudo /sbin/service nagios start
