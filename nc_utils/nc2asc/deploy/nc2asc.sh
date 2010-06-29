#!/bin/csh -x

setenv JNC2ASC /opt/local/nc2asc

java -jar ${JNC2ASC}/nc2asc.jar $* 
