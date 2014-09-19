%define nidas_prefix /opt/nidas

Summary: CMIGITS binary plugin for NIDAS
Name: cmigits-nidas
Version: 1.%{version}
Release: %{release}%{?dist}
License: GPL
Group: Applications/Engineering
Url: http://www.eol.ucar.edu/
Vendor: UCAR
Source: %{name}-%{version}.tar.gz
BuildRequires: gcc-c++ scons subversion nidas-devel
Requires: nidas-libs
BuildRoot: %{_topdir}/%{name}-%{version}-root
# Allow this package to be relocatable to other places than /opt/nidas
# rpm --relocate /opt/nidas=/usr
Prefix: %{nidas_prefix}
%description
CMIGITS binary plugin for NIDAS.

%prep
%setup -q -n cmigits -c

%build
scons -j 4 BUILDS="host arm armbe"
 
%install
rm -rf $RPM_BUILD_ROOT
scons -j 4 BUILDS="host arm armbe" INSTALLROOT=${RPM_BUILD_ROOT}%{nidas_prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%post 
/sbin/ldconfig

%files
%defattr(0775,root,root,2775)
%{nidas_prefix}/%{_lib}/libnidas_dynld_raf_CMIGITS3_Serial.so*
%{nidas_prefix}/arm/lib/libnidas_dynld_raf_CMIGITS3_Serial.so*
%{nidas_prefix}/armbe/lib/libnidas_dynld_raf_CMIGITS3_Serial.so*

%changelog
* Wed Aug 06 2014 Gordon Maclean <maclean@ucar.edu> 1.%{version}-%{release}
- Initial version
