import numpy
import psycopg2
import os
import imp
from lxml import etree
getInfo=imp.load_source('getInfo','../vardb/editpy/getInfo.py')
#Julian Quick
#SUPER intern 2014
# compares psql data feed to metadata via checks configured iin checks.xml
# outputs check results to /tmp/nagiosPassiveCommands
#================================================
def specialCase(elm,data):
   variable=elm.attrib['variable']
   if variable=='GGQUAL':
      if int(data)==5:
         return ok+metSep+'at 5'
      elif 1<data and 5>data:
         return warning+metSep+'value is '+str(data)
      elif int(data)==0:
         return critical+metSep+'0'
      else: return warning+metSep+' value is '+str(data)

def flatLining(varnum,rows,elm):
   calcList=[]
   if 'tolerance' in elm.attrib:
      tolerance=elm.attrib['tolerance']
   else:
      tolerance = defaultTolerance
   if 'lookBack' in elm.attrib:
      checkRecords=elm.attrib['lookBack']
   else:
      checkRecords=defaultCheckRecords
   for j in range(0,int(checkRecords)):
     calcList.append(rows[j][varnum])
   if numpy.std(calcList)<tolerance:
      return flats
   else:
      return ok+metSep+'not flatlining'

def boundsCheck(elm,data):
   # Check for bounds in check, find in vardb if not there
   if minlim not in elm.attrib:
      for var in getInfo.getinfo(vardb):
         if var[1]==elm.attrib['variable']:
            if minlim in var[0]:
               minBoundary=var[var[0].index(minlim)]
               maxBoundary=var[var[0].index(maxlim)]
            else: return warning+metSep+' No metdata'
   else:
      try:
         minBoundary=float(elm.attrib['min_limit'])
         maxBoundary=float(elm.attrib['max_limit'])
      except KeyError:
         return warning+metSep+' No metdata'

   if minBoundary>data:
      return critical+metSep+'**Below minimum limit** value is '+str(data)
   elif maxBoundary<data:
      return critical+metSep+'**Above maximum limit** value is '+str(data)
   else:
      return ok+metSep+'bounds'

def constant(dataRows,varnum,elm):
   if 'tolerance' in elm.attrib:
      tolerance=elm.attrib['tolerance']
   else:
      tolerance = defaultTolerance
   if 'lookBack' in elm.attrib:
      checkRecords=elm.attrib['lookBack']
   else:
      checkRecords=defaultCheckRecords
   calcList=[]
   for j in range(0,int(checkRecords)):
     calcList.append(dataRows[j][varnum])
   if numpy.std(calcList)>tolerance:
      return flaps
   else:
      return ok+metSep+'constant'
#================================================
#status: 0-Good 1-No Data 2-Bounds or flatlining
#-----------------------------------------------
#signalHistory is the max number of entries to scan when checking for flatlining
signalHistory=200

#defaultCheckRecords is the default number of entries to scan back when doing a flatline or stability test
defaultCheckRecords=25

vardb=os.path.expandvars("$PROJ_DIR/$PROJECT/$AIRCRAFT/vardb.xml")

metSep=':  '

defaultTolerance=0.01

maxlim='max_limit'
minlim='min_limit'
#nodat is number representing no data
missingdata=-32767.0

#cmds is passive check output file, which is piped to nagios by shell
cmds=open('/tmp/nagiosPassiveCommands','w+')

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
#foo = psycopg2.connect(database="real-time", user="ads", host="steam")

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
cur.execute("select * from raf_lrt order by datetime desc limit "+str(signalHistory)+";")
rows=cur.fetchall()
#---------------------------------------
#Get monitoring instructions from monitoring.xml
monidoc=etree.parse(os.path.expandvars('${PROJ_DIR}/${PROJECT}/${AIRCRAFT}/checks.xml'))
#---------------------------------------
#set up status dictionary
status={}
i=0
while i<len(names):
   status[names[i]]=[]
   splits=names[i].split('_')
   if len(splits)>1:
      idMet=splits[0]
      for k in range(1,len(splits)-1):
         idMet+='_'+str(splits[k])
   else:idMet=names[i]
   for elm in monidoc.getiterator('check'):
      var=elm.attrib['variable']
      if var.upper()==names[i].upper() or var.strip('.*').upper()==str(idMet).upper():
         if elm.attrib['type']=='flatline':
            status[names[i]].append(flatLining(i,rows,elm))
         elif elm.attrib['type']=='bounds':
            status[names[i]].append(boundsCheck(elm,row[i]))
         elif elm.attrib['type']=='stable':
            status[names[i]].append(constant(rows,i,elm))
         elif elm.attrib['type']=='custom':
            status[names[i]].append(specialCase(elm,row[i]))

         #Check for no data. Overrides other checks.
         if row[i]==missingdata:
            status[names[i]].append(nodat)
   i+=1

for key in status:
#   print key,status[key]
   if len(key)!=0:
      for entry in status[key]:
         cmds.write('RAF'+';'+key+';'+nagiosSignals[entry.split(metSep)[0]]+';'+entry+'\n')
cmds.close()
