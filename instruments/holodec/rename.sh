#!/bin/bash

###
# this script renames the holodec files
# usage: ./rename.sh EVENT PROJ_DATE
# ex: ./rename.sh RF01 20150707
# written by Soo Park, 2015
###


DIR=$(pwd)/$1 # event

cd $DIR
echo "Working in $DIR"

for file in *
do
  IFS=_ read event var <<< "$file"
  #echo "$event"
  #echo "$var"
  
  IFS=. read time seq data <<< "$var"
  #echo $time
  #echo $seq
  #echo $data

  IFS=- read hour min sec <<< "$time"
  #echo $hour
  #echo $min
  #echo $sec

  echo "new name is: "
  newName="$1.$2_$hour$min$sec.$seq.$data"
  echo "$file -> $newName"

  echo "Please press enter to change the name. Any other input will not change the name."
  read input
  if ["$input" == ""]
  then
    mv $file $newName 
    #echo "yes"
  else
    true
  fi

done


