#!/usr/bin/python

# Python script to dump contents of the vardb.xml file using the
# python editors internal structure.
# Author : Julian Quick

import time
import sys
sys.path.append('../editpy')
import getInfo
myList = getInfo.getinfo("../editpy/VDB.xml")


# METHOD 1
# Order matters in the XML
# Does not take missing information into account

# for line in myList:
#  print " {0: <20}{1: <25}{2: <28}".format(line[1],line[2],line[3])

#==============================================

# METHOD 2
# This way works with any XML element order
for line in myList:
  nameIndex=line[0].index('name')+1

  #if no units exist, 'None' is displayed
  try:unitIndex=line[0].index('units')+1
  except ValueError:
      unitIndex=len(line)
      line.append('None')

  try : longNameIndex=line[0].index('long_name')+1
  except ValueError:
      longNameIndex=len(line)
      line.append('None')

  print " {0: <20}{1: <25}{2: <28}".format(line[nameIndex],line[unitIndex],line[longNameIndex])

