#!/bin/sh

/usr/java/jdk/bin/javac -classpath ../lib/getopt.jar Groundfeed/*java
jar cvf ../lib/groundfeed.jar Groundfeed/*class
