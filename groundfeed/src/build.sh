#!/bin/sh

/usr/java/latest/bin/javac -classpath ../lib/getopt.jar Groundfeed/*java
jar cvf ../lib/groundfeed.jar Groundfeed/*class
