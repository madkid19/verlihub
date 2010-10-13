Summary: Direct Connect (p2p) Server
Name: verlihub
Version: 0.9.9a 
Release: 1
License: GPL
Group: Applications/Internet
Requires: MySQL
Requires: libmysql12
Requires: libpcre0
Requires: GeoIP >= 1.3.1
BuildRequires:libmysql12-devel
BuildRequires:libpcre0-devel
BuildRequires:GeoIP-devel
Source: ./verlihub-%{version}.tar.gz
URL: http://verlihub.sourceforge.net
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%define _unpackaged_files_terminate_build 0

%description
This progam let's you have a p2p server for file sharing. It's powerfull, flexible. Plugins are available.
Discussion forum - http://www.verlihubforums.com
Discussion DC hub - hub.verlihub-project.org:7777
Manual Wiki Pages - http://www.verlihub-project.org

%prep
%setup

%build
echo RPM_BUILD_ROOT = $RPM_BUILD_ROOT
%configure --with-cflags="$RPM_OPT_FLAGS"
make DESTDIR=$RPM_BUILD_ROOT  RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT

make DESTDIR=$RPM_BUILD_ROOT  install

find $RPM_BUILD_ROOT -type f -print -or -type l -print| 
        sed -e "s@^$RPM_BUILD_ROOT@@g" \
	    -e 's@\(/man.*\.[1-9]\)$@\1.bz2@' > %{name}-filelist

%post
ldconfig

%postun
ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files -f %{name}-filelist
%defattr(-,root,root,0755)
%doc AUTHORS README COPYING ChangeLog TODO

%changelog

