#!/usr/bin/python

# creates nagios configuration files based on checks.xml. Supports ".*"
# wildcard in xml.

from vardb import VariableList

vars = VariableList().loadVariables()

# names=[s[0] for s in names]

configfile = NagiosConfig.NagiosConfig()

HostName='RAF'

configfile.write(configfile.makeHost(HostName))

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
