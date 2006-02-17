#!/bin/sh

#
# File must be changed for each aircraft presently.
# This will be changed later.  For now, see the comments below.
#

PATH=$PATH:/sbin:/usr/sbin:/usr/bin

app=Groundfeed.Sender

dbname=real-time
outname=/tmp/xmit/nimbus_sql_

cd /home/local/raf/groundfeed/lib

#
# C130 
#
groundvars=/jnet/local/proj/145/groundvars
dbhost=hercules
/usr/java/jre/bin/java \
  -cp groundfeed.jar:getopt.jar:postgresql.jar $app -g $groundvars -v \
  -h $dbhost -n $dbname -c -o $outname >> /tmp/Groundfeed.out 2>&1 &

#
# GV
#
#groundvars=/jnet/local/proj/145/groundvars
#dbhost=hyper
#/usr/java/jre/bin/java \
#  -cp groundfeed.jar:getopt.jar:postgresql.jar $app -g $groundvars -v \
#  -h $dbhost -n $dbname -5 -o $outname >> /tmp/Groundfeed.out 2>&1 &
