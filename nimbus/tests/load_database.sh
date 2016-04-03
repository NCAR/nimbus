#! /bin/sh

# Run sync_server on raw data file and send it to nimbus running in
# real-time, which inserts the data into a postgres database.
#
# Someday it might be better to add this functionality to nimbus.  I think
# it's just a matter of getting nimbus to read from a raw file instead of
# the sync_server port even though running in real-time mode.  And then it
# should be possible to specify time periods to load also.

# This excerpt from the nidas config specifies the sync record
# output which nimbus expects:
#
#             <processor class="raf.SyncRecordGenerator">
#                 <output class="SampleOutputStream">
#                     <socket port="30001" type="server"/>
#                 </output>
#             </processor>
#
# localhost:30001

nimbus=../src/filter/nimbus
pgtdb="python ../../caledit/site_scons/site_tools/postgres_testdb.py"

usage()
{
    cat <<EOF
load_database <xml> <raw-data-file>

Run sync_server on the raw data file for the given xml file, and then run
nimbus in real-time mode as the sync_server client.  Nimbus processes the
sync records and submits them to the real-time test database.

EOF
}

if [ $# -ne 2 ]; then
    usage
    exit 1
fi

xml="$1"
raw="$2"

# When run in real-time on the plane, the start_data_acq script sets the
# FLIGHT environment variable, from which Project::getFlightName() gets the
# flight name.  Here we need to set it directly from the filename.  It's
# not entirely necessary, but one effect is that by default the sample-rate
# tables are not inserted when the plane was actually flying (tf, rf, ff).

flight=`echo "$raw" | sed -e 's/.*\([rftc]f[0-9][0-9]\)\.ads/\1/'`
export FLIGHT="$flight"
echo "Setting FLIGHT to $flight"

echo "***** Starting the test database..."
$pgtdb init
eval `$pgtdb env`

echo "***** Running sync_server..."
set -x
sync_server -l 2 -x "$xml" "$raw" &
set +x
sspid="$!"
sleep 5
echo "***** Running nimbus..."
(set -x; $nimbus --loglevel debug --xml "$xml" -rt3 -y)
status=$?

kill $sspid

cat <<EOF

The test database may not be stopped.  Run this command to set the
environment variables to connect to it:

   eval \`$pgtdb env\`

Run this command to stop it:

   $pgtdb stop

EOF

exit $status
