import numpy
import psycopg2
import os
import sys
lib_path=os.path.abspath('../vardb/editor/.')
sys.path.append(lib_path)
import getInfo
#Julian Quick
#SUPER intern 2014
#================================================
#status: 0-Good 1-No Data 2-Bounds or flatlining
#-----------------------------------------------
#flatLineHistory is the number of entries to scan when checking for flatlining
flatLineHistory=3

#Flat line tolerance is the highest allowable deviation allowable for a signal to
#still be considered flatlining
flatLineTolerance=0.000001

#nodat is number representing no data
nodat=-32767.0

#cmds is passive check output file, which is piped to nagios by shell
cmds=open('commands','w+')

#VardDB Location
vardb=os.path.expandvars('$PROJ_DIR/$PROJECT/$AIRCRAFT/vardb.xml')
vardb='VDB.xml'
#================================================
#foo = psycopg2.connect(database="real-time", user="ads", host="acserver")
foo = psycopg2.connect(database="real-time", user="ads", host="hercules.raf-ext.ucar.edu")
cur=foo.cursor()

#Get live data names
cur.execute("SELECT column_name FROM information_schema.columns WHERE table_name = 'raf_lrt'")
names=cur.fetchall()
names=[s[0] for s in names]

#initiate status list
status=[0]*len(names)
message=['OK']*len(names)

#Get metadata by parsing vardb
vdbEntries=getInfo.getinfo()
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
#Get 20 most recent entries
cur.execute("select * from raf_lrt order by datetime desc limit "+str(flatLineHistory)+";")
rows=cur.fetchall()

#Check data 
i=1
while(i<len(row)):
   if isinstance(row[i],list):
      pass
   if names[i].upper() in vdbNames or (names[i].strip('_')[0]).upper() in vdbNames:
     #++++++++++++++++++++
     #check for flatlining

     #calculate sandard deviation
     j=0
     calcList=[]
     while j<flatLineHistory:
       calcList.append(rows[j][i])
       j+=1
     if numpy.std(calcList)<flatLineTolerance:
       message[i]='**FLATLINING**  '
       status[i]=2

     #++++++++++
     #Check if data is in bounds
     for elm in vdbEntries:
       if elm[1]==names[i] or elm[1]==names[i].split('_')[0]:
          j=1
          while j<len(elm)-1:
            if elm[0][j]=='maxLimit':
               if float(elm[j+1])>row[i]:
                 message[i]+='**Above maximum limit**  '
                 status[i]=2
            if elm[0][j]=='minLimit':
               if float(elm[j+1])<row[i]:
                 message[i]+='**Below minimum limit**  '
                 status[i]=2
            j+=1
  
     #+++++++++++++++++
     #Check for no data
     if row[i]==nodat:
        status[i]=1
        message[i]='**NO DATA**  '

   i+=1
#write name:message:status to file
i=0 
while(i<len(names)):
  if names[i].upper() in vdbNames or (names[i].strip('_')[0]).upper() in vdbNames:
    cmds.write('RAF'+';'+names[i]+';'+str(status[i])+';'+message[i]+'\n')
  i+=1
cmds.close()
