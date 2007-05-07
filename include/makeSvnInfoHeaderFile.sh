#!/bin/csh
#
# this script creates a file svnInfo.h whch
# contains lines from the svn info command.
# The most important is the line containing the
# subversion revision number. It looks like:
# Revision: 1243
# If svnInfo.h does not exist, it will be created.
# If svnInfo.h does exist, it is only replaced if
# the current revision number is different from
# the one found in the file. Thus svnInfo.h should
# only change once per revision.
#
# get the revision and last changed date lines
# from the svn info command
set svnRevision = `svn info .. | grep Revision`
#
# Extract just the revision number
set rev = ($svnRevision)
set rev = $rev[2]
#
# if there is an existing svnInfo.h file, get
# the revision number from it.
set oldRev = "-1"
if (-e svnInfo.h) then
   set oldRev = `grep Revision svnInfo.h`
   set oldRev = ($oldRev)
   set oldRev = `echo $oldRev[4] | sed 's/"$//'`
endif
#
# IF the file is missing, or the old revision number
# is doifferent from the current one, make a new svnInfo.h
if ($rev != $oldRev) then
set svnDate = `svn info .. | grep "Last Changed Date"`
set svnURL = `svn info .. | grep "URL:"`
cat > svnInfo.h <<EOF
#ifndef SVNINFOINC
#define SVNINFOINC
#define SVNREVISION "$svnRevision"
#define SVNLASTCHANGEDDATE "$svnDate"
#define SVNURL "$svnURL"
#endif
EOF
#
# This setup is not smart enough to rebuild the files that include the
# new svnInfo.h, remove them until we come up with something better.
echo $oldRev
echo $rev
rm ../src/filter/nimbus.o
rm ../src/filter/cb_main.o
rm ../src/filter/netcdf.o
rm ../src/filter/nimbus
#
echo "Current subversion revision is $rev"





