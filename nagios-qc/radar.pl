#!/usr/bin/perl -w

# WHAT IS IT WORTH?
# -------------------------------------------------------------------
# this script generates automated config files with host and
# service definitions for icinga/nagios from passively received
# host and service checks.
#  
# This might be useful if your nagios/icinga system runs in a
# distributed environment with icinga/nagios instances sending
# passive checks to a central nagios/icinga server or if you
# have many hosts sending passive checks.
# 
# Normally, hosts and services based on passive checks must
# be configured both on the nagios/icinga instance receiving
# the checks and on the instance sending the checks This script
# uses received passive checks to create automated host and service
# definitions in the nagios/icinga configuration. This way
# the configuration of the implied hosts and services must
# only be made on the instance that sends the passive checks
#
# Moreover this script can be used to make nagios and Munin
# work together. You can run a Munin server and a nagios/icinga
# server in parallel. The munin server sends its results as
# passive checks to the nagios/icinga server. Then the
# corresponding service and host definitions are generated 
# and included in the nagios/icinga configuration. 
#
#
# HOW DOES IT WORK?
# -------------------------------------------------------------------
# The instance that sends passive checks must send them to
# the NSCA of the central nagios/icinga server. So be sure
# you have configured the NSCA correctly
#
# When a pssive check result is received by the nagios/icinga
# instance and the corresponding host and service respectively
# does not exist in the nagios/icinga configuration an error
# occurs and is written to the nagios/icinga log file.
#
# the radar script parses the nagios/icinga log file for these
# errors and uses them to generate automated host and service
# definitions in the nagios/icinga configuration.
#
# As long as nagios/icinga is not reloaded passive host and
# service checks keep causing errors in the nagios/icinga
# log file. Thus, to avoid multiple host and service definitions
# for the same host or service, the radar script parses the entire
# nagios/icinga configuration.
#
# As the icinga/nagios configuration may be very large the script
# gets active only every hour, working in the background as a daemon.
# The time interval can be set to any desired value.
#
# When running the radar script it is run in daemon mode instantly.
# So use an init script to start/stop/restart the daemon
#
# TEMPLATES
# -------------------------------------------------------------------
# the radar script only generates rudimentary host and service
# information. To make them work a host and service template for
# the generated hosts and services must be included into the
# nagios/icinga configuration. The names of the templates are set
# in the configuration of the radar script (see below).
# You can use the following example templates.
#  
# define host{
#        name                            generic-radar-host
#        notifications_enabled           1
#        event_handler_enabled           1
#        flap_detection_enabled          1
#        failure_prediction_enabled      1
#        process_perf_data               1
#        retain_status_information       1
#        retain_nonstatus_information    1
#        check_command                   check-host-alive
#        max_check_attempts              10
#        check_period                    24x7
#        notification_interval           120
#        notification_options            d,u,r
#        notification_period             24x7
#        contact_groups                  admins
#        active_checks_enabled           0
#        passive_checks_enabled          1
#        host_groups                     radarhosts
#        register                        0
# }
# 
#
# define service {
#        name                            generic-radar-service
#        check_command                   check_dummy
#        active_checks_enabled           0
#        passive_checks_enabled          1
#        parallelize_check               1
#        obsess_over_service             1
#        check_freshness                 0
#        notifications_enabled           1
#        event_handler_enabled           1
#        flap_detection_enabled          1
#        failure_prediction_enabled      1
#        process_perf_data               1
#        retain_status_information       1
#        retain_nonstatus_information    1
#        is_volatile                     0
#        check_period                    24x7
#        max_check_attempts              3
#        normal_check_interval           10
#        retry_check_interval            2
#        contact_groups                  admins
#        notification_options            w,u,c,r
#        notification_interval           120
#        notification_period             24x7
#        service_groups                  radarservices
#        register                        0
# } 
# 
#
# CONFIGURATION
# -------------------------------------------------------------------



###################### BASIC CONFIGURATION ########################

# absolute path to the nagios/icinga log file
my $NAGIOS_LOGFILE="/var/log/nagios/nagios.log";

# the absolute path to the directory where generated cfg-files
# are to be stored
my $CFG_DIRECTORY="/etc/nagios/radar";

# the absolute path to the directory where the
# nagios/icinga cfg files are kept
my $NAGIOS_CONFIG="/etc/nagios/objects/";

# the nagios/icinga host template to use for generated
# host definitions
my $HOST_TEMPLATES="generic-radar-host";

# the nagios/icinga service template to use for
# generated service definitions
my $SERVICE_TEMPLATES="generic-radar-service";

# the nagios/icinga user
my $ICINGA_USER="nagios";

# the nagios/icinga group
my $ICINGA_GROUP="nagios";

# enable/disable logging (1=enabled)
my $ENABLE_LOGGING=1;

# directory to put the log file
my $LOGFILE_DIRECTORY="/var/log/";

# directory to put the pid file                                 #jquick
#my $PID_FILE_DIRECTORY="/usr/local/lib64/perl5";               #if problems arise, try sudo perl -MCPAN -e shell
my $PID_FILE_DIRECTORY="/var/run";                              #if problems arise, try sudo perl -MCPAN -e shell
                                                                #                           > install File::Pid
###################################################################




###################### ADVANCED CONFIGURATION #####################

# prefix for generated cfg files
my $CFG_FILE_PREFIX="NCAR";

# a regular expression to find a new host from the nagios/icinga
# logging output
my $HOST_PATTERN=qr/Passive\s+check\s+result.+received\s+for\s+host\s+'(.+)'.+not.+found/i;

# a regular expression to find a new service from the
# nagios/icinga logging output
my $SERVICE_PATTERN=qr/Passive\s+check\s+result.+received\s+for\s+service\s+'(.+)'\s+on\s+host\s+'(.+)'.+not.+found/i;

# the time interval in seconds to use for searching the nagios/icinga
# log file for new hosts and services
my $SCAN_INTERVAL=1;

# the time interval in seconds to use for creating new cfg files
# within this interval all new host and service definitions
# are written to the same file. After the interval another
# cfg file will be created and new host and service definitions
# will be written to the new file
my $CFG_CREATION_INTERVAL=1;                                          #Changes efficiency

# this sets the permissions of the directory where generated
# cfg files are stored
my $DIRECTORY_MASK=0755;

# this sets the permissions of the generated cfg files
my $FILE_MASK=0664;

# the name of the daemon
my $DAEMON_NAME="radar";

###################################################################





use strict;
use warnings;
use POSIX;
use File::Pid;
use Socket;
use Fcntl qw(:flock);
use File::Find;
use POSIX qw(setsid);
my $DIE_NOW=0;


# read the icinga log file and store it to an array
sub readLogFile {

   open(IN, "<$NAGIOS_LOGFILE");
   my @file=<IN>;
   close IN;

   logEntry("nagios log file ($NAGIOS_LOGFILE): found $#file lines");
   return \@file;
}


# read the cfg directory ($CFG_DOIRTECTORY) recursively and
# put the locally defined hosts and services to hashes %hosts and %services
sub readCfgDirectory {

   my $hosts=shift;      # param: reference to hosts hash
   my $services=shift;   # param: reference to services hash

   my @files=$NAGIOS_CONFIG;
   my @filesArr=();
   find sub {push(@filesArr,$File::Find::name)}, @files;

   foreach(@filesArr) {

      if (-f $_ and  $_=~/\.cfg$/) {

         my $content=readCfgFile($_);
         parseCfgContent($content, $hosts, $services);
      }
   }

   logEntry("nagios cfg directory ($NAGIOS_CONFIG): found " . ($#filesArr + 1) . " files, " . keys(%{$hosts}) . " hosts and " . keys(%{$services}) . " services");
}


# read a particular cfg-file and return its content as an array
sub readCfgFile {

    my $cfgFile=shift;    # param: path to cfg-file
    my @content=();

    if(-f $cfgFile) {

       open(TMPCFGFILE, "<", $cfgFile);
       @content=<TMPCFGFILE>;
       close(TMPCFGFILE);
    }

    logEntry("nagios cfg-file ($cfgFile): parsed $#content lines");
    return \@content;
}


# parse the content of a cfg-file and find new hosts and services
sub parseCfgContent  {

    my $cfgContent=shift;  # param: reference to file content array (provided by readCfgFile)
    my $hosts=shift;       # param: reference to hosts hash 
    my $services=shift;    # param: reference to services hash

    my $def="";
    my $hostName="";
    my $serviceDescription="";


    foreach(@{$cfgContent}) {

       $_=~s/^\s+//;   # remove leading whitespaces
       $_=~s/\s+$//;   # remove closing whitespaces

       if($_=~/define\s+(host|service)\s*\{/) {

          $def=$1;  # found host or service definition
       }
       elsif($_=~/(host_name|service_description)\s+(\S.+)/) {

          if($1 eq "host_name") {

             $hostName=$2;  # found host description

             if($def ne "host" and $def ne "service") {

                return 0; # host description found outside a host or service definition => invalid cfg-file
             }
             elsif($def eq "host") {

                # we are in scope of host definition
                # host definition is complete
                ${$hosts}{$hostName}=1;
                $def="";
                $hostName="";
             }
             elsif($def eq "service") {
                
                # we are in scope of service definition
                if($serviceDescription eq "") {

                    next; # service description has not been found yet, continue with next line
                }
                else {

                    # service description has been found, now service definition is complete
                    ${$services}{$hostName.":".$serviceDescription}=1;
                    $def="";
                    $hostName="";
                }
             }
          }
          else {

             $serviceDescription=$2;  # found service description

             if($def ne "service") {

                return 0;  # service description was found outside a service definition => invalid cfg-file
             }

             if($hostName eq  "") {

                next;  # service definition is not yet complete, host name is missing => continue with next line
             }
             else {

                # host name is already defined, service definition is complete,
                # search for next host or service definition
                ${$services}{$hostName.":".$serviceDescription}=1;
                $def="";
                $serviceDescription="";
                $hostName="";
             }
          }
       }
    }

    return 1;
}


# extract host and service messages of non-existing hosts and services out of the log file
sub parseLog {

   my $logContent=shift;     # param: the log file as array, provided by readLogFile()
   my %parsedContent;        # list of hashes with hosts as keys and service array-list as values

   foreach(@$logContent) {   

      # for each line of the log file: 
      # check if the entry is a message of an unknown host
      if($_=~$HOST_PATTERN) {

         # if it's a host, check if the host already exists in the host hash
         if(not exists $parsedContent{$1}) {

            # if the host doesn't exits yet, create a hash entry
            # with an empty service array
            @{$parsedContent{$1}}=();
         }
      }
      # if it's not a host, check if it is a service
      else {

         if($_=~$SERVICE_PATTERN) {
 
            # if it's a service, check if the host associated with it exists
            if(not exists $parsedContent{$2}) {

               # if the host doesn't exist, create a hash entry for the host
               # with an empty host array
               @{$parsedContent{$2}}=();
            }
           
            # push the service entry to the host's service array
            push(@{$parsedContent{$2}},$1);
            
         }
      }
   }

   # remove duplicate entries from each host's service list
   while((my $key, my $value) = each(%parsedContent)) {

      @{$parsedContent{$key}}=removeDuplicateItems(@{$value});
   }

   logEntry("nagios log file ($NAGIOS_LOGFILE): found " . keys(%parsedContent) . " hosts with new services");

   return \%parsedContent; 
}


# helper function to delete duplicate entries in an array
sub removeDuplicateItems {

   my %all;
   $all{$_}=0 for @_;
   return (keys %all);
}


# helper function to get icinga user id
sub getIcingaUserId {

   (my $name, my $pass, my$uid, my $gid, my $quo, my $com, my $gc, my $dir, my $sh,my $exp) = getpwnam($ICINGA_USER);
   return $uid;
}


# helper function to get icinga group id
sub getIcingaGroupId {

   (my $name, my $pass, my $gid, my $members)=getgrnam($ICINGA_GROUP);
   return $gid;
}


# helper function to set file ownership and permissions
sub setFilePermissions {

   my $file=shift;
   my $mask=shift;
   my $uid=getIcingaUserId();
   my $gid=getIcingaGroupId();

   chown($uid, $gid, $file);
   chmod($mask, $file);
}


# helper function to add "/" to path
sub dirPath {

   my $dir=shift;
   
   if($dir=~m/\/$/) {

      return $dir;
   }

   return $dir."/";
}


# create the directory where the scanned hosts and services are to be stored
sub createCfgDirectory {

   my $akdir='/';
   my $entirePath=dirPath($CFG_DIRECTORY);

   $entirePath=~ s/^\s+//; # remove spaces at the beginning
   $entirePath=~ s/\s+$//; # remove spaces at the end
   $entirePath=~ s/^\///;  # remove leading '/'
   $entirePath=~ s/\/$//;  # remove closing '/'

   my @singleDirectories=split(/\//,$entirePath); # create array of single directories

   # create directories recursively
   foreach (@singleDirectories) {

      $akdir.=$_;
      if (!-e $akdir) {

         # create directory if doesn't exist
         if(mkdir($akdir)!=1) {

            return 0;
         }

         # set ownership and permissions of current directory
         setFilePermissions($akdir,$DIRECTORY_MASK);
         logEntry("created directory: $akdir");
      }

      $akdir.='/';
   }

   return 1;
}


# open the cfg file where new hosts and services are to be defined
sub openCfgFile {

   my $type=shift;

   # create one cfg file per week
   my $suffix=sprintf ("%.0f", time()/$CFG_CREATION_INTERVAL);
   my $cfgFilePath=dirPath($CFG_DIRECTORY)."$CFG_FILE_PREFIX"."_".$type."_".$suffix.".cfg";

   if(-e $cfgFilePath) {

      # if cfg file exists open to append content
      open(CFGFILE, ">>$cfgFilePath");
   }
   else {

      # if cfg file doesn't exist create and open it
      open(CFGFILE, ">$cfgFilePath");
   }

   logEntry("cfg-file: $cfgFilePath: opened for writing");

   # lock the cfg-file, so no other process can use it
   flock(CFGFILE, LOCK_EX);
   logEntry("cfg-file $cfgFilePath: locked for writing");

   # set ownership and permissions of cfg-file
   setFilePermissions($cfgFilePath,$FILE_MASK); 
}


# close an open file (file handle CFGFILE must have been opened by openCfgFile())
sub closeCfgFile {

   flock(CFGFILE, LOCK_UN);
   logEntry("cfg-file unlocked");
   close(CFGFILE);
   logEntry("cfg-file closed");
}


# check if a host has already been discovered and written to the cfg-files
sub hostExists {

   my $hostName=shift;    # param: host name
   my $hostsHash=shift;   # param: reference to hosts hash

   if(exists ${$hostsHash}{$hostName}) {

      return 1;
   }

   return 0;
}


# check if a service has already been discovered and written to the cfg-files
sub serviceExists {

   my $hostName=shift;             # param: host name
   my $serviceDescription=shift;   # param: service description
   my $servicesHash=shift;         # param: reference to services hash

   if(exists ${$servicesHash}{$hostName.":".$serviceDescription}) {

      return 1;
   }

   return 0;
}



# write single host definition to cfg-file, use standard values
sub writeHostDefinition {

   my $hostName=shift;   # param: host name
   my $hostsHash=shift;  # param: reference to a hash where discovered hosts are stored (*)

   # (*) we need to remember the created host definitions
   # so there are no multiple host definitions in the cfg-files

   # skip if the host has already been written to the cfg-files
   if(hostExists($hostName,$hostsHash)) {

      return 1;
   }

   my $hostAddress=getIpByHostname($hostName);
   my $hostTemplates=$HOST_TEMPLATES;

   $hostTemplates=~s/^\s+//; # remove leading whitespaces
   $hostTemplates=~s/\s+$//; # remove closing whitespaces

   my $hostDef ="\ndefine host{\n";

   # insert host template if defined in the configuration section of this script
   if($hostTemplates ne "") {

      $hostDef.="   use                     $hostTemplates\n";
   }

   $hostDef.="   host_name               $hostName\n";
   $hostDef.="   alias                   $hostName\n";
   $hostDef.="   address                 $hostAddress\n";
   $hostDef.="}\n";

   # push host to hosts hash
   ${$hostsHash}{$hostName}=1;

   # print host definition to file handler CFGFILE
   # CFGFILE must have been opened before by openCfgFile()
   print CFGFILE $hostDef;

   logEntry("created new cfg-entry for host $hostName");
}


# write single service definition to cfg-file, use default parameters
# disable active checks and enable passive checks
sub writeServiceDefinition {

   my $hostName=shift;
   my $serviceDescription=shift;              # param: service description
   my $serviceTemplates=$SERVICE_TEMPLATES;   # param: serviceTemplates e.g. "template1,template2" 
   my $hostsHash=shift;                       # reference to a hash where discovered hosts are stored (*)
   my $servicesHash=shift;                    # reference to a hash where discovered services are stored (*)

   # (*) we need to remember the created host and service definitions
   # so there are no multiple host or service definitions in the cfg-files

   # skip if the service has already been written to the cfg-file
   if(serviceExists($hostName,$serviceDescription,$servicesHash)) {

      return 1;
   }

   $serviceTemplates=~s/^\s+//; # remove leading whitespaces
   $serviceTemplates=~s/\s+$//; # remove closing whitespaces

   my $srvDef ="\ndefine service{\n";

   # insert service templates if defined, see configuration
   if($serviceTemplates ne "") {

      $srvDef.="   use                     $serviceTemplates\n";
   }

   $srvDef.="   host_name               $hostName\n";
   $srvDef.="   service_description     $serviceDescription\n";
   $srvDef.="}\n";

   # push host to the hosts hash and the service to the services hash
   ${$hostsHash}{$hostName}=1;
   ${$servicesHash}{$hostName.":".$serviceDescription}=1;

   # print service definition to file handler CFGFILE
   # CFGFILE must have been opened before by openCfgFile()
   print CFGFILE $srvDef;

   logEntry("created new cfg-entry for service $serviceDescription on host $hostName");
}


# write all host definitions to cfg-file
# parameter $parsedContent is a reference to a hash of arrays: 
# host_name => (service1, service2,...)
# $parsedContent is yielded by parseLog()
sub createHostDefinitions {

   my $parsedContent=shift;  # param: see description above
   my $hostsHash=shift;      # param: reference to hosts hash

   my $defStr="";

   openCfgFile("hosts");

   # get single hosts and create host definition for each of them
   while((my $key, my $value)=each %{$parsedContent}) {

      $defStr.=writeHostDefinition($key,$hostsHash);
   }

   closeCfgFile();

   return $defStr;
}


# write all service definitions to cfg-file
# parameter $parsedContent is a reference to a hash of arrays:
# host_name => (service1, service2)
# $parsedContent is yielded by parseLog()
sub createServiceDefinitions {

   my $parsedContent=shift;  # param: see description above
   my $hostsHash=shift;      # param: reference to hosts hash
   my $servicesHash=shift;   # param: reference to services hash  

   my $defStr="";

   openCfgFile("services");

   # get single host
   while((my $key, my $value)=each %{$parsedContent}) {

      # get single services of host and create service
      # definition for each of them
      foreach(@{$value}) {

         $defStr.=writeServiceDefinition($key,$_,$hostsHash,$servicesHash);
      }
   }

   closeCfgFile();

   return $defStr;
}


# try to get IP address by host name using 'ping'
# if the IP cannot be resolved, '0.0.0.0' is returned as default value
sub getIpByHostname {

   my $hostName=shift;
   my $result=`ping $hostName -c3`;

   # get IPV4 address
   if($result=~/(\d+\.\d+\.\d+\.\d+)/) {

      logEntry("host $hostName: has IP $1");
      return $1;
   }

   logEntry("host $hostName: no IP found, set IP to 0.0.0.0");
   return "0.0.0.0";
}



# run this file as daemon
sub daemonize {

   chdir '/';
   umask 0;
   open STDIN,  '/dev/null'   or die "Can't read /dev/null: $!";
   open STDOUT, '>>/dev/null' or die "Can't write to /dev/null: $!";
   open STDERR, '>>/dev/null' or die "Can't write to /dev/null: $!";
   defined( my $pid = fork ) or die "Can't fork: $!";
   exit if $pid;
print "hello Joolian\n";

   logEntry("switching to daemon mode");
}


# dissociate this process from the controlling terminal that started it and stop being part
# of whatever process group this process was a part of.
sub dissociate {

   POSIX::setsid() or die "Can't start a new session.";
}


# set callback signal handler for signals.
sub setSignalHandler {

   $SIG{INT} = $SIG{TERM} = $SIG{HUP} = \&signalHandler;
   $SIG{PIPE} = 'ignore';
}


# create pid file
sub createPidFile {

   my $pidFileName = dirPath($PID_FILE_DIRECTORY) . $DAEMON_NAME . ".pid";
   my $pidfile = File::Pid->new( { file => $pidFileName, } );
   $pidfile->write or die "Can't write PID file, /dev/null: $!";
   return $pidfile;
}



# turn on logging
sub activateLogging {

   my $logFileName = dirPath($LOGFILE_DIRECTORY) . $DAEMON_NAME . ".log";
   
   if($ENABLE_LOGGING) {

      open LOG, ">>$logFileName" or die "could not open log file";
      select((select(LOG), $|=1)[0]); # make the log file "hot" - turn off buffering
   }
}

# infinite main loop
sub loop {

   logEntry("starting...");

   my %hosts=();
   my %services=();

   # "infinite" loop where some useful process happens
   until ($DIE_NOW) {

       readCfgDirectory(\%hosts, \%services);

       my @logFile=readLogFile();
       # get host and service names from log file
       my $logContent=parseLog(@logFile);

       createCfgDirectory();
       createHostDefinitions($logContent,\%hosts);
       createServiceDefinitions($logContent,\%hosts,\%services);

       sleep($SCAN_INTERVAL);
       # logEntry("log something"); # use this to log whatever you need to
   }
}


# add a line to the log file
sub logEntry {
	my ($logText) = @_;
	my ( $sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst ) = localtime(time);
	my $dateTime = sprintf "%4d-%02d-%02d %02d:%02d:%02d", $year + 1900, $mon + 1, $mday, $hour, $min, $sec;
	if ($ENABLE_LOGGING) {
		print LOG "$dateTime $logText\n";
	}
}

# catch signals and end the program if one is caught.
sub signalHandler {

	$DIE_NOW = 1;    # this will cause the "infinite loop" to exit
        logEntry("Caught signal " .$SIG{INT}. ". Oh my God! I'm going to die!");
}

# do this when exit is called
sub cleanExit {

   my $pidFile = shift;

   if ($ENABLE_LOGGING) { 

      close LOG;
      $pidFile->remove if defined $pidFile;
   }
}


########################## MAIN ##############################

daemonize();
dissociate();
setSignalHandler();
my $pidFile = createPidFile();
activateLogging();
sleep(2);
loop();
END { cleanExit($pidFile); }

