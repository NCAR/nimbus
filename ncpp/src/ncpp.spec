Summary: Spec file for ncpp
Name: ncpp
Version: 2.2
Release: 2
License: GPL
Group: System Environment/Daemons
Url: http://www.eol.ucar.edu/
Packager: Chris Webster <cjw@ucar.edu>
# becomes RPM_BUILD_ROOT
BuildRoot:  %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Vendor: UCAR
BuildArch: x86_64
Requires: openmotif netcdf libpng
Source: ftp://ftp.eol.ucar.edu/pub/archive/RAF-src/%{name}.tar.gz

%description
Configuration for NCAR-EOL ncpp display for size-distribution data.

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
* Wed Jul 13 2011 Chris Webster <cjw@ucar.edu> - 1.0-1
- initial version
