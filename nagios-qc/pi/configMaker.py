def ServiceMaker(Host,Service,conf):
   conf.write('define service{\n')
   conf.write('   use                     generic-radar-service\n')
   conf.write('   host_name               '+Host+'\n')
   conf.write('   service_description     '+Service+'\n')
   conf.write('}\n\n')

commands=open('commands')
configFile=open('/etc/nagios/raf/config.cfg','w')
lines=commands.readlines()
for line in lines:
   ServiceMaker(line.split(';')[0],line.split(';')[1],configFile)
