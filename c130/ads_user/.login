################################################################################
#                                                                              #
# .login file for onboard turnkey logins                                       #
#                                                                              #
################################################################################

onintr
if (`uname` == Linux) then
  set host = `hostname -s`
else
  set host = `uname -n`
endif
setenv DISPLAY ${host}:0
echo "" 
echo "Display set to $DISPLAY"  
echo ""

source $LOCAL/scripts/SetAirplanesAndHosts
set one_host = $host
source $LOCAL/scripts/AskHostForWindsServer

# $LOCAL/winds/bin/prt_srvr ttyS0 &

if ( `tty` == /dev/console ) then
 echo ""
 echo "...WINDS turnkey operation starting up on $host..."
 echo ""
 if ( $host == $CALCART ) then
  echo ""
  echo "This host is the Calibration Cart..."
#
# need to get out of home dir if this is the calib-cart, since we want to 
# mount a different filesystem over it in order to "connect" to onboard server
#
  cd /
  unsetenv WindsServer
  source $LOCAL/scripts/GetAirplaneFromUser
  source $LOCAL/scripts/AskHostForWindsServer
  source /home/local/scripts/connect
  cd $LOCAL/winds/proj/hosts/$CALCART
 endif
 if ( $?WindsServer ) then
  if ( `hostname -s` != $WindsServer) then
   echo ""
   echo "Synchronizing time with $WindsServer..."
   echo ""
   rsh $host -l root rdate $WindsServer
   if ( $status == 1 ) then
    echo ""
    echo "WHOOPS...permissions aren't set right to allow this..."
    echo "You're advised to manually change things in /.rhosts to grant"
    echo "$host-winds root permission. In the meantime it is possible"
    echo "that date/time will not be sync'd between $host and $WindsServer."
    echo ""
    echo "Please see an expert if needed..."
    echo ""
    echo "Hit <r> to continue..."
    set ready = $<
   endif
  endif
 endif
 if ( -e /dev/fbs/tvtwo0 ) then
#
# this covers NetVUE ala UniFlix or Parallax, as per netconfig entries
#
  echo ""
  echo "initiating real-time video environment... "
  echo ""
  $WINDS/scripts/NetVUE -openwin
  echo ""
 else
  echo ""
  echo "Starting GNOME ... "
  echo ""
#  tar xf ads_gnome.tar
  startx
 endif
endif
