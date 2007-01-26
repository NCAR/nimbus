#!/bin/sh

PATH=$PATH:/opt/nidas/x86/bin

export ADS3_CONFIG=${JLOCAL}

sync_server $*
