%define nidas_prefix /opt/nidas

Summary: CMIGITS binary plugin for NIDAS
Name: cmigits-nidas-plugin
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
%setup -q -n cmigits

%build
scons -j 4 BUILDS="host arm armbe"
 
%install
rm -rf $RPM_BUILD_ROOT
# scons -j 4 BUILDS="host arm armbe" PREFIX=${RPM_BUILD_ROOT}%{nidas_prefix} install

# install -d ${RPM_BUILD_ROOT}%{nidas_prefix}/%{_lib}
install -d ${RPM_BUILD_ROOT}%{nidas_prefix}/%{_lib}
install -t ${RPM_BUILD_ROOT}%{nidas_prefix}/%{_lib} build/libnidas* 
install -d ${RPM_BUILD_ROOT}%{nidas_prefix}/arm/lib
install -t ${RPM_BUILD_ROOT}%{nidas_prefix}/arm/lib build_arm/libnidas* 
install -d ${RPM_BUILD_ROOT}%{nidas_prefix}/armbe/lib
install -t ${RPM_BUILD_ROOT}%{nidas_prefix}/armbe/lib build_armbe/libnidas* 

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(0775,root,root,2775)
%{nidas_prefix}/%{_lib}/libnidas_dynld_raf_CMIGITS3_Serial.so*
%{nidas_prefix}/arm/lib/libnidas_dynld_raf_CMIGITS3_Serial.so*
%{nidas_prefix}/armbe/lib/libnidas_dynld_raf_CMIGITS3_Serial.so*

%changelog
* Wed Aug 06 2014 Gordon Maclean <maclean@ucar.edu> 1.%{version}-%{release}
- Initial version
