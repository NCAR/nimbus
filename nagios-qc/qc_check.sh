Script started on Thu 07 Aug 2014 11:11:06 AM MDT
Disk quotas for user jquick (uid 24462): 
     Filesystem  blocks   quota   limit   grace   files   quota   limit   grace
j1.eol.ucar.edu:/vol/eolhomes
                  1722M  60416M  61440M           28262   4295m   4295m        
]0;kilojoules:/home/raf/nagios-qc26% cata[K d[Kqc_check.sh 
]0;kilojoules:/home/raf/nagios-qc27% vim qc_check.sh 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"qc_check.sh" 0L, 0C[>c[2;1H[1m[34m~                                                                                                                                                            [3;1H~                                                                                                                                                            [4;1H~                                                                                                                                                            [5;1H~                                                                                                                                                            [6;1H~                                                                                                                                                            [7;1H~                                                                                                                                                            [8;1H~                                                                                                                                                            [9;1H~                                                                                                                                                            [10;1H~                                                                                                                                                            [11;1H~                                                                                                                                                            [12;1H~                                                                                                                                                            [13;1H~                                                                                                                                                            [14;1H~                                                                                                                                                            [15;1H~                                                                                                                                                            [16;1H~                                                                                                                                                            [17;1H~                                                                                                                                                            [18;1H~                                                                                                                                                            [19;1H~                                                                                                                                                            [20;1H~                                                                                                                                                            [21;1H~                                                                                                                                                            [22;1H~                                                                                                                                                            [23;1H~                                                                                                                                                            [24;1H~                                                                                                                                                            [25;1H~                                                                                                                                                            [26;1H~                                                                                                                                                            [27;1H~                                                                                                                                                            [28;1H~                                                                                                                                                            [29;1H~                                                                                                                                                            [30;1H~                                                                                                                                                            [31;1H~                                                                                                                                                            [32;1H~                                                                                                                                                            [33;1H~                                                                                                                                                            [34;1H~                                                                                                                                                            [35;1H~                                                                                                                                                            [36;1H~                                                                                                                                                            [37;1H~                                                                                                                                                            [38;1H~                                                                                                                                                            [39;1H~                                                                                                                                                            [40;1H~                                                                                                                                                            [41;1H~                                                                                                                                                            [42;1H~                                                                                                                                                            [43;1H~                                                                                                                                                            [44;1H~                                                                                                                                                            [45;1H~                                                                                                                                                            [46;1H~                                                                                                                                                            [47;1H~                                                                                                                                                            [48;1H~                                                                                                                                                            [49;1H~                                                                                                                                                            [50;1H~                                                                                                                                                            [51;1H~                                                                                                                                                            [52;1H~                                                                                                                                                            [53;1H~                                                                                                                                                            [m[54;140H0,0-1[9CAll[1;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[54;1H[K[54;1H:[?12l[?25hq[?25l[?12l[?25h[?25l[54;1H[K[54;1H[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc28% ls
checks.xml  [0m[01;32mcompare.py[0m  [01;32mconfigMaker.py[0m  [01;34mpi[0m  [01;32mpiper.sh[0m  [01;32mqc_check.sh[0m  [01;32mregen_cfg.sh[0m  sample_checks.xml  VDB.xml
[m]0;kilojoules:/home/raf/nagios-qc29% sh regen_cfg.sh 
[sudo] password for jquick: 
rm: cannot remove `/etc/nagios/raf/*': No such file or directory
Stopping nagios: ..done.
define host{
   use                     generic-radar-host
   host_name               RAF
   alias                   RAF
   address                 0.0.0.0
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     PRES
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     PSXC
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     A2DTEMP
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     IRIG_STATUS
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     IRIG_Tdiff
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     ATX
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     DPXC
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     CAVP
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     PSFC
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     PSFD
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     QCF
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     QCR
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     TASR
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     TASF
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     LAT
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     LON
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     GGLAT
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     GGLON
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     GGQUAL
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     GGSTATUS
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     FTMP
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     FTMP3
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     CNTEMP
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     FREF
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     FREF3
}

Starting nagios: done.
]0;kilojoules:/home/raf/nagios-qc30% sh pi
pi/       piper.sh* 
]0;kilojoules:/home/raf/nagios-qc30% sh piper.sh 
]0;kilojoules:/home/raf/nagios-qc31% sh piper.sh[8Gregen_cfg.sh[5Gls[Kvim qc_check.sh[5Gcat[20G[5Gvim compare.py[K[5Gcd /home/raf/nagios-qc/[5Gvim /var/cache/foomatic/[5Glocate '' | grep foo[Kntop[14G[K | grep :[14G[K | grep ntopfoo[K[5Gvim /var/cache/foomatic/[5Gcd /home/raf/nagios-qc/[K[5Gvim compare.py[K[5Gcat qc_check.sh[5Gvim[20G[5Gls[Ksh regen_cfg.sh[8Gpiper.sh[K[5G[Ksh piper.sh[8Gregen_cfg.sh[5Gls[Kvim qc_check.sh[5Gcat[20G[5Gvim compare.py[K[5Gcat qc_check.sh[5Gvim compare.py[K[5Gcd /home/raf/nagios-qc/[5Gvim /var/cache/foomatic/[5Glocate '' | grep foo[Kntop[14G[K | grep :[12G[3P[20G[12G[3@'' [23G[14G[K | grep ntopfoo[K[5Gvim /var/cache/foomatic/[5Gcd /home/raf/nagios-qc/[K[5Gvim compare.py[K[5Gcat qc_check.sh[5Gvim[20G[5Gls[Ksh regen_cfg.sh[8Gpiper.sh[K[5G[Kcat /tmp/commands 
RAF;QCR;2;critical::**Below minimum limit** value is 61.9712
RAF;QCF;2;critical::**Below minimum limit** value is 62.0522
RAF;ATX;2;critical::**Below minimum limit** value is 20.6194
RAF;GGLON;2;**FLATLINING**
RAF;GGQUAL;1;warning:: value is 9.0
RAF;PSXC;2;critical::**Below minimum limit** value is 805.539
RAF;GGLAT;2;**FLATLINING**
RAF;DPXC;2;critical::**Below minimum limit** value is 12.2342
]0;kilojoules:/home/raf/nagios-qc32% vim c
checks.xml      compare.py*     configMaker.py* 
]0;kilojoules:/home/raf/nagios-qc32% vim co
compare.py*     configMaker.py* 
]0;kilojoules:/home/raf/nagios-qc32% vim configMaker.py 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"configMaker.py" 31L, 1056C[>c[1;1H[35mfrom[m lxml [35mimport[m etree
[33mdef[m [36mServiceMaker[m(Host,Service,conf):
   conf.write('[31mdefine service{[m[35m\n[m')
   conf.write('[31m   use                     generic-radar-service[m[35m\n[m')
   conf.write('[31m   host_name               [m'+Host+'[35m\n[m')
   conf.write('[31m   service_description     [m'+Service+'[35m\n[m')
   conf.write('[31m}[m[35m\n\n[m')

[33mdef[m [36mHostMaker[m(Host,conf):
   conf.write('[31mdefine host{[m[35m\n[m')
   conf.write('[31m   use                     generic-radar-host[m[35m\n[m')
   conf.write('[31m   host_name               [m'+Host+'[35m\n[m')
   conf.write('[31m   alias                   [m'+Host+'[35m\n[m')
   conf.write('[31m   address                 0.0.0.0[m[35m\n[m')
   conf.write('[31m}[m[35m\n\n[m')

configFile=open('[31m/etc/nagios/raf/config.cfg[m','[31mw[m')
HostName='[31mRAF[m'
HostMaker(HostName,configFile)

[34m#Using commands output file
#-------------------------
#commands=open('commands')
#lines=commands.readlines()
#for line in lines:
#   ServiceMaker(line.split(';')[0],line.split(';')[1],configFile)

#Using checks xml[m
Checks=etree.parse('[31mchecks.xml[m')
[33mfor[m Elm [33min[m Checks.getiterator('[31mcheck[m'):
  ServiceMaker(HostName,Elm.attrib['[31mvariable[m'],configFile)
[1m[34m~                                                                                                                                                            [33;1H~                                                                                                                                                            [34;1H~                                                                                                                                                            [35;1H~                                                                                                                                                            [36;1H~                                                                                                                                                            [37;1H~                                                                                                                                                            [38;1H~                                                                                                                                                            [39;1H~                                                                                                                                                            [40;1H~                                                                                                                                                            [41;1H~                                                                                                                                                            [42;1H~                                                                                                                                                            [43;1H~                                                                                                                                                            [44;1H~                                                                                                                                                            [45;1H~                                                                                                                                                            [46;1H~                                                                                                                                                            [47;1H~                                                                                                                                                            [48;1H~                                                                                                                                                            [49;1H~                                                                                                                                                            [50;1H~                                                                                                                                                            [51;1H~                                                                                                                                                            [52;1H~                                                                                                                                                            [53;1H~                                                                                                                                                            [m[54;140H1,1[11CAll[1;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[54;140H2[2;1H[?12l[?25h[?25l[54;140H3[3;1H[?12l[?25h[?25l[54;140H4[4;1H[?12l[?25h[?25l[54;140H5[5;1H[?12l[?25h[?25l[54;140H6[6;1H[?12l[?25h[?25l[54;140H7[7;1H[?12l[?25h[?25l[54;140H8,0-1[8;1H[?12l[?25h[?25l[54;140H9,1  [9;1H[?12l[?25h[?25l[54;140H10,1[10;1H[?12l[?25h[?25l[54;141H1[11;1H[?12l[?25h[?25l[54;141H2[12;1H[?12l[?25h[?25l[54;141H3[13;1H[?12l[?25h[?25l[54;141H4[14;1H[?12l[?25h[?25l[54;141H5[15;1H[?12l[?25h[?25l[54;141H6,0-1[16;1H[?12l[?25h[?25l[54;141H7,1  [17;1H[?12l[?25h[?25l[54;141H8[18;1H[?12l[?25h[?25l[54;141H9[19;1H[?12l[?25h[?25l[54;140H20,0-1[20;1H[?12l[?25h[?25l[54;141H1,1  [21;1H[?12l[?25h[?25l[54;141H2[22;1H[?12l[?25h[?25l[54;141H3[23;1H[?12l[?25h[?25l[54;141H4[24;1H[?12l[?25h[?25l[54;141H5[25;1H[?12l[?25h[?25l[54;141H6[26;1H[?12l[?25h[?25l[54;141H7,0-1[27;1H[?12l[?25h[?25l[54;141H8,1  [28;1H[?12l[?25h[?25l[54;141H9[29;1H[?12l[?25h[?25l[54;140H30[30;1H[?12l[?25h[?25l[54;141H1[31;1H[?12l[?25h[?25l[54;141H0[30;1H[?12l[?25h[?25l[54;140H29[29;1H[?12l[?25h[?25l[54;143H2[29;2H[?12l[?25h[?25l[54;143H3[29;3H[?12l[?25h[?25l[54;143H4[29;4H[?12l[?25h[?25l[54;143H5[29;5H[?12l[?25h[?25l[54;143H6[29;6H[?12l[?25h[?25l[54;143H7[29;7H[?12l[?25h[?25l[54;143H8[29;8H[?12l[?25h[?25l[54;143H9[29;9H[?12l[?25h[?25l[54;143H10[29;10H[?12l[?25h[?25l[54;144H1[29;11H[?12l[?25h[?25l[54;144H2[29;12H[?12l[?25h[?25l[54;144H3[29;13H[?12l[?25h[?25l[54;144H4[29;14H[?12l[?25h[?25l[54;144H5[29;15H[?12l[?25h[?25l[54;144H6[29;16H[?12l[?25h[?25l[54;144H7[29;17H[?12l[?25h[?25l[54;1H[K[54;1H:[?12l[?25hq[?25l[?12l[?25h[?25l[54;1H[K[54;1H[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc33% vi[K[Kvim pi
pi/       piper.sh* 
]0;kilojoules:/home/raf/nagios-qc33% vim piper.sh 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"piper.sh" 19L, 573C[>c[1;1H[34m#!/bin/sh
# This script writes passive checks to the Nagios command file to cause
# it to process service check results.  Called by qc_check.[m

[36mechocmd[m=[33m"[m[31m/bin/echo[m[33m"[m
[36mCommandFile[m=[33m"[m[31m/var/spool/nagios/cmd/nagios.cmd[m[33m"
while read line
do[m
  [34m# get the current date/time in seconds since UNIX epoch[m
  [36mdatetime[m=[35m`date +%s`[m

  [34m# create the command line to add to the command file[m
  [36mcmdline[m=[33m"[m[31m[[m[35m$datetime[m[31m] PROCESS_SERVICE_CHECK_RESULT;[m[35m$line[m[33m"[m
  [34m#echo "$cmdline"[m

  [34m# append the command to the end of the command file[m
  [35m`$echocmd $cmdline [m[33m>>[m[35m $CommandFile`[m
[34m#  echo $cmdline[m
[33mdone[m [33m<[m/tmp/commands
[1m[34m~                                                                                                                                                            [21;1H~                                                                                                                                                            [22;1H~                                                                                                                                                            [23;1H~                                                                                                                                                            [24;1H~                                                                                                                                                            [25;1H~                                                                                                                                                            [26;1H~                                                                                                                                                            [27;1H~                                                                                                                                                            [28;1H~                                                                                                                                                            [29;1H~                                                                                                                                                            [30;1H~                                                                                                                                                            [31;1H~                                                                                                                                                            [32;1H~                                                                                                                                                            [33;1H~                                                                                                                                                            [34;1H~                                                                                                                                                            [35;1H~                                                                                                                                                            [36;1H~                                                                                                                                                            [37;1H~                                                                                                                                                            [38;1H~                                                                                                                                                            [39;1H~                                                                                                                                                            [40;1H~                                                                                                                                                            [41;1H~                                                                                                                                                            [42;1H~                                                                                                                                                            [43;1H~                                                                                                                                                            [44;1H~                                                                                                                                                            [45;1H~                                                                                                                                                            [46;1H~                                                                                                                                                            [47;1H~                                                                                                                                                            [48;1H~                                                                                                                                                            [49;1H~                                                                                                                                                            [50;1H~                                                                                                                                                            [51;1H~                                                                                                                                                            [52;1H~                                                                                                                                                            [53;1H~                                                                                                                                                            [m[54;140H1,1[11CAll[1;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[54;140H2[2;1H[?12l[?25h[?25l[54;140H3[3;1H[?12l[?25h[?25l[54;140H4,0-1[4;1H[?12l[?25h[?25l[54;140H5,1  [5;1H[?12l[?25h[?25l[54;140H6[6;1H[?12l[?25h[?25l[54;140H7[7;1H[?12l[?25h[?25l[54;140H8[8;1H[?12l[?25h[?25l[54;140H9[9;1H[?12l[?25h[?25l[54;140H10,1[10;1H[?12l[?25h[?25l[54;141H1,0-1[11;1H[?12l[?25h[?25l[54;141H2,1  [12;1H[?12l[?25h[?25l[54;141H3[13;1H[?12l[?25h[?25l[54;141H4[14;1H[?12l[?25h[?25l[54;141H5,0-1[15;1H[?12l[?25h[?25l[54;141H6,1  [16;1H[?12l[?25h[?25l[54;141H7[17;1H[?12l[?25h[?25l[54;141H8[18;1H[?12l[?25h[?25l[54;141H9[19;1H[?12l[?25h[?25l[54;141H8[18;1H[?12l[?25h[?25l  [33mecho[m[31m [m[35m$cmdline[m[18;16H[K[18;1H[?12l[?25h[?25l[54;1H[K[54;1H:[?12l[?25hw[?25l[?12l[?25hq[?25l[?12l[?25h[?25l"piper.sh" 19L, 572C written
[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc34% vim piper.sh[9GconfigMaker.py[5Gcat /tmp/commands[K[5Gsh piper.sh[K
[1407431810] PROCESS_SERVICE_CHECK_RESULT;RAF;QCR;2;critical::**Below minimum limit** value is 61.9712
[1407431810] PROCESS_SERVICE_CHECK_RESULT;RAF;QCF;2;critical::**Below minimum limit** value is 62.0522
[1407431810] PROCESS_SERVICE_CHECK_RESULT;RAF;ATX;2;critical::**Below minimum limit** value is 20.6194
[1407431810] PROCESS_SERVICE_CHECK_RESULT;RAF;GGLON;2;**FLATLINING**
[1407431810] PROCESS_SERVICE_CHECK_RESULT;RAF;GGQUAL;1;warning:: value is 9.0
[1407431811] PROCESS_SERVICE_CHECK_RESULT;RAF;PSXC;2;critical::**Below minimum limit** value is 805.539
[1407431811] PROCESS_SERVICE_CHECK_RESULT;RAF;GGLAT;2;**FLATLINING**
[1407431811] PROCESS_SERVICE_CHECK_RESULT;RAF;DPXC;2;critical::**Below minimum limit** value is 12.2342
]0;kilojoules:/home/raf/nagios-qc35% sh piper.sh[5G[1@vim[17G
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"piper.sh" 19L, 572C[>c[1;1H[34m#!/bin/sh
# This script writes passive checks to the Nagios command file to cause
# it to process service check results.  Called by qc_check.[m

[36mechocmd[m=[33m"[m[31m/bin/echo[m[33m"[m
[36mCommandFile[m=[33m"[m[31m/var/spool/nagios/cmd/nagios.cmd[m[33m"
while read line
do[m
  [34m# get the current date/time in seconds since UNIX epoch[m
  [36mdatetime[m=[35m`date +%s`[m

  [34m# create the command line to add to the command file[m
  [36mcmdline[m=[33m"[m[31m[[m[35m$datetime[m[31m] PROCESS_SERVICE_CHECK_RESULT;[m[35m$line[m[33m"[m
  [34m#echo "$cmdline"[m

  [34m# append the command to the end of the command file[m
  [35m`$echocmd $cmdline [m[33m>>[m[35m $CommandFile`[m
  [33mecho[m[31m [m[35m$cmdline[m
[33mdone[m [33m<[m/tmp/commands
[1m[34m~                                                                                                                                                            [21;1H~                                                                                                                                                            [22;1H~                                                                                                                                                            [23;1H~                                                                                                                                                            [24;1H~                                                                                                                                                            [25;1H~                                                                                                                                                            [26;1H~                                                                                                                                                            [27;1H~                                                                                                                                                            [28;1H~                                                                                                                                                            [29;1H~                                                                                                                                                            [30;1H~                                                                                                                                                            [31;1H~                                                                                                                                                            [32;1H~                                                                                                                                                            [33;1H~                                                                                                                                                            [34;1H~                                                                                                                                                            [35;1H~                                                                                                                                                            [36;1H~                                                                                                                                                            [37;1H~                                                                                                                                                            [38;1H~                                                                                                                                                            [39;1H~                                                                                                                                                            [40;1H~                                                                                                                                                            [41;1H~                                                                                                                                                            [42;1H~                                                                                                                                                            [43;1H~                                                                                                                                                            [44;1H~                                                                                                                                                            [45;1H~                                                                                                                                                            [46;1H~                                                                                                                                                            [47;1H~                                                                                                                                                            [48;1H~                                                                                                                                                            [49;1H~                                                                                                                                                            [50;1H~                                                                                                                                                            [51;1H~                                                                                                                                                            [52;1H~                                                                                                                                                            [53;1H~                                                                                                                                                            [m[54;140H18,3[10CAll[18;3H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[18;53r[53;1H
[1;54r[53;1H[1m[34m~                                                                                                                                                            [m[54;1H[K[54;140H18,1[10CAll[18;1H[?12l[?25h[?25l[54;141H7[17;1H[?12l[?25h[?25l[54;141H6[16;1H[?12l[?25h[?25l[54;141H5,0-1[15;1H[?12l[?25h[?25l[54;141H4,1  [14;1H[?12l[?25h[?25l[54;141H3[13;1H[?12l[?25h[?25l[54;141H2[12;1H[?12l[?25h[?25l[54;141H1,0-1[11;1H[?12l[?25h[?25l[54;141H0,1  [10;1H[?12l[?25h[?25l[54;140H9,1 [9;1H[?12l[?25h[?25l[54;140H8[8;1H[?12l[?25h[?25l[54;140H7[7;1H[?12l[?25h[?25l[54;142H2[7;2H[?12l[?25h[?25l[54;142H3[7;3H[?12l[?25h[?25l[54;142H4[7;4H[?12l[?25h[?25l[54;142H5[7;5H[?12l[?25h[?25l[54;142H6[7;6H[?12l[?25h[?25l[54;142H7[7;7H[?12l[?25h[?25l[54;142H8[7;8H[?12l[?25h[?25l[54;142H9[7;9H[?12l[?25h[?25l[54;142H10[7;10H[?12l[?25h[?25l[54;143H1[7;11H[?12l[?25h[?25l[54;143H2[7;12H[?12l[?25h[?25l[54;143H3[7;13H[?12l[?25h[?25l[54;143H4[7;14H[?12l[?25h[?25l[54;143H5[7;15H[?12l[?25h[?25l[54;140H[K[54;1H:[?12l[?25hq[?25l[?12l[?25h[?25l[1m[37m[41mE37: No write since last change (add ! to override)[m[88C7,15[10CAll[7;15H[?12l[?25h[?25l[54;1H[K[54;1H:[?12l[?25hq[?25l[?12l[?25h![?25l[?12l[?25h[?25l[54;1H[K[54;1H[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc36% vim piper.sh
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"piper.sh" 19L, 572C[>c[1;1H[34m#!/bin/sh
# This script writes passive checks to the Nagios command file to cause
# it to process service check results.  Called by qc_check.[m

[36mechocmd[m=[33m"[m[31m/bin/echo[m[33m"[m
[36mCommandFile[m=[33m"[m[31m/var/spool/nagios/cmd/nagios.cmd[m[33m"
while read line
do[m
  [34m# get the current date/time in seconds since UNIX epoch[m
  [36mdatetime[m=[35m`date +%s`[m

  [34m# create the command line to add to the command file[m
  [36mcmdline[m=[33m"[m[31m[[m[35m$datetime[m[31m] PROCESS_SERVICE_CHECK_RESULT;[m[35m$line[m[33m"[m
  [34m#echo "$cmdline"[m

  [34m# append the command to the end of the command file[m
  [35m`$echocmd $cmdline [m[33m>>[m[35m $CommandFile`[m
  [33mecho[m[31m [m[35m$cmdline[m
[33mdone[m [33m<[m/tmp/commands
[1m[34m~                                                                                                                                                            [21;1H~                                                                                                                                                            [22;1H~                                                                                                                                                            [23;1H~                                                                                                                                                            [24;1H~                                                                                                                                                            [25;1H~                                                                                                                                                            [26;1H~                                                                                                                                                            [27;1H~                                                                                                                                                            [28;1H~                                                                                                                                                            [29;1H~                                                                                                                                                            [30;1H~                                                                                                                                                            [31;1H~                                                                                                                                                            [32;1H~                                                                                                                                                            [33;1H~                                                                                                                                                            [34;1H~                                                                                                                                                            [35;1H~                                                                                                                                                            [36;1H~                                                                                                                                                            [37;1H~                                                                                                                                                            [38;1H~                                                                                                                                                            [39;1H~                                                                                                                                                            [40;1H~                                                                                                                                                            [41;1H~                                                                                                                                                            [42;1H~                                                                                                                                                            [43;1H~                                                                                                                                                            [44;1H~                                                                                                                                                            [45;1H~                                                                                                                                                            [46;1H~                                                                                                                                                            [47;1H~                                                                                                                                                            [48;1H~                                                                                                                                                            [49;1H~                                                                                                                                                            [50;1H~                                                                                                                                                            [51;1H~                                                                                                                                                            [52;1H~                                                                                                                                                            [53;1H~                                                                                                                                                            [m[54;140H7,1[11CAll[7;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[54;140H8[8;1H[?12l[?25h[?25l[54;140H9[9;1H[?12l[?25h[?25l[54;140H10,1[10;1H[?12l[?25h[?25l[54;141H1,0-1[11;1H[?12l[?25h[?25l[54;141H2,1  [12;1H[?12l[?25h[?25l[54;141H3[13;1H[?12l[?25h[?25l[54;141H4[14;1H[?12l[?25h[?25l[54;141H5,0-1[15;1H[?12l[?25h[?25l[54;141H6,1  [16;1H[?12l[?25h[?25l[54;141H7[17;1H[?12l[?25h[?25l[54;141H8[18;1H[?12l[?25h[?25l[54;1H[1m-- INSERT --[m[54;13H[K[54;140H18,1[10CAll[18;1H[?12l[?25h[?25l[34m#  echo $cmdline[m[54;143H2[18;2H[?12l[?25h[54;1H[K[18;1H[?25l[54;140H18,1[10CAll[18;1H[?12l[?25h[?25l[54;140H[K[54;1H:[?12l[?25hw[?25l[?12l[?25hq[?25l[?12l[?25h[?25l"piper.sh" 19L, 573C written
[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc37% vim piper.sh[5G[Psh[16G
]0;kilojoules:/home/raf/nagios-qc38% /sbin/service nagios stop
Stopping nagios: /etc/init.d/nagios: line 74: kill: (4713) - Operation not permitted
..........
Warning - nagios did not exit in a timely manner
rm: cannot remove `/var/log/nagios/status.dat': Permission denied
rm: cannot remove `/var/run/nagios.pid': Permission denied
rm: cannot remove `/var/lock/subsys/nagios': Permission denied
]0;kilojoules:/home/raf/nagios-qc39% /sbin/service nagios stop[5G[1@s[1@u[1@d[1@o[1@ 
[sudo] password for jquick: 
Stopping nagios: ..done.
]0;kilojoules:/home/raf/nagios-qc40% sudo /sbin/service nagios stop[K[Kart
Starting nagios: done.
]0;kilojoules:/home/raf/nagios-qc41% rm /etc/nagios/raf/*
rm: remove write-protected regular file `/etc/nagios/raf/config.cfg'? y
rm: cannot remove `/etc/nagios/raf/config.cfg': Permission denied
]0;kilojoules:/home/raf/nagios-qc42% y[Krm /etc/nagios/raf/*[5G[1@s[1@u[1@d[1@o[1@ 
]0;kilojoules:/home/raf/nagios-qc43% sudo rm /etc/nagios/raf/*[5Gsudo[P[P[P[P rm[P[P[P[1@l[1@s /etc/nagios/raf/*[K
]0;kilojoules:/home/raf/nagios-qc44% ls /etc/nagios/raf/[5Gsudo rm /etc/nagios/raf/*[5G[5P[25G[5Gsudo /sbin/service nagios start[1@r[1@e
Running configuration check...done.
Stopping nagios: .done.
Starting nagios: done.
]0;kilojoules:/home/raf/nagios-qc45% sudo /sbin/service nagios restart[5Gls /etc/nagios/raf/[K[5Gsudo rm /etc/nagios/raf/*[5Gls /etc/nagios/raf/[K[5Gsudo /sbin/service nagios restart[5G[Kvim configMaker.py 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"configMaker.py" 31L, 1056C[>c[1;1H[35mfrom[m lxml [35mimport[m etree
[33mdef[m [36mServiceMaker[m(Host,Service,conf):
   conf.write('[31mdefine service{[m[35m\n[m')
   conf.write('[31m   use                     generic-radar-service[m[35m\n[m')
   conf.write('[31m   host_name               [m'+Host+'[35m\n[m')
   conf.write('[31m   service_description     [m'+Service+'[35m\n[m')
   conf.write('[31m}[m[35m\n\n[m')

[33mdef[m [36mHostMaker[m(Host,conf):
   conf.write('[31mdefine host{[m[35m\n[m')
   conf.write('[31m   use                     generic-radar-host[m[35m\n[m')
   conf.write('[31m   host_name               [m'+Host+'[35m\n[m')
   conf.write('[31m   alias                   [m'+Host+'[35m\n[m')
   conf.write('[31m   address                 0.0.0.0[m[35m\n[m')
   conf.write('[31m}[m[35m\n\n[m')

configFile=open('[31m/etc/nagios/raf/config.cfg[m','[31mw[m')
HostName='[31mRAF[m'
HostMaker(HostName,configFile)

[34m#Using commands output file
#-------------------------
#commands=open('commands')
#lines=commands.readlines()
#for line in lines:
#   ServiceMaker(line.split(';')[0],line.split(';')[1],configFile)

#Using checks xml[m
Checks=etree.parse('[31mchecks.xml[m')
[33mfor[m Elm [33min[m Checks.getiterator('[31mcheck[m'):
  ServiceMaker(HostName,Elm.attrib['[31mvariable[m'],configFile)
[1m[34m~                                                                                                                                                            [33;1H~                                                                                                                                                            [34;1H~                                                                                                                                                            [35;1H~                                                                                                                                                            [36;1H~                                                                                                                                                            [37;1H~                                                                                                                                                            [38;1H~                                                                                                                                                            [39;1H~                                                                                                                                                            [40;1H~                                                                                                                                                            [41;1H~                                                                                                                                                            [42;1H~                                                                                                                                                            [43;1H~                                                                                                                                                            [44;1H~                                                                                                                                                            [45;1H~                                                                                                                                                            [46;1H~                                                                                                                                                            [47;1H~                                                                                                                                                            [48;1H~                                                                                                                                                            [49;1H~                                                                                                                                                            [50;1H~                                                                                                                                                            [51;1H~                                                                                                                                                            [52;1H~                                                                                                                                                            [53;1H~                                                                                                                                                            [m[54;140H29,1[10CAll[29;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[54;140H30[30;1H[?12l[?25h[?25l[54;140H29[29;1H[?12l[?25h[?25l[54;141H8[28;1H[?12l[?25h[?25l[54;141H7,0-1[27;1H[?12l[?25h[?25l[54;141H6,1  [26;1H[?12l[?25h[?25l[54;141H5[25;1H[?12l[?25h[?25l[54;141H4[24;1H[?12l[?25h[?25l[54;141H3[23;1H[?12l[?25h[?25l[54;141H2[22;1H[?12l[?25h[?25l[54;141H1[21;1H[?12l[?25h[?25l[54;141H0,0-1[20;1H[?12l[?25h[?25l[54;140H19,1  [19;1H[?12l[?25h[?25l[54;141H8[18;1H[?12l[?25h[?25l[54;143H2[18;2H[?12l[?25h[?25l[54;143H3[18;3H[?12l[?25h[?25l[54;143H4[18;4H[?12l[?25h[?25l[54;143H5[18;5H[?12l[?25h[?25l[54;143H6[18;6H[?12l[?25h[?25l[54;143H7[18;7H[?12l[?25h[?25l[54;143H8[18;8H[?12l[?25h[?25l[54;143H9[18;9H[?12l[?25h[?25l[54;143H10[18;10H[?12l[?25h[?25l[54;144H1[18;11H[?12l[?25h[?25l[54;144H2[18;12H[?12l[?25h[?25l[54;144H3[18;13H[?12l[?25h[?25l[54;1H[1m-- INSERT --[m[54;13H[K[54;140H18,14[9CAll[18;14H[?12l[?25h[?25l[31mA[m'[18;14H[K[54;144H3[18;13H[?12l[?25h[?25l[31mR[m'[18;13H[K[54;144H2[18;12H[?12l[?25h[?25l''[18;12H[K[54;144H1[18;11H[?12l[?25h[?25l'[31mA[m'[54;144H2[18;12H[?12l[?25h[?25l[31mAi[m'[54;144H3[18;13H[?12l[?25h[?25l[31mir[m'[54;144H4[18;14H[?12l[?25h[54;1H[K[18;13H[?25l[54;140H18,13[9CAll[18;13H[?12l[?25h[?25l[54;140H[K[54;1H:[?12l[?25hw[?25l[?12l[?25hq[?25l[?12l[?25h[?25l"configMaker.py" 31L, 1056C written
[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc46% vim configMaker.py[5Gsudo /sbin/service nagios restart[5Gls /etc/nagios/raf/[K[5Gsudo rm /etc/nagios/raf/*[5G[5P[25G[5Gsudo /sbin/service nagios startop[K[5G[5P[30G[5Gsh piper.sh[K[5G[1@vim[17G[5Gcat /tmp/commands[5Gsh regen_cfg.sh[K[5Gls[Kvim qc_check.sh[5Gls[Ksh regen_cfg.sh
rm: cannot remove `/etc/nagios/raf/*': No such file or directory
Stopping nagios: .done.
define host{
   use                     generic-radar-host
   host_name               Air
   alias                   Air
   address                 0.0.0.0
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     PRES
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     PSXC
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     A2DTEMP
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     IRIG_STATUS
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     IRIG_Tdiff
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     ATX
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     DPXC
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     CAVP
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     PSFC
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     PSFD
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     QCF
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     QCR
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     TASR
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     TASF
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     LAT
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     LON
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     GGLAT
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     GGLON
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     GGQUAL
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     GGSTATUS
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     FTMP
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     FTMP3
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     CNTEMP
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     FREF
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     FREF3
}

Starting nagios: done.
]0;kilojoules:/home/raf/nagios-qc47% sh regen_cfg.sh[5Gvim configMaker.py[5Gsudo /sbin/service nagios restart[5Gvim configMaker.py[K[5Gsudo /sbin/service nagios restart[5Gls /etc/nagios/raf/[K[5Gsudo rm /etc/nagios/raf/*[5G[5P[25G[5Gsudo /sbin/service nagios startop[Kart[5Grm /etc/nagios/raf/*[K[5G[5@sudo [30G[5Gls /etc/nagios/raf/[K[5Gsudo /sbin/service nagios restart[5Gvim configMaker.py[K[5Gsh regen_cfg.sh[K[5G[Kvim pi
pi/       piper.sh* 
]0;kilojoules:/home/raf/nagios-qc47% vim piper.sh 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"piper.sh" 19L, 573C[>c[1;1H[34m#!/bin/sh
# This script writes passive checks to the Nagios command file to cause
# it to process service check results.  Called by qc_check.[m

[36mechocmd[m=[33m"[m[31m/bin/echo[m[33m"[m
[36mCommandFile[m=[33m"[m[31m/var/spool/nagios/cmd/nagios.cmd[m[33m"
while read line
do[m
  [34m# get the current date/time in seconds since UNIX epoch[m
  [36mdatetime[m=[35m`date +%s`[m

  [34m# create the command line to add to the command file[m
  [36mcmdline[m=[33m"[m[31m[[m[35m$datetime[m[31m] PROCESS_SERVICE_CHECK_RESULT;[m[35m$line[m[33m"[m
  [34m#echo "$cmdline"[m

  [34m# append the command to the end of the command file[m
  [35m`$echocmd $cmdline [m[33m>>[m[35m $CommandFile`[m
[34m#  echo $cmdline[m
[33mdone[m [33m<[m/tmp/commands
[1m[34m~                                                                                                                                                            [21;1H~                                                                                                                                                            [22;1H~                                                                                                                                                            [23;1H~                                                                                                                                                            [24;1H~                                                                                                                                                            [25;1H~                                                                                                                                                            [26;1H~                                                                                                                                                            [27;1H~                                                                                                                                                            [28;1H~                                                                                                                                                            [29;1H~                                                                                                                                                            [30;1H~                                                                                                                                                            [31;1H~                                                                                                                                                            [32;1H~                                                                                                                                                            [33;1H~                                                                                                                                                            [34;1H~                                                                                                                                                            [35;1H~                                                                                                                                                            [36;1H~                                                                                                                                                            [37;1H~                                                                                                                                                            [38;1H~                                                                                                                                                            [39;1H~                                                                                                                                                            [40;1H~                                                                                                                                                            [41;1H~                                                                                                                                                            [42;1H~                                                                                                                                                            [43;1H~                                                                                                                                                            [44;1H~                                                                                                                                                            [45;1H~                                                                                                                                                            [46;1H~                                                                                                                                                            [47;1H~                                                                                                                                                            [48;1H~                                                                                                                                                            [49;1H~                                                                                                                                                            [50;1H~                                                                                                                                                            [51;1H~                                                                                                                                                            [52;1H~                                                                                                                                                            [53;1H~                                                                                                                                                            [m[54;140H18,1[10CAll[18;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[54;1H[K[54;1H:[?12l[?25hq[?25l[?12l[?25h[?25l[54;1H[K[54;1H[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc48% vim compare.py 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1HE325: ATTENTION
Found a swap file by the name ".compare.py.swp"
[10Cowned by: jquick   dated: Thu Aug  7 11:10:03 2014
[9Cfile name: /home/raf/nagios-qc/compare.py
[10Cmodified: no
[9Cuser name: jquick   host name: kilojoules
[8Cprocess ID: 1057 (still running)
While opening file "compare.py"
[13Cdated: Thu Aug  7 11:09:59 2014

(1) Another program may be editing the same file.
    If this is the case, be careful not to end up with two
    different instances of the same file when making changes.
    Quit, or continue with caution.

(2) An edit session for this file crashed.
    If this is the case, use ":recover" or "vim -r compare.py"
    to recover the changes (see ":help recovery").
    If you did this already, delete the swap file ".compare.py.swp"
    to avoid this message.

Swap file ".compare.py.swp" already exists!
[O]pen Read-Only, (E)dit anyway, (R)ecover, (Q)uit, (A)bort:[?12l[?25h[54;1H[K[54;1H
[?1l>[?1049l]0;kilojoules:/home/raf/nagios-qc49% 
]0;kilojoules:/home/raf/nagios-qc49% 
]0;kilojoules:/home/raf/nagios-qc49% 
]0;kilojoules:/home/raf/nagios-qc49% vim compare.py[9Gpiper.sh[K[5Gsh regen_cfg.sh[5Gvim piper.sh[K[5Gsh regen_cfg.sh[5Gvim piper.sh[K[9Gcompare.py[5G[Ko[Kpython compare.py 
]0;kilojoules:/home/raf/nagios-qc50% sh piper.sh 
]0;kilojoules:/home/raf/nagios-qc51% sh piper.sh[5Gpython compare.py[5G[3Pvim[19G[9Gpiper.sh[K[5Gsh regen_cfg.sh[5Gvim configMaker.py[5Gsudo /sbin/service nagios restart[5Gls /etc/nagios/raf/[K[5Gsudo /sbin/service nagios restart
Running configuration check...done.
Stopping nagios: .done.
Starting nagios: done.
]0;kilojoules:/home/raf/nagios-qc52% sudo /sbin/service nagios restart[6Gh piper.sh[K
]0;kilojoules:/home/raf/nagios-qc53% sh piper.sh[5G[Ksh piper.sh
]0;kilojoules:/home/raf/nagios-qc54% cat /etc/nagios/ra
radar/ raf/   
]0;kilojoules:/home/raf/nagios-qc54% cat /etc/nagios/radar/
cat: /etc/nagios/radar/: Is a directory
]0;kilojoules:/home/raf/nagios-qc55% cat /etc/nagios/radar/*

define host{
   use                     generic-radar-host
   host_name               RAF
   alias                   RAF
   address                 0.0.0.0
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     wic
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     theta
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     ggalt
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     qcr
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     tasx
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     atx
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     psxc
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     ggnsat
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     dpxc
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     psf
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     thdg
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     wi_gp
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     qcf
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     gglat
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     gsf
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     cnts
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     uxc
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     rice
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     gglon
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     pdumppr
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     paltf
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     bdifr
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     wsc
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     vyc
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     pcn
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     wdc
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     adifr
}
]0;kilojoules:/home/raf/nagios-qc56% rm /etc/nagios/radar/*
rm: cannot remove `/etc/nagios/radar/NCAR_hosts_1406745631.cfg': Permission denied
rm: cannot remove `/etc/nagios/radar/NCAR_hosts_1406745632.cfg': Permission denied
rm: cannot remove `/etc/nagios/radar/NCAR_hosts_1406745633.cfg': Permission denied
rm: cannot remove `/etc/nagios/radar/NCAR_services_1406745631.cfg': Permission denied
rm: cannot remove `/etc/nagios/radar/NCAR_services_1406745632.cfg': Permission denied
rm: cannot remove `/etc/nagios/radar/NCAR_services_1406745633.cfg': Permission denied
]0;kilojoules:/home/raf/nagios-qc57% rm /etc/nagios/radar/*[5G[1@s[1@u[1@d[1@o[1@ 
]0;kilojoules:/home/raf/nagios-qc58% sudo rm /etc/nagios/radar/*[5G[5P[27G[5G[1@cat[28G[K*[5G[Prm[27G[5G[5@sudo [32G[5G[Ksudo rm /etc/nagios/radar/*[5G[5P[27G[5G[1@cat[28G/*[K/*[K/*[K/*[K/*[Kr/*a/*f/*
define host{
   use                     generic-radar-host
   host_name               Air
   alias                   Air
   address                 0.0.0.0
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     PRES
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     PSXC
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     A2DTEMP
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     IRIG_STATUS
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     IRIG_Tdiff
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     ATX
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     DPXC
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     CAVP
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     PSFC
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     PSFD
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     QCF
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     QCR
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     TASR
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     TASF
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     LAT
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     LON
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     GGLAT
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     GGLON
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     GGQUAL
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     GGSTATUS
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     FTMP
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     FTMP3
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     CNTEMP
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     FREF
}

define service{
   use                     generic-radar-service
   host_name               Air
   service_description     FREF3
}

]0;kilojoules:/home/raf/nagios-qc59% cat /etc/nagios/raf/*[5Gsudo rm /etc/nagios/radar/*[5G[5P[27G[5G[5@sudo [32G[5Gcat /etc/nagios/raf/*[K[5G[Kvim /etc/nagios/na
nagios.cfg          nagios.cfg.ipjl     nagios.cfg.rpmnew   nagios.cfg.rpmsave  nagios.cfg.works    nagios.conf         nagios.orig/
]0;kilojoules:/home/raf/nagios-qc59% vim /etc/nagios/nagios.c
nagios.cfg          nagios.cfg.ipjl     nagios.cfg.rpmnew   nagios.cfg.rpmsave  nagios.cfg.works    nagios.conf         
]0;kilojoules:/home/raf/nagios-qc59% vim /etc/nagios/nagios.cf
nagios.cfg          nagios.cfg.ipjl     nagios.cfg.rpmnew   nagios.cfg.rpmsave  nagios.cfg.works    
]0;kilojoules:/home/raf/nagios-qc59% vim /etc/nagios/nagios.cfg
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"/etc/nagios/nagios.cfg" [readonly] 1328L, 43801C[>c[1;1H[34m##############################################################################
#
# NAGIOS.CFG - Sample Main Config File for Nagios 3.2.3
#
# Read the documentation for more information on this configuration
# file.  I've provided some comments here, but things may not be so
# clear without further explanation.
#
# Last Modified: 12-14-2008
#
##############################################################################


# LOG FILE
# This is the main log file where service and host events are logged
# for historical purposes.  This should be the first option specified 
# in the config file!!![m

[33mlog_file[m=[31m/var/log/nagios/nagios.log[m[23;1H[34m# OBJECT CONFIGURATION FILE(S)
# These are the object configuration files in which you define hosts,
# host groups, contacts, contact groups, services, etc.
# You can split your object definitions across several config files
# if you wish (as shown below), or keep them all in a single config file.

# You can specify individual object config files as shown below:[m
[33mcfg_file[m=[31m/etc/nagios/objects/commands.cfg[m
[33mcfg_file[m=[31m/etc/nagios/objects/raf_commands.cfg[m
[33mcfg_file[m=[31m/etc/nagios/objects/contacts.cfg[m
[33mcfg_file[m=[31m/etc/nagios/objects/timeperiods.cfg[m
[33mcfg_file[m=[31m/etc/nagios/objects/templates.cfg[m

[34m# Definitions for monitoring the local (Linux) host[m
[33mcfg_file[m=[31m/etc/nagios/objects/raf_localhost.cfg[m

[34m# Definitions for monitoring a Windows machine
#cfg_file=/etc/nagios/objects/windows.cfg

# Definitions for monitoring a router/switch
#cfg_file=/etc/nagios/objects/switch.cfg

# Definitions for monitoring a network printer
#cfg_file=/etc/nagios/objects/printer.cfg


# You can also tell Nagios to process all config files (with a .cfg
# extension) in a particular directory by using the cfg_dir
# directive as shown below:[m

[33mcfg_dir[m=[31m/etc/nagios/radar[m[54;140H1,1[11CTop[1;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[54;140H2[2;1H[?12l[?25h[?25l[54;140H3[3;1H[?12l[?25h[?25l[54;140H4[4;1H[?12l[?25h[?25l[54;140H5[5;1H[?12l[?25h[?25l[54;140H6[6;1H[?12l[?25h[?25l[54;140H7[7;1H[?12l[?25h[?25l[54;140H8[8;1H[?12l[?25h[?25l[54;140H9[9;1H[?12l[?25h[?25l[54;140H10,1[10;1H[?12l[?25h[?25l[54;141H1[11;1H[?12l[?25h[?25l[54;141H2,0-1[12;1H[?12l[?25h[?25l[54;141H3[13;1H[?12l[?25h[?25l[54;141H4,1  [14;1H[?12l[?25h[?25l[54;141H5[15;1H[?12l[?25h[?25l[54;141H6[16;1H[?12l[?25h[?25l[54;141H7[17;1H[?12l[?25h[?25l[54;141H8,0-1[18;1H[?12l[?25h[?25l[54;141H9,1  [19;1H[?12l[?25h[?25l[54;140H20,0-1[20;1H[?12l[?25h[?25l[54;141H1[21;1H[?12l[?25h[?25l[54;141H2[22;1H[?12l[?25h[?25l[54;141H3,1  [23;1H[?12l[?25h[?25l[54;141H4[24;1H[?12l[?25h[?25l[54;141H5[25;1H[?12l[?25h[?25l[54;141H6[26;1H[?12l[?25h[?25l[54;141H7[27;1H[?12l[?25h[?25l[54;141H8,0-1[28;1H[?12l[?25h[?25l[54;141H9,1  [29;1H[?12l[?25h[?25l[54;140H30[30;1H[?12l[?25h[?25l[54;141H1[31;1H[?12l[?25h[?25l[54;141H2[32;1H[?12l[?25h[?25l[54;141H3[33;1H[?12l[?25h[?25l[54;141H4[34;1H[?12l[?25h[?25l[54;141H5,0-1[35;1H[?12l[?25h[?25l[54;141H6,1  [36;1H[?12l[?25h[?25l[54;141H7[37;1H[?12l[?25h[?25l[54;141H8,0-1[38;1H[?12l[?25h[?25l[54;141H9,1  [39;1H[?12l[?25h[?25l[54;140H40[40;1H[?12l[?25h[?25l[54;141H1,0-1[41;1H[?12l[?25h[?25l[54;141H2,1  [42;1H[?12l[?25h[?25l[54;141H3[43;1H[?12l[?25h[?25l[54;141H4,0-1[44;1H[?12l[?25h[?25l[54;141H5,1  [45;1H[?12l[?25h[?25l[54;141H6[46;1H[?12l[?25h[?25l[54;141H7,0-1[47;1H[?12l[?25h[?25l[54;141H8[48;1H[?12l[?25h[?25l[54;141H9,1  [49;1H[?12l[?25h[?25l[54;140H50[50;1H[?12l[?25h[?25l[54;141H1[51;1H[?12l[?25h[?25l[54;141H2,0-1[52;1H[?12l[?25h[?25l[54;141H3,1  [53;1H[?12l[?25h[?25l[1;53r[53;1H
[1;54r[53;1H[34m#cfg_dir=/etc/nagios/printers[m[54;1H[K[54;140H54,1[11C0%[53;1H[?12l[?25h[?25l[1;53r[53;1H
[1;54r[53;1H[34m#cfg_dir=/etc/nagios/switches[m[54;140H[K[54;140H55,1[11C0%[53;1H[?12l[?25h[?25l[1;53r[53;1H
[1;54r[53;1H[34m#cfg_dir=/etc/nagios/routers[m[54;140H[K[54;140H56,1[11C0%[53;1H[?12l[?25h[?25l[54;141H5[52;1H[?12l[?25h[?25l[54;141H4[51;1H[?12l[?25h[?25l[54;141H3[50;1H[?12l[?25h[?25l[54;143H8[50;8H[?12l[?25h[?25l[54;143H10[50;10H[?12l[?25h[?25l[54;144H3[50;13H[?12l[?25h[?25l[54;144H4[50;14H[?12l[?25h[?25l[54;143H20[50;20H[?12l[?25h[?25l[54;144H1[50;21H[?12l[?25h[?25l[54;144H2[50;22H[?12l[?25h[?25l[54;144H3[50;23H[?12l[?25h[?25l[54;1H[31mW10: Warning: Changing a readonly file[m[54;140H[K[54;1H[1m-- INSERT --[m[54;13H[K[54;140H53,23[10C0%[50;22H[31maf[m[50;24H[K[54;144H4[50;24H[?12l[?25h[54;1H[K[50;23H[?25l[54;140H53,23[10C0%[50;23H[?12l[?25h[?25l[54;140H[K[54;1H:[?12l[?25hw[?25l[?12l[?25hq[?25l[?12l[?25h[?25l[1m[37m[41mE45: 'readonly' option is set (add ! to override)[m[90C53,23[10C0%[50;23H[?12l[?25h[?25l[54;1H[K[54;1H:[?12l[?25hw[?25l[?12l[?25hq[?25l[?12l[?25h[?25l[1m[37m[41mE45: 'readonly' option is set (add ! to override)[m[90C53,23[10C0%[50;23H[?12l[?25h[?25l[54;1H[K[54;1H:[?12l[?25hw[?25l[?12l[?25hq[?25l[?12l[?25h![?25l[?12l[?25h[?25l"/etc/nagios/nagios.cfg"
[1m[37m[41m"/etc/nagios/nagios.cfg" E212: Can't open file for writing[m
[32mPress ENTER or type command to continue[?12l[?25h[?25l[m[54;1H[K[54;1H:[?12l[?25hq[?25l[?12l[?25h![?25l[?12l[?25h[?25l[54;1H[K[54;1H[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc60% vim /etc/nagios/nagios.cfg[5G[1@s[1@u[1@d[1@o[1@ 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"/etc/nagios/nagios.cfg" 1328L, 43801C[>c[2;1H[34m# OBJECT CONFIGURATION FILE(S)
# These are the object configuration files in which you define hosts,
# host groups, contacts, contact groups, services, etc.
# You can split your object definitions across several config files
# if you wish (as shown below), or keep them all in a single config file.

# You can specify individual object config files as shown below:[m
[33mcfg_file[m=[31m/etc/nagios/objects/commands.cfg[m
[33mcfg_file[m=[31m/etc/nagios/objects/raf_commands.cfg[m
[33mcfg_file[m=[31m/etc/nagios/objects/contacts.cfg[m
[33mcfg_file[m=[31m/etc/nagios/objects/timeperiods.cfg[m
[33mcfg_file[m=[31m/etc/nagios/objects/templates.cfg[m

[34m# Definitions for monitoring the local (Linux) host[m
[33mcfg_file[m=[31m/etc/nagios/objects/raf_localhost.cfg[m

[34m# Definitions for monitoring a Windows machine
#cfg_file=/etc/nagios/objects/windows.cfg

# Definitions for monitoring a router/switch
#cfg_file=/etc/nagios/objects/switch.cfg

# Definitions for monitoring a network printer
#cfg_file=/etc/nagios/objects/printer.cfg


# You can also tell Nagios to process all config files (with a .cfg
# extension) in a particular directory by using the cfg_dir
# directive as shown below:[m

[33mcfg_dir[m=[31m/etc/nagios/radar[m
[34m#cfg_dir=/etc/nagios/printers
#cfg_dir=/etc/nagios/switches
#cfg_dir=/etc/nagios/routers[40;1H# OBJECT CACHE FILE
# This option determines where object definitions are cached when
# Nagios starts/restarts.  The CGIs read object definitions from 
# this cache file (rather than looking at the object config files
# directly) in order to prevent inconsistencies that can occur
# when the config files are modified after Nagios starts.[m

[33mobject_cache_file[m=[31m/var/log/nagios/objects.cache[m[51;1H[34m# PRE-CACHED OBJECT FILE
# This options determines the location of the precached object file.
# If you run Nagios with the -p command line option, it will preprocess[m[54;140H74,1[11C1%[53;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[1;53r[53;1H
[1;54r[53;1H[34m# your object configuration file(s) and write the cached config to this[m[54;1H[K[54;140H75,1[11C1%[53;1H[?12l[?25h[?25l[54;141H4[52;1H[?12l[?25h[?25l[54;141H3[51;1H[?12l[?25h[?25l[54;141H2[50;1H[?12l[?25h[?25l[54;141H1,0-1[49;1H[?12l[?25h[?25l[54;141H0[48;1H[?12l[?25h[?25l[54;140H69[47;1H[?12l[?25h[?25l[54;141H8,1  [46;1H[?12l[?25h[?25l[54;141H7,0-1[45;1H[?12l[?25h[?25l[54;141H6,1  [44;1H[?12l[?25h[?25l[54;141H5[43;1H[?12l[?25h[?25l[54;141H4[42;1H[?12l[?25h[?25l[54;141H3[41;1H[?12l[?25h[?25l[54;141H2[40;1H[?12l[?25h[?25l[54;141H1[39;1H[?12l[?25h[?25l[54;141H0,0-1[38;1H[?12l[?25h[?25l[54;140H59[37;1H[?12l[?25h[?25l[54;141H8[36;1H[?12l[?25h[?25l[54;141H7[35;1H[?12l[?25h[?25l[54;141H6,1  [34;1H[?12l[?25h[?25l[54;141H5[33;1H[?12l[?25h[?25l[54;141H4[32;1H[?12l[?25h[?25l[54;141H3,21[31;21H[?12l[?25h[?25l[54;144H2[31;22H[?12l[?25h[?25l[54;144H3[31;23H[?12l[?25h[?25l[54;1H[1m-- INSERT --[m[54;140H[K[54;140H53,23[10C1%[31;23H[K[31;23H[?12l[?25h[?25l[31maf[m[54;144H4[31;24H[?12l[?25h[54;1H[K[31;23H[?25l[54;140H53,23[10C1%[31;23H[?12l[?25h[?25l[54;140H[K[54;1H:[?12l[?25hw[?25l[?12l[?25hq[?25l[?12l[?25h[?25l"/etc/nagios/nagios.cfg" 1328L, 43799C written
[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc61% sudo vim /etc/nagios/nagios.cfg[5G[5P[31G[5Gcat /etc/nagios/raf/*[K[5Gsudo rm /etc/nagios/radar/*[5G[5P[27G[5G[1@cat[28G[5G[Prm[27G[5G[5@sudo [32G[5G[5P[27G[5G[1@cat[28G[K[5Gsh piper.sh[K[6Gudo /sbin/service nagios restart
Running configuration check...done.
Stopping nagios: ..done.
Starting nagios: done.
]0;kilojoules:/home/raf/nagios-qc62% sudo /sbin/service nagios restart[10Gvim /etc/nagios/nagios.cfg[K[5G[5P[31G[5Gcat /etc/nagios/raf/*[K[5Gsudo rm /etc/nagios/radar/*[5G[5P[27G[5G[1@cat[28G[K[5Gsh piper.sh[K
]0;kilojoules:/home/raf/nagios-qc63% vim pi
pi/       piper.sh* 
]0;kilojoules:/home/raf/nagios-qc63% vim pip[K[K[KconfigMaker.py 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1H"configMaker.py" 31L, 1056C[>c[1;1H[35mfrom[m lxml [35mimport[m etree
[33mdef[m [36mServiceMaker[m(Host,Service,conf):
   conf.write('[31mdefine service{[m[35m\n[m')
   conf.write('[31m   use                     generic-radar-service[m[35m\n[m')
   conf.write('[31m   host_name               [m'+Host+'[35m\n[m')
   conf.write('[31m   service_description     [m'+Service+'[35m\n[m')
   conf.write('[31m}[m[35m\n\n[m')

[33mdef[m [36mHostMaker[m(Host,conf):
   conf.write('[31mdefine host{[m[35m\n[m')
   conf.write('[31m   use                     generic-radar-host[m[35m\n[m')
   conf.write('[31m   host_name               [m'+Host+'[35m\n[m')
   conf.write('[31m   alias                   [m'+Host+'[35m\n[m')
   conf.write('[31m   address                 0.0.0.0[m[35m\n[m')
   conf.write('[31m}[m[35m\n\n[m')

configFile=open('[31m/etc/nagios/raf/config.cfg[m','[31mw[m')
HostName='[31mAir[m'
HostMaker(HostName,configFile)

[34m#Using commands output file
#-------------------------
#commands=open('commands')
#lines=commands.readlines()
#for line in lines:
#   ServiceMaker(line.split(';')[0],line.split(';')[1],configFile)

#Using checks xml[m
Checks=etree.parse('[31mchecks.xml[m')
[33mfor[m Elm [33min[m Checks.getiterator('[31mcheck[m'):
  ServiceMaker(HostName,Elm.attrib['[31mvariable[m'],configFile)
[1m[34m~                                                                                                                                                            [33;1H~                                                                                                                                                            [34;1H~                                                                                                                                                            [35;1H~                                                                                                                                                            [36;1H~                                                                                                                                                            [37;1H~                                                                                                                                                            [38;1H~                                                                                                                                                            [39;1H~                                                                                                                                                            [40;1H~                                                                                                                                                            [41;1H~                                                                                                                                                            [42;1H~                                                                                                                                                            [43;1H~                                                                                                                                                            [44;1H~                                                                                                                                                            [45;1H~                                                                                                                                                            [46;1H~                                                                                                                                                            [47;1H~                                                                                                                                                            [48;1H~                                                                                                                                                            [49;1H~                                                                                                                                                            [50;1H~                                                                                                                                                            [51;1H~                                                                                                                                                            [52;1H~                                                                                                                                                            [53;1H~                                                                                                                                                            [m[54;140H18,1[10CAll[18;1H[?12l[?25hP+q436f\P+q6b75\P+q6b64\P+q6b72\P+q6b6c\P+q2332\P+q2334\P+q2569\P+q2a37\P+q6b31\[?25l[54;141H9[19;1H[?12l[?25h[?25l[54;140H20,0-1[20;1H[?12l[?25h[?25l[54;141H1,1  [21;1H[?12l[?25h[?25l[54;141H2[22;1H[?12l[?25h[?25l[54;141H3[23;1H[?12l[?25h[?25l[54;141H4[24;1H[?12l[?25h[?25l[54;141H5[25;1H[?12l[?25h[?25l[54;141H6[26;1H[?12l[?25h[?25l[54;141H7,0-1[27;1H[?12l[?25h[?25l[54;141H8,1  [28;1H[?12l[?25h[?25l[54;141H9[29;1H[?12l[?25h[?25l[54;140H30[30;1H[?12l[?25h[?25l[54;141H1[31;1H[?12l[?25h[?25l[54;143H2[31;2H[?12l[?25h[?25l[54;143H3[31;3H[?12l[?25h[?25l[54;143H4[31;4H[?12l[?25h[?25l[54;143H5[31;5H[?12l[?25h[?25l[54;143H6[31;6H[?12l[?25h[?25l[54;143H7[31;7H[?12l[?25h[?25l[54;143H8[31;8H[?12l[?25h[?25l[54;143H9[31;9H[?12l[?25h[?25l[54;143H10[31;10H[?12l[?25h[?25l[54;144H1[31;11H[?12l[?25h[?25l[54;144H2[31;12H[?12l[?25h[?25l[54;141H0[30;12H[?12l[?25h[?25l[54;140H29[29;12H[?12l[?25h[?25l[54;141H8[28;12H[?12l[?25h[?25l[54;141H7,0-1[27;1H[?12l[?25h[?25l[54;141H6,12 [26;12H[?12l[?25h[?25l[54;141H5[25;12H[?12l[?25h[?25l[54;141H4[24;12H[?12l[?25h[?25l[54;141H3[23;12H[?12l[?25h[?25l[54;141H2[22;12H[?12l[?25h[?25l[54;141H1[21;12H[?12l[?25h[?25l[54;141H0,0-1[20;1H[?12l[?25h[?25l[54;140H19,12 [19;12H[?12l[?25h[?25l[54;141H8[18;12H[?12l[?25h[?25l[54;144H1[18;11H[?12l[?25h[?25l[54;144H0[18;10H[?12l[?25h[?25l[54;144H1[18;11H[?12l[?25h[?25l[54;1H[1m-- INSERT --[m[54;13H[K[54;140H18,11[9CAll[18;10H''[18;12H[K[18;11H[?12l[?25h[?25l'[31mR[m'[54;144H2[18;12H[?12l[?25h[?25l[31mRA[m'[54;144H3[18;13H[?12l[?25h[?25l[31mAF[m'[54;144H4[18;14H[?12l[?25h[54;1H[K[18;13H[?25l[54;140H18,13[9CAll[18;13H[?12l[?25h[?25l[54;140H[K[54;1H:[?12l[?25hw[?25l[?12l[?25hq[?25l[?12l[?25h[?25l"configMaker.py" 31L, 1056C written
[?1l>[?12l[?25h[?1049l]0;kilojoules:/home/raf/nagios-qc64% vim compare.py 
[?1049h[?1h=[1;54r[?12;25h[?12l[?25h[27m[m[H[2J[?25l[54;1HE325: ATTENTION
Found a swap file by the name ".compare.py.swp"
[10Cowned by: jquick   dated: Thu Aug  7 11:22:16 2014
[9Cfile name: /home/raf/nagios-qc/compare.py
[10Cmodified: no
[9Cuser name: jquick   host name: kilojoules
[8Cprocess ID: 1057 (still running)
While opening file "compare.py"
[13Cdated: Thu Aug  7 11:22:12 2014

(1) Another program may be editing the same file.
    If this is the case, be careful not to end up with two
    different instances of the same file when making changes.
    Quit, or continue with caution.

(2) An edit session for this file crashed.
    If this is the case, use ":recover" or "vim -r compare.py"
    to recover the changes (see ":help recovery").
    If you did this already, delete the swap file ".compare.py.swp"
    to avoid this message.

Swap file ".compare.py.swp" already exists!
[O]pen Read-Only, (E)dit anyway, (R)ecover, (Q)uit, (A)bort:[?12l[?25h[54;1H[K[54;1H
[?1l>[?1049l]0;kilojoules:/home/raf/nagios-qc65% vim compare.py[11GnfigMaker.py[5Gsh piper.sh[K[6Gudo /sbin/service nagios restart[6Gh piper.sh[K[5Gvim configMaker.py[11Gmpare.py[K[5G[Ksh res[K[K[Kco
compare.py*     configMaker.py* 
]0;kilojoules:/home/raf/nagios-qc65% sh configMaker.py 
configMaker.py: line 1: from: command not found
configMaker.py: line 2: syntax error near unexpected token `('
configMaker.py: line 2: `def ServiceMaker(Host,Service,conf):'
]0;kilojoules:/home/raf/nagios-qc66% sh configMaker.py[5Gsh[P[P[1@p[1@y
py: Command not found.
]0;kilojoules:/home/raf/nagios-qc67% py configMaker.py[5Gpy[1@t[1@h
python           python2          python2.6        python2.6-config python-config    
]0;kilojoules:/home/raf/nagios-qc67% python configMaker.py[11G[1@o[P
Traceback (most recent call last):
  File "configMaker.py", line 17, in <module>
    configFile=open('/etc/nagios/raf/config.cfg','w')
IOError: [Errno 13] Permission denied: '/etc/nagios/raf/config.cfg'
]0;kilojoules:/home/raf/nagios-qc68% python configMaker.py[5G[Kchmod 777 /etc/nagios/raf/config.cfg
chmod: changing permissions of `/etc/nagios/raf/config.cfg': Operation not permitted
]0;kilojoules:/home/raf/nagios-qc69% chmod 777 /etc/nagios/raf/config.cfg[5Gpython configMaker.py[K[5Gchmod 777 /etc/nagios/raf/config.cfg[5G[Kchmod 777 /etc/nagios/raf/config.cfg[5G[1@s[1@u[1@d[1@o[1@ 
]0;kilojoules:/home/raf/nagios-qc70% sudo chmod 777 /etc/nagios/raf/config.cfg[5G[5P[41G[5Gpython configMaker.py[K
]0;kilojoules:/home/raf/nagios-qc71% python configMaker.py[5Gsudo chmod 777 /etc/nagios/raf/config.cfg[5G[5P[41G[5Gpy configMaker.py[K[5Gsh[22G[5Gvim compare.py[K[5Gsh configMaker.py[5Gvim compare.py[K[11GnfigMaker.py[5Gsh piper.sh[K[6Gudo /sbin/service nagios restart[10Gvim /etc/nagios/nagios.cfg[K[5G[5P[31G[5Gcat /etc/nagios/raf/*[K[5Gsudo rm /etc/nagios/radar/*[5G[5P[27G[5G[1@cat[28G[K[5Gpython compare.py[K[5Gvim piper.sh[K[5Gsh regen_cfg.sh[5Gls /etc/nagios/raf/[5Gsh regen_cfg.sh[K
Stopping nagios: .done.
define host{
   use                     generic-radar-host
   host_name               RAF
   alias                   RAF
   address                 0.0.0.0
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     PRES
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     PSXC
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     A2DTEMP
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     IRIG_STATUS
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     IRIG_Tdiff
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     ATX
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     DPXC
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     CAVP
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     PSFC
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     PSFD
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     QCF
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     QCR
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     TASR
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     TASF
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     LAT
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     LON
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     GGLAT
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     GGLON
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     GGQUAL
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     GGSTATUS
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     FTMP
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     FTMP3
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     CNTEMP
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     FREF
}

define service{
   use                     generic-radar-service
   host_name               RAF
   service_description     FREF3
}

Starting nagios: done.
]0;kilojoules:/home/raf/nagios-qc72% sh regen_cfg.sh[5Gpython configMaker.py[5Gsudo chmod 777 /etc/nagios/raf/config.cfg[5G[5P[41G[5Gpy configMaker.py[K[5Gsh[22G[5Gvim compare.py[K[5Gsh configMaker.py[5Gvim compare.py[K[11GnfigMaker.py[5Gsh piper.sh[K
]0;kilojoules:/home/raf/nagios-qc73% sh piper.sh
]0;kilojoules:/home/raf/nagios-qc74% cat /tmp/commands 
Air;QCR;2;critical::**Below minimum limit** value is 67.9684
Air;QCF;2;critical::**Below minimum limit** value is 67.9741
Air;ATX;2;critical::**Below minimum limit** value is 19.2881
Air;GGLON;2;**FLATLINING**
Air;GGQUAL;1;warning:: value is 9.0
Air;PSXC;2;critical::**Below minimum limit** value is 806.962
Air;GGLAT;2;**FLATLINING**
Air;DPXC;2;critical::**Below minimum limit** value is 11.2433
]0;kilojoules:/home/raf/nagios-qc75% cat /tmp/commands[5Gsh piper.sh[K[8Gregen_cfg.sh[8Gpiper.sh[K
]0;kilojoules:/home/raf/nagios-qc76% 
]0;kilojoules:/home/raf/nagios-qc76% 
]0;kilojoules:/home/raf/nagios-qc76% 
]0;kilojoules:/home/raf/nagios-qc76% 
]0;kilojoules:/home/raf/nagios-qc76% sh piper.sh[5G[Kcat piper.sh 
#!/bin/sh
# This script writes passive checks to the Nagios command file to cause
# it to process service check results.  Called by qc_check.

echocmd="/bin/echo"
CommandFile="/var/spool/nagios/cmd/nagios.cmd"
while read line
do
  # get the current date/time in seconds since UNIX epoch
  datetime=`date +%s`

  # create the command line to add to the command file
  cmdline="[$datetime] PROCESS_SERVICE_CHECK_RESULT;$line"
  #echo "$cmdline"

  # append the command to the end of the command file
  `$echocmd $cmdline >> $CommandFile`
#  echo $cmdline
done </tmp/commands
]0;kilojoules:/home/raf/nagios-qc77% cat piper.sh[5G[Psh[16G
]0;kilojoules:/home/raf/nagios-qc78% python compare.py 
]0;kilojoules:/home/raf/nagios-qc79% python compare.py[5Gsh piper.sh[K
]0;kilojoules:/home/raf/nagios-qc80% locat ''|grep Service is not scheduled to be checked.
locat: Command not found.
grep: is: No such file or directory
grep: not: No such file or directory
grep: scheduled: No such file or directory
grep: to: No such file or directory
grep: be: No such file or directory
grep: checked.: No such file or directory
]0;kilojoules:/home/raf/nagios-qc81% 
]0;kilojoules:/home/raf/nagios-qc81% 
]0;kilojoules:/home/raf/nagios-qc81% locat '' | grep Service is not scheduled to be checked.[5Gsh piper.sh[K[5Glocat '' | grep Service is not scheduled to be checked.[5G[Klocat '' | grep Service is not scheduled to be checked.cat[1@e
grep: is: No such file or directory
grep: not: No such file or directory
grep: scheduled: No such file or directory
grep: to: No such file or directory
grep: be: No such file or directory
grep: checked.: No such file or directory
]0;kilojoules:/home/raf/nagios-qc82% locate '' | grep Service is not scheduled to be checked.[1@'Service is not scheduled to be checked.'
]0;kilojoules:/home/raf/nagios-qc83% locate '' | grep 'Service is not scheduled to be checked.'[5G[Klocate '' | grep 'Service is not scheduled to be checked.'[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K[K'
/etc/dbus-1/system.d/org.fedoraproject.Config.Services.conf
/home/check_mk-1.1.8/livestatus.src/src/OffsetStringServiceMacroColumn.cc
/home/check_mk-1.1.8/livestatus.src/src/OffsetStringServiceMacroColumn.h
/home/check_mk-1.1.8/livestatus.src/src/ServiceContactsColumn.cc
/home/check_mk-1.1.8/livestatus.src/src/ServiceContactsColumn.h
/home/check_mk-1.1.8/livestatus.src/src/ServicegroupsColumn.cc
/home/check_mk-1.1.8/livestatus.src/src/ServicegroupsColumn.h
/home/check_mk-1.1.8/livestatus.src/src/ServicelistColumn.cc
/home/check_mk-1.1.8/livestatus.src/src/ServicelistColumn.h
/home/check_mk-1.1.8/livestatus.src/src/ServicelistColumnFilter.cc
/home/check_mk-1.1.8/livestatus.src/src/ServicelistColumnFilter.h
/home/check_mk-1.1.8/livestatus.src/src/ServicelistStateColumn.cc
/home/check_mk-1.1.8/livestatus.src/src/ServicelistStateColumn.h
/home/check_mk-1.1.8/livestatus.src/src/TableServicegroups.cc
/home/check_mk-1.1.8/livestatus.src/src/TableServicegroups.h
/home/check_mk-1.1.8/livestatus.src/src/TableServices.cc
/home/check_mk-1.1.8/livestatus.src/src/TableServices.h
/home/check_mk-1.1.8/livestatus.src/src/livestatus_so-OffsetStringServiceMacroColumn.o
/home/check_mk-1.1.8/livestatus.src/src/livestatus_so-ServiceContactsColumn.o
/home/check_mk-1.1.8/livestatus.src/src/livestatus_so-ServicegroupsColumn.o
/home/check_mk-1.1.8/livestatus.src/src/livestatus_so-ServicelistColumn.o
/home/check_mk-1.1.8/livestatus.src/src/livestatus_so-ServicelistColumnFilter.o
/home/check_mk-1.1.8/livestatus.src/src/livestatus_so-ServicelistStateColumn.o
/home/check_mk-1.1.8/livestatus.src/src/livestatus_so-TableServicegroups.o
/home/check_mk-1.1.8/livestatus.src/src/livestatus_so-TableServices.o
/home/check_mk-1.1.8/livestatus.src/src/.deps/livestatus_so-OffsetStringServiceMacroColumn.Po
/home/check_mk-1.1.8/livestatus.src/src/.deps/livestatus_so-ServiceContactsColumn.Po
/home/check_mk-1.1.8/livestatus.src/src/.deps/livestatus_so-ServicegroupsColumn.Po
/home/check_mk-1.1.8/livestatus.src/src/.deps/livestatus_so-ServicelistColumn.Po
/home/check_mk-1.1.8/livestatus.src/src/.deps/livestatus_so-ServicelistColumnFilter.Po
/home/check_mk-1.1.8/livestatus.src/src/.deps/livestatus_so-ServicelistStateColumn.Po
/home/check_mk-1.1.8/livestatus.src/src/.deps/livestatus_so-TableServicegroups.Po
/home/check_mk-1.1.8/livestatus.src/src/.deps/livestatus_so-TableServices.Po
/home/raf/python/iss/rsync/RsyncService.py
/usr/include/QtDBus/QDBusServiceWatcher
/usr/include/QtGui/QDesktopServices
/usr/include/xercesc/util/TransService.hpp
/usr/include/xercesc/util/Transcoders/IconvGNU/IconvGNUTransService.hpp
/usr/lib64/libQtServiceFramework.so.1
/usr/lib64/libQtServiceFramework.so.1.1
/usr/lib64/libQtServiceFramework.so.1.1.3
/usr/share/dbus-1/interfaces/org.kde.nepomuk.QueryService.xml
/usr/share/dbus-1/interfaces/org.kde.nepomuk.ServiceControl.xml
/usr/share/dbus-1/interfaces/org.kde.nepomuk.ServiceManager.xml
/usr/share/dbus-1/system-services/org.fedoraproject.Config.Services.service
/usr/share/gtk-doc/html/gdata/GDataCalendarService.html
/usr/share/gtk-doc/html/gdata/GDataContactsService.html
/usr/share/gtk-doc/html/gdata/GDataDocumentsService.html
/usr/share/gtk-doc/html/gdata/GDataPicasaWebService.html
/usr/share/gtk-doc/html/gdata/GDataService.html
/usr/share/gtk-doc/html/gdata/GDataYouTubeService.html
/usr/share/gtk-doc/html/gio/GNetworkService.html
/usr/share/gtk-doc/html/gio/GSocketService.html
/usr/share/gtk-doc/html/gio/GThreadedSocketService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServiceAvailableEvent.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServiceProviderBeanInfo.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServiceRevokedEvent.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServiceRevokedListener.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServices.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServicesListener.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServicesSupport.BCSSChild.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServicesSupport.BCSSProxyServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServicesSupport.BCSSServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/BeanContextServicesSupport.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServiceAvailableEvent.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServiceProviderBeanInfo.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServiceRevokedEvent.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServiceRevokedListener.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServices.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServicesListener.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServicesSupport.BCSSChild.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServicesSupport.BCSSProxyServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServicesSupport.BCSSServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/beans/beancontext/class-use/BeanContextServicesSupport.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/net/UnknownServiceException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/net/class-use/UnknownServiceException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/security/Provider.Service.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/security/class-use/Provider.Service.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/ServiceConfigurationError.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/ServiceLoader.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/class-use/ServiceConfigurationError.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/class-use/ServiceLoader.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/AbstractExecutorService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/CompletionService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/ExecutorCompletionService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/ExecutorService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/ScheduledExecutorService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/class-use/AbstractExecutorService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/class-use/CompletionService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/class-use/ExecutorCompletionService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/class-use/ExecutorService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/concurrent/class-use/ScheduledExecutorService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/spi/LocaleServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/java/util/spi/class-use/LocaleServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/imageio/spi/IIOServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/imageio/spi/RegisterableService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/imageio/spi/ServiceRegistry.Filter.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/imageio/spi/ServiceRegistry.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/imageio/spi/class-use/IIOServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/imageio/spi/class-use/RegisterableService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/imageio/spi/class-use/ServiceRegistry.Filter.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/imageio/spi/class-use/ServiceRegistry.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/jws/WebService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/jws/class-use/WebService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/ServiceNotFoundException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/class-use/ServiceNotFoundException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/relation/InvalidRelationServiceException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/relation/RelationService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/relation/RelationServiceMBean.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/relation/RelationServiceNotRegisteredException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/relation/class-use/InvalidRelationServiceException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/relation/class-use/RelationService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/relation/class-use/RelationServiceMBean.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/relation/class-use/RelationServiceNotRegisteredException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/remote/JMXServiceURL.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/management/remote/class-use/JMXServiceURL.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/naming/ServiceUnavailableException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/naming/class-use/ServiceUnavailableException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/MultiDocPrintService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/PrintService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/PrintServiceLookup.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/ServiceUI.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/ServiceUIFactory.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/StreamPrintService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/StreamPrintServiceFactory.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/attribute/HashPrintServiceAttributeSet.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/attribute/PrintServiceAttribute.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/attribute/PrintServiceAttributeSet.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/attribute/class-use/HashPrintServiceAttributeSet.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/attribute/class-use/PrintServiceAttribute.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/attribute/class-use/PrintServiceAttributeSet.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/class-use/MultiDocPrintService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/class-use/PrintService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/class-use/PrintServiceLookup.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/class-use/ServiceUI.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/class-use/ServiceUIFactory.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/class-use/StreamPrintService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/class-use/StreamPrintServiceFactory.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/event/PrintServiceAttributeEvent.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/event/PrintServiceAttributeListener.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/event/class-use/PrintServiceAttributeEvent.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/print/event/class-use/PrintServiceAttributeListener.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/security/auth/kerberos/ServicePermission.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/security/auth/kerberos/class-use/ServicePermission.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/crypto/dsig/TransformService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/crypto/dsig/class-use/TransformService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/Service.Mode.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/Service.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/ServiceMode.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/WebServiceClient.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/WebServiceContext.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/WebServiceException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/WebServiceFeature.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/WebServicePermission.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/WebServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/WebServiceRef.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/WebServiceRefs.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/Service.Mode.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/Service.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/ServiceMode.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/WebServiceClient.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/WebServiceContext.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/WebServiceException.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/WebServiceFeature.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/WebServicePermission.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/WebServiceProvider.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/WebServiceRef.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/class-use/WebServiceRefs.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/spi/ServiceDelegate.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/spi/WebServiceFeatureAnnotation.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/spi/class-use/ServiceDelegate.html
/usr/share/javadoc/java-1.6.0-openjdk/api/javax/xml/ws/spi/class-use/WebServiceFeatureAnnotation.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/ServiceDetail.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/ServiceDetailHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/ServiceInformation.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/ServiceInformationHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/ServiceInformationHolder.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/class-use/ServiceDetail.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/class-use/ServiceDetailHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/class-use/ServiceInformation.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/class-use/ServiceInformationHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/CORBA/class-use/ServiceInformationHolder.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/ServiceContext.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/ServiceContextHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/ServiceContextHolder.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/ServiceContextListHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/ServiceContextListHolder.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/ServiceIdHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/TransactionService.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/class-use/ServiceContext.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/class-use/ServiceContextHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/class-use/ServiceContextHolder.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/class-use/ServiceContextListHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/class-use/ServiceContextListHolder.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/class-use/ServiceIdHelper.html
/usr/share/javadoc/java-1.6.0-openjdk/api/org/omg/IOP/class-use/TransactionService.html
/usr/share/javadoc/java-1.6.0-openjdk/jdk/api/jpda/jdi/com/sun/jdi/connect/spi/TransportService.Capabilities.html
/usr/share/javadoc/java-1.6.0-openjdk/jdk/api/jpda/jdi/com/sun/jdi/connect/spi/TransportService.ListenKey.html
/usr/share/javadoc/java-1.6.0-openjdk/jdk/api/jpda/jdi/com/sun/jdi/connect/spi/TransportService.html
/usr/share/javadoc/java-1.6.0-openjdk/jdk/api/jpda/jdi/com/sun/jdi/connect/spi/class-use/TransportService.Capabilities.html
/usr/share/javadoc/java-1.6.0-openjdk/jdk/api/jpda/jdi/com/sun/jdi/connect/spi/class-use/TransportService.ListenKey.html
/usr/share/javadoc/java-1.6.0-openjdk/jdk/api/jpda/jdi/com/sun/jdi/connect/spi/class-use/TransportService.html
/usr/share/javadoc/java-1.6.0-openjdk/jre/api/plugin/dom/com/sun/java/browser/dom/DOMService.html
/usr/share/javadoc/java-1.6.0-openjdk/jre/api/plugin/dom/com/sun/java/b