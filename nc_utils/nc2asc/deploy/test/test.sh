#!/bin/sh

for file in *.batch ; do
	echo ""
	echo $file
	java -jar nc2asc.jar -b $file
	rfile=`basename $file .batch`.asc
	echo "rfile=lrt-$rfile      dir+rfile=test-std/lrt-$rfile"
	diff "lrt-$rfile" "test-std/lrt-$rfile"
done

