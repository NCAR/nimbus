#!/usr/bin/python

# creates nagios configuration files based on checks.xml. Supports ".*" wildcard in xml.
import psycopg2
import os
from lxml import etree
def ServiceMaker(Host,Service,conf):
   conf.write('define service{\n')
   conf.write('   use                     generic-radar-service\n')
   conf.write('   host_name               '+Host+'\n')
   conf.write('   service_description     '+Service.lower()+'\n')
   conf.write('}\n\n')

def HostMaker(Host,conf):
   conf.write('define host{\n')
   conf.write('   use                     generic-radar-host\n')
   conf.write('   host_name               '+Host+'\n')
   conf.write('   alias                   '+Host+'\n')
   conf.write('   address                 0.0.0.0\n')
   conf.write('}\n\n')

db = psycopg2.connect(database="real-time", user="ads", host="acserver")
cur=db.cursor()
cur.execute("SELECT column_name FROM information_schema.columns WHERE table_name = 'raf_lrt'")
names=cur.fetchall()
names=[s[0] for s in names]
configFile=open('/etc/nagios/raf/config.cfg','w')
HostName='RAF'
HostMaker(HostName,configFile)
Checks=etree.parse(os.path.expandvars('${PROJ_DIR}/${PROJECT}/${AIRCRAFT}/checks.xml'))

#Conf file for QC
for Elm in Checks.getiterator('check'):
  if Elm.attrib['variable'][-2:]==".*":

    #keep variable name without trailing .*
    orig=Elm.attrib['variable'][:-2]
    for nam in names:
       splits=nam.split('_')
       var=splits[0]

       #remove last _xxxx entry
       if len(splits)>1:
          for i in range(1,len(splits)-1):  
             var+="_"+splits[i]

       if var.upper()==orig.upper() or nam.upper()==orig.upper():
          ServiceMaker(HostName,nam,configFile)
  else:
    for nam in names:
       if Elm.attrib['variable'].upper()==nam.upper():
          ServiceMaker(HostName,Elm.attrib['variable'],configFile)
          break

#Conf for no data tab
noDatHost='No Data Check'
HostMaker(noDatHost,configFile)
for name in names:
   ServiceMaker(noDatHost,name,configFile)

configFile.close()
