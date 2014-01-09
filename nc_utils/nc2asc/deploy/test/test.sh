#!/bin/sh

for file in *.batch ; do
	echo ""
	echo $file
	rfile=`basename $file .batch`.asc
	outfile="lrt-$rfile"
	rm -f "$outfile"
	# java -jar nc2asc.jar -b $file
	nc2asc -b $file
	echo "rfile=$outfile      dir+rfile=test-std/$outfile"
	diff "$outfile" "test-std/$outfile"
done

