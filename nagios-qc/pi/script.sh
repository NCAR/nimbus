#This script launches nagios, compares the real-time psql feed to vardb, then pipes passive checks to nagios
#When exiting this script, use ctrl+c, not ctrl+z
#Author: Julian Quick, SUPER Intern 2014

#Erase information from last execution
rm commands
sudo rm /var/log/nagios/nagios.log
sudo rm /etc/nagios/radar/*
sudo /sbin/service nagios stop
sudo /sbin/service nagios start

#Check data
python pi.py

#Pipe data to nagios
sh shellcmd.sh

#scan nagios error log for missing configuration files, make them
sudo perl ../radar.pl
sleep 3
cat /etc/nagios/radar/*

#restart nagios with new configuration files
sudo /sbin/service nagios restart
sudo pkill -f radar

#Pipe information to nagios again
sh shellcmd.sh

#This loop indefinatly compares psql data to vardb and pipes passive 
#checks to nagios every 30 seconds
while :
do
    python pi.py
    sh shellcmd.sh
    sleep 30
done
