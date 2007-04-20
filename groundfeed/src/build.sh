#!/bin/sh

/usr/java/jre/bin/javac -classpath ../lib/getopt.jar Groundfeed/*java
jar cvf ../lib/groundfeed.jar Groundfeed/*class
