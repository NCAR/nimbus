#!/bin/sh

PGHOST=lenado.atd.ucar.edu ; export PGHOST
PGUSER=ads ; export PGUSER
PGDATABASE=real-time ; export PGDATABASE

# static_psql is just a statically linked version of the psql(1) command
# distributed with PostGreSQL.  The exposed host does not have PostGreS
# installed.

gunzip - | /h/9/cjw/bin/parse_from_ac.sh

# hack for ProgSci rf01 data: add single quotes before { and after };
# It only adds quotes if necessary - it won't add quotes
# if they're already there.
#gunzip - |sed -r -e "s/([^'])\{/\1'{/g" -e "s/\}([^'])/}'\1/g" | /h/9/cjw/bin/static_psql
