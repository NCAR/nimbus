#!/bin/sh

# Requires project number as 1st arg.
if [ $# == 0 ]; then
  echo $0: Usage 'Groundfeed.sh proj_name'.
  exit 1
fi

if [ -z ${AIRCRAFT} ]; then
  echo Environment variable AIRCRAFT not set.
  exit 1
fi

PATH=$PATH:/sbin:/usr/sbin:/usr/bin

app=Groundfeed.Sender

dbname=real-time
groundvars=${PROJ_DIR}/$1/${AIRCRAFT}/groundvars.rt

# Make sure outname matches file names that the ground LDM is expecting.
# see malbec:~ldm/etc/ldmd.conf & pqact.conf files.
#
if [ ${AIRCRAFT} == C130_N130AR ]; then
  outname=${XMIT_DIR}/${AIRCRAFT:+C130}_nimbus_sql_
  dbhost=hercules
  ac=1
fi

if [ ${AIRCRAFT} == GV_N677F ]; then
  outname=${XMIT_DIR}/${AIRCRAFT:+GV}_nimbus_sql_
  dbhost=hyper
  ac=5
fi

cd ${JLOCAL}/raf/groundfeed/lib

/usr/java/latest/bin/java \
  -cp groundfeed.jar:getopt.jar:postgresql.jar $app -g $groundvars \
  -h $dbhost -n $dbname -$ac -o $outname \
  >> /tmp/Groundfeed.out 2>&1 &
