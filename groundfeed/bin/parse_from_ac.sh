#!/bin/bash

# Called by exec_sql.sh.  Parsing of sql files brought to the ground.

#
# Format of incoming file should be:
# line 1 : aircraft, 'gv' or 'c130'
# line 2 : file type, 'script' or 'sql'
# line 3...n : body of file to execute.
#

read aircraft
read file_type

# default host.
export PGHOST=eol-rt-data.guest.ucar.edu

# This becomes needed if we fly two planes at once, we need multiple databases.
case $aircraft in
  C130)
#    export PGHOST=sloop.atd.ucar.edu
#    export PGHOST=lenado.atd.ucar.edu
    ;;

  NRLP3)
#    export PGHOST=lenado.atd.ucar.edu
    ;;

  GV)
#    export PGHOST=lenado.atd.ucar.edu
    ;;
esac

case $file_type in
  sql)
    while read line; do
        echo $line | psql
    done;;

  script)
    while read line; do
      $line
    done;;
esac
