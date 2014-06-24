Summary: Spec file for xpms2d
Name: xpms2d
Version: 2.6
Release: 2
License: GPL
Group: System Environment/Daemons
Url: http://www.eol.ucar.edu/
Packager: Chris Webster <cjw@ucar.edu>
# becomes RPM_BUILD_ROOT
BuildRoot:  %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Vendor: UCAR
BuildArch: x86_64
Requires: openmotif libpng
Source: ftp://ftp.eol.ucar.edu/pub/archive/RAF-src/%{name}.tar.gz

%description
Configuration for NCAR-EOL xpms2d display for OAP probes.

%prep
%setup -n %{name}

%build
cd src
make

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}%{_bindir}
cp src/%{name} %{buildroot}%{_bindir}

%post


%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%{_bindir}/%{name}

%changelog

* Wed Dec 12 2013 Chris Webster <cjw@ucar.edu> - 2.6-2
- bug fixes.
- Magnify box issue - XButtonEvent needed update, wasn't detecting if NumLock was off (or on).
- Merge in branch file positioning.
- Replace fopen64() freado64(), fseeko64(), etc with fopen() fread(), fseeko()
- Clean up Mac build for Mountian Lion
* Wed Jul 13 2011 Chris Webster <cjw@ucar.edu> - 2.6-1
- initial version