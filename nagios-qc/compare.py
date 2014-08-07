#!/usr/bin/python

import numpy
import psycopg2
import os
import imp
from lxml import etree
getInfo=imp.load_source('getInfo','../vardb/editpy/getInfo.py')
#Julian Quick
#SUPER intern 2014
#================================================
# Definitions of checks
# These checks expect variable names to be an exact match, or the 
# live data names to have extra information after a _

def specialCase(elm,data):
   variable=elm.attrib['variable']
   if variable=='GGQUAL':
      if int(data)==5:
         return ok+'::at 5'
      elif 1<data and 5>data:
         return warning+'::value is '+str(data)
      elif int(data)==0:
         return critical+'::0'
      else: return warning+':: value is '+str(data)

def flatLining(varnum,rows,elm):
   calcList=[]
   for j in range(0,flatLineHistory):
     calcList.append(rows[j][varnum])
   if 'tolerance' in elm.attrib:
      tolerance=elm.attrib['tolerance']
   else:
      tolerance = defaulttolerance
   if numpy.std(calcList)<tolerance:
      return flats
   else:
      return ok+'::not flatlining'

def boundsCheck(elm,data):

   # Check for bounds in check, find in vardb if not there
   if minlim not in elm.attrib:
      for var in getInfo.getinfo(vardb):
         if var[1]==elm.attrib['variable']:
            if minlim in var[0]:
               minBoundary=var[var[0].index(minlim)]
               maxBoundary=var[var[0].index(maxlim)]
            else: return warning+':: No metdata'
   else:
      try:
         minBoundary=elm.attrib['min_limit'] 
         maxBoundary=elm.attrib['max_limit'] 
      except KeyError:
         return warning+':: No metdata'

   if minBoundary>data:
      return critical+'::**Below minimum limit** value is '+str(data)
   elif maxBoundary<data:
      return critical+'::**Above maximum limit** value is '+str(data)
   else:
      return ok+'::bounds'

def constant(dataRows,varnum,elm):
   if 'tolerance' in elm.attrib:
      tolerance=elm.attrib['tolerance']
   else:
      tolerance = defaulttolerance
   calcList=[]
   for j in range(0,flatLineHistory):
     calcList.append(dataRows[j][varnum])
   if numpy.std(calcList)>tolerance:
      return flaps
   else:
      return ok+'::constant'
#================================================
#status: 0-Good 1-No Data 2-Bounds or flatlining
#-----------------------------------------------
#flatLineHistory is the number of entries to scan when checking for flatlining
flatLineHistory=6
vardb=os.path.expandvars("$PROJ_DIR/$PROJECT/$AIRCRAFT/vardb.xml")

defaultTolerance=0.01

maxlim='max_limit'
minlim='min_limit'
#nodat is number representing no data
missingdata=-32767.0

#cmds is passive check output file, which is piped to nagios by shell
cmds=open('/tmp/commands','w+')

#VardDB Location
vardb=os.path.expandvars('$PROJ_DIR/$PROJECT/$AIRCRAFT/vardb.xml')

#define status entries
ok='OK'
warning='warning'
critical='critical'
flats='**FLATLINING**'
flaps='**FLAPPING**'
bounds='**OUT OF BOUNDS**'
nodat='**NO DATA**'

nagiosSignals={}
nagiosSignals[ok]='0'
nagiosSignals[nodat]='1'
nagiosSignals[warning]='1'
nagiosSignals[critical]='2'
nagiosSignals[flats]='2'
nagiosSignals[flaps]='2'
nagiosSignals[bounds]='2'
#================================================
#used for deubugging
#foo = psycopg2.connect(database="real-time-C130", user="ads", host="eol-rt-data.fl-ext.ucar.edu")

foo = psycopg2.connect(database="real-time", user="ads", host="acserver")
cur=foo.cursor()

#Get live data names
cur.execute("SELECT column_name FROM information_schema.columns WHERE table_name = 'raf_lrt'")
names=cur.fetchall()
names=[s[0] for s in names]

#Get metadata by parsing vardb
vdbEntries=getInfo.getinfo(vardb)
vdbNames=[]
for name in vdbEntries:
  vdbNames.append(name[1])
#---------------------------------------
#Get live data
#row is a list of the most recent data entries
#rows is a list of the 20 most recent data entry lists

#Get most recent entry
cur.execute("select * from raf_lrt where datetime=(select max(datetime) from raf_lrt );")
row=cur.fetchone()
#---------------------------------------
#Get recent entries
cur.execute("select * from raf_lrt order by datetime desc limit "+str(flatLineHistory)+";")
rows=cur.fetchall()
#---------------------------------------
#Get monitoring instructions from monitoring.xml
monidoc=etree.parse(os.path.expandvars('${PROJ_DIR}/${PROJECT}/${AIRCRAFT}/checks.xml'))
#---------------------------------------
#set up status dictionary
status={}
for elm in monidoc.getiterator('check'):
   status[elm.attrib['variable']]=[]
i=0
while i<len(names):
   for elm in monidoc.getiterator('check'):
      if elm.attrib['variable'].upper()==names[i].upper() or elm.attrib['variable'].split('_')[0].upper()==names[i].upper():
         if elm.attrib['type']=='flatline':
            status[elm.attrib['variable']].append(flatLining(i,rows,elm))
         elif elm.attrib['type']=='bounds':
            status[elm.attrib['variable']].append(boundsCheck(elm,row[i]))
         elif elm.attrib['type']=='stable':
            status[elm.attrib['variable']].append(constant(rows,i,elm))
         elif elm.attrib['type']=='custom':
            status[elm.attrib['variable']].append(specialCase(elm,row[i]))

         #Check for no data
         if row[i]==missingdata:
            status[elm.attrib['variable']].append(nodat)
   i+=1

for key in status:
#   print key,status[key]
   if len(key)!=0:
      for entry in status[key]:
         cmds.write('RAF'+';'+key+';'+nagiosSignals[entry.split('::')[0]]+';'+entry+'\n')
cmds.close()
