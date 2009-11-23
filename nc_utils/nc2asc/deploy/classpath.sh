#!/bin/sh

# set the CLASSPATH to the jar files found in lib/linux

cp=(lib/*.jar )
OLDIFS=$IFS
IFS=:
# This syntax for expanding an array expands to a single word
# with each member separated by the first character in IFS
CLASSPATH="${cp[*]}"
IFS=$OLDIFS

echo $CLASSPATH
export CLASSPATH

