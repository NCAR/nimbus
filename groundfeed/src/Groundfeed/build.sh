#!/bin/sh

/usr/java/jre/bin/javac -classpath ../../lib/getopt.jar *java
jar cvf ../../lib/groundfeed.jar *class
