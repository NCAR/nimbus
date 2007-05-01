#!/bin/bash

#
# Format of incoming file should be:
# line 1 : aircraft, 'gv' or 'c130'
# line 2 : file type, 'script' or 'sql'
# line 3...n : body of file to execute.
#

read aircraft
read file_type

case $aircraft in
  C130)
    PGHOST=sloop.atd.ucar.edu ; export PGHOST;
    ;;

  GV)
    PGHOST=lenado.atd.ucar.edu ; export PGHOST;
    ;;
esac

case $file_type in
  sql)
    while read line; do
# This if statement is temporary until I figure out why nimbus keeps
# sending next day 23:59:59 records once midnight is crossed.
      echo $line | /bin/grep 23:59:59 > /dev/null
      if [ $? != 0 ]; then
        echo $line | /h/9/cjw/bin/static_psql
      fi
    done;;

  script)
    while read line; do
      $line
    done;;
esac

