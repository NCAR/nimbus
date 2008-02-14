#!/bin/sh

# This script is called from the ground based LDM pqacct.conf file.

# Adjust host as needed.
export PGHOST=lenado.eol.ucar.edu
export PGHOST=eol-rt-data.guest.ucar.edu
export PGUSER=ads
export PGDATABASE=real-time

# parse_from_ac.sh may be installed elsewhere.
gunzip - | /h/eol/cjw/bin/parse_from_ac.sh
