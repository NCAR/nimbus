#!/bin/sh

PATH=$PATH:/opt/ads3/x86/bin

export ADS3_CONFIG=${JLOCAL}

sync_server $*
