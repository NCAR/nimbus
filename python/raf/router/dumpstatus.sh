#! /bin/sh

rooturl="http://192.168.99.1/"
creds="--user=admin --password=password"

if [ $# -ge 1 ]; then
    rooturl="$1"
fi

wget -O - $creds $rooturl/sysstatus.html > sysstatus.html
# wget -O - $creds $rooturl/mtenSysStatistics.html
wget -O - $creds $rooturl/mtenstatisticTable.html > statsTable.html
wget -O - $creds $rooturl/pppoestatus.html > pppoestatus.html
