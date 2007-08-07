#!/bin/sh

# This script is run from the LDM pqact.conf file on sql.gz files
# sent down from the aircraft.

# Host may change.  Over-ridden in parse_from_ac.sh
#
export PGHOST=lenado.atd.ucar.edu
export PGUSER=ads
export PGDATABASE=real-time

# static_psql is just a statically linked version of the psql(1) command
# distributed with PostGreSQL.  The exposed host does not have PostGreS
# installed.

gunzip - | /h/9/cjw/bin/parse_from_ac.sh

# hack for ProgSci rf01 data: add single quotes before { and after };
# It only adds quotes if necessary - it won't add quotes
# if they're already there.
#gunzip - |sed -r -e "s/([^'])\{/\1'{/g" -e "s/\}([^'])/}'\1/g" | /h/9/cjw/bin/static_psql
