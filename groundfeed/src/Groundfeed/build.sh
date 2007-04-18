#!/bin/sh

javac -classpath ../../lib/getopt.jar *java
jar cvf ../../lib/groundfeed.jar *class
