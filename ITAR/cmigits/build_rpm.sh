#!/bin/sh

script=`basename $0`
dir=`dirname $0`

pkg=cmigits-nidas-plugin

install=false

while [ $# -gt 0 ]; do
    case $1 in
        -i)
            install="true"
            ;;
    esac
    shift
done

cd $dir || exit 1
source repo_scripts/repo_funcs.sh || exit 1

topdir=${TOPDIR:-`get_rpm_topdir`}

# echo "topdir=$topdir"
[ -d $topdir/SOURCES ] || mkdir -p $topdir/SOURCES
[ -d $topdir/BUILD ] || mkdir -p $topdir/BUILD
[ -d $topdir/SRPMS ] || mkdir -p $topdir/SRPMS
[ -d $topdir/RPMS ] || mkdir -p $topdir/RPMS

rroot=`get_eol_repo_root`

log=`mktemp /tmp/${script}_XXXXXX.log`
trap "{ rm -f $log; }" EXIT

set -o pipefail

# pkg-1.version-release
# version will be the svnversion of the cmigits working directory
# release will be the svnversion of NIDAS, as read from
# /opt/nidas/include/nidas/SvnInfo.h
get_nidas_revision() 
{
    awk '/SVNREVISION/{print $3}' /opt/nidas/include/nidas/SvnInfo.h | sed 's/"//g'
}

get_svnversion() 
{
    # discard M,S,P, mixed versions
    v=$(svnversion . | sed 's/:.*$//' | sed s/[A-Z]//g)
    [ "$v" == exported ] && v=1
    echo $v | fgrep -q committed && v=1
    echo $v
}

get_rpmversion() 
{
    awk '/^Version:/{print $2; exit 0}' $1
}

# set -x

# Jenkins defines SVN_REVISION
version=${SVN_REVISION:=$(get_svnversion)}

release=$(get_nidas_revision)

fullversion=$(get_rpmversion ${pkg}.spec | sed "s/%{version}/$version/")


cd ..
tar czf $topdir/SOURCES/${pkg}-${fullversion}.tar.gz --exclude .svn \
        cmigits/*.h cmigits/*.cc cmigits/SC* cmigits/site_scons || exit $?
cd -

# set debug_package to %{nil} to suppress the build of the debug package,
# which avoids this failure:

rpmbuild -ba \
    --define "_topdir $topdir" \
    --define "_unpackaged_files_terminate_build 0" \
    --define "debug_package %{nil}" \
    --define "release $release" \
    --define "version $version" \
    ${pkg}.spec 2>&1 | tee -a $log  || exit $?


echo "RPMS:"
egrep "^Wrote:" $log
rpms=`egrep '^Wrote:' $log | egrep RPMS/ | awk '{print $2}'`

if $install && [ -d $rroot ]; then
    echo "Moving rpms to $rroot"
    copy_rpms_to_eol_repo $rpms
elif $install; then
    echo "$rroot not found. Leaving RPMS in $topdir"
else
    echo "-i or -r options not specified. RPMS will not be installed"
fi

# print out warnings: and the following file list
sed -n '
    /^warning:/{
    : next
    p
    n
    /^ /b next
    }
    ' $log
