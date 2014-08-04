from lxml import etree
def ServiceMaker(Host,Service,conf):
   conf.write('define service{\n')
   conf.write('   use                     generic-radar-service\n')
   conf.write('   host_name               '+Host+'\n')
   conf.write('   service_description     '+Service+'\n')
   conf.write('}\n\n')

def HostMaker(Host,conf):
   conf.write('define host{\n')
   conf.write('   use                     generic-radar-host\n')
   conf.write('   host_name               '+Host+'\n')
   conf.write('   alias                   '+Host+'\n')
   conf.write('   address                 0.0.0.0\n')
   conf.write('}\n\n')

configFile=open('/etc/nagios/raf/config.cfg','w')
HostName='RAF'
HostMaker(HostName,configFile)

#Using commands output file
#-------------------------
#commands=open('commands')
#lines=commands.readlines()
#for line in lines:
#   ServiceMaker(line.split(';')[0],line.split(';')[1],configFile)

#Using checks xml
Checks=etree.parse('checks.xml')
for Elm in Checks.getiterator('check'):
  ServiceMaker(HostName,Elm.attrib['variable'],configFile)
