#!/bin/sh

# Requires project number as 1st arg.
if [ $# == 0 ]; then
  echo $0: Usage 'Groundfeed.sh proj_number'.
  exit 1
fi

PATH=$PATH:/sbin:/usr/sbin:/usr/bin

app=Groundfeed.Sender

dbname=real-time
outname=$XMIT_DIR/nimbus_sql_
groundvars=$PROJ_DIR/$1/groundvars.rt

if [ ${1:0:1} == 1 ]; then
  dbhost=hercules
fi

if [ ${1:0:1} == 5 ]; then
  dbhost=hyper
fi

cd $JLOCAL/raf/groundfeed/lib

/usr/java/jre/bin/java \
  -cp groundfeed.jar:getopt.jar:postgresql.jar $app -g $groundvars -v \
  -h $dbhost -n $dbname -${1:0:1} -o $outname >> /tmp/Groundfeed.out 2>&1 &
