%global _default_patch_fuzz 2

%global _hardened_build 1
%{?perl_default_filter}

# disable collectd debug by default
%bcond_with debug

# plugins
%define with_aggregation 0%{!?_without_aggregation:1}
%define with_amqp 0%{!?_without_amqp:1}
%define with_amqp1 0%{!?_without_amqp1:1}
%define with_apache 0%{!?_without_apache:1}
%define with_apcups 0%{!?_without_apcups:1}
%define with_apple_sensors 0%{!?_without_apple_sensors:0}
%define with_aquaero 0%{!?_without_aquaero:0}
%define with_ascent 0%{!?_without_ascent:1}
%define with_barometer 0%{!?_without_barometer:0}
%define with_battery 0%{!?_without_battery:1}
%define with_bind 0%{!?_without_bind:1}
%define with_buddyinfo 0%{!?_without_buddyinfo:1}
%define with_capabilities 0%{!?_without_capabilities:1}
%define with_ceph 0%{!?_without_ceph:1}
%define with_cgroups 0%{!?_without_cgroups:1}
%define with_check_uptime 0%{!?_without_check_uptime:1}
%define with_chrony 0%{!?_without_chrony:1}
%define with_connectivity 0%{!?_without_connectivity:1}
%define with_conntrack 0%{!?_without_conntrack:1}
%define with_contextswitch 0%{!?_without_contextswitch:1}
%define with_cpu 0%{!?_without_cpu:1}
%define with_cpufreq 0%{!?_without_cpufreq:1}
%define with_cpusleep 0%{!?_without_cpusleep:1}
%define with_csv 0%{!?_without_csv:1}
%define with_curl 0%{!?_without_curl:1}
%define with_curl_json 0%{!?_without_curl_json:1}
%define with_curl_xml 0%{!?_without_curl_xml:1}
%define with_dbi 0%{!?_without_dbi:1}
%define with_dcpmm 0%{!?_without_dcpmm:0}
%define with_df 0%{!?_without_df:1}
%define with_disk 0%{!?_without_disk:1}
%define with_diskadapter 0%{!?_without_diskadapter:0}
%define with_diskpath 0%{!?_without_diskpath:0}
%define with_dns 0%{!?_without_dns:1}
%define with_dpdk_telemetry 0%{!?_without_dpdk_telemetry:1}
%define with_dpdkevents 0%{!?_without_dpdkevents:0}
%define with_dpdkstat 0%{!?_without_dpdkstat:0}
%define with_drbd 0%{!?_without_drbd:1}
%define with_email 0%{!?_without_email:1}
%define with_entropy 0%{!?_without_entropy:1}
%define with_ethstat 0%{!?_without_ethstat:1}
%define with_exec 0%{!?_without_exec:1}
%define with_fhcount 0%{!?_without_fhcount:1}
%define with_filecount 0%{!?_without_filecount:1}
%define with_fscache 0%{!?_without_fscache:1}
%define with_gmond 0%{!?_without_gmond:0}
%define with_gps 0%{!?_without_gps:1}
%define with_gpu_nvidia 0%{!?_without_gpu_nvidia:0}
%define with_grpc 0%{!?_without_grpc:0}
%define with_hba 0%{!?_without_hba:0}
%define with_hddtemp 0%{!?_without_hddtemp:1}
%define with_hugepages 0%{!?_without_hugepages:1}
%define with_infiniband 0%{!?_without_infiniband:1}
%define with_intel_pmu 0%{!?_without_intel_pmu:0}
%define with_intel_rdt 0%{!?_without_intel_rdt:0}
%define with_interface 0%{!?_without_interface:1}
%define with_ipc 0%{!?_without_ipc:1}
%define with_ipmi 0%{!?_without_ipmi:1}
%define with_ipstats 0%{!?_without_ipstats:0}
%define with_iptables 0%{!?_without_iptables:1}
%define with_ipvs 0%{!?_without_ipvs:1}
%define with_irq 0%{!?_without_irq:1}
%define with_java 0%{!?_without_java:1}
%define with_load 0%{!?_without_load:1}
%define with_log_logstash 0%{!?_without_log_logstash:1}
%define with_logfile 0%{!?_without_logfile:1}
%define with_logparser 0%{!?_without_logparser:1}
%define with_lpar 0%{!?_without_lpar:0}
%define with_lua 0%{!?_without_lua:1}
%define with_lvm 0%{!?_without_lvm:0}
%define with_madwifi 0%{!?_without_madwifi:1}
%define with_mbmon 0%{!?_without_mbmon:1}
%define with_mcelog 0%{!?_without_mcelog:1}
%define with_md 0%{!?_without_md:1}
%define with_mdevents 0%{!?_without_mdevents:1}
%define with_memcachec 0%{!?_without_memcachec:1}
%define with_memcached 0%{!?_without_memcached:1}
%define with_memory 0%{!?_without_memory:1}
%define with_mic 0%{!?_without_mic:0}
%define with_mmc 0%{!?_without_mmc:1}
%define with_modbus 0%{!?_without_modbus:1}
%define with_mongodb 0%{!?_without_mongodb:1}
%define with_mqtt 0%{!?_without_mqtt:1}
%define with_multimeter 0%{!?_without_multimeter:1}
%define with_mysql 0%{!?_without_mysql:1}
%define with_netlink 0%{!?_without_netlink:1}
%define with_netapp 0%{!?_without_netapp:0}
%define with_netstat_udp 0%{!?_without_netstat_udp:0}
%define with_network 0%{!?_without_network:1}
%define with_nfs 0%{!?_without_nfs:1}
%define with_nginx 0%{!?_without_nginx:1}
%define with_notify_dbi 0%{!?_without_notify_dbi:1}
%define with_notify_desktop 0%{!?_without_notify_desktop:1}
%define with_notify_email 0%{!?_without_notify_email:1}
%define with_notify_nagios 0%{!?_without_notify_nagios:1}
%define with_notify_snmp 0%{!?_without_notify_snmp:1}
%define with_ntpd 0%{!?_without_ntpd:1}
%define with_numa 0%{!?_without_numa:1}
%define with_nut 0%{!?_without_nut:1}
%define with_odbc 0%{!?_without_odbc:1}
%define with_olsrd 0%{!?_without_olsrd:1}
%define with_onewire 0%{!?_without_onewire:0}
%define with_openldap 0%{!?_without_openldap:1}
%define with_openvpn 0%{!?_without_openvpn:1}
%define with_oracle 0%{!?_without_oracle:0}
%define with_ovs_events 0%{!?_without_ovs_events:1}
%define with_ovs_stats 0%{!?_without_ovs_stats:1}
%define with_pcie_errors 0%{!?_without_pcie_errors:1}
%define with_perl 0%{!?_without_perl:1}
%define with_pf 0%{!?_without_pf:0}
%define with_pinba 0%{!?_without_pinba:1}
%define with_ping 0%{!?_without_ping:1}
%define with_postgresql 0%{!?_without_postgresql:1}
%define with_powerdns 0%{!?_without_powerdns:1}
%define with_processes 0%{!?_without_processes:1}
%define with_procevent 0%{!?_without_procevent:1}
%define with_protocols 0%{!?_without_protocols:1}
%define with_python 0%{!?_without_python:1}
%define with_ras 0%{!?_without_ras:0}
%define with_redis 0%{!?_without_redis:1}
%define with_redfish 0%{!?_without_redfish:0}
%define with_routeros 0%{!?_without_routeros:0}
%define with_rrdcached 0%{!?_without_rrdcached:1}
%define with_rrdtool 0%{!?_without_rrdtool:1}
%define with_sensors 0%{!?_without_sensors:1}
%define with_serial 0%{!?_without_serial:1}
%define with_sigrok 0%{!?_without_sigrok:0}
%define with_slurm 0%{!?_without_slurm:0}
%define with_smart 0%{!?_without_smart:1}
%define with_snmp 0%{!?_without_snmp:1}
%define with_snmp_agent 0%{!?_without_snmp_agent:1}
%define with_statsd 0%{!?_without_statsd:1}
%define with_swap 0%{!?_without_swap:1}
%define with_synproxy 0%{!?_without_synproxy:1}
%define with_sysevent 0%{!?_without_sysevent:1}
%define with_syslog 0%{!?_without_syslog:1}
%define with_table 0%{!?_without_table:1}
%define with_tail 0%{!?_without_tail:1}
%define with_tail_csv 0%{!?_without_tail_csv:1}
%define with_tape 0%{!?_without_tape:0}
%define with_tcpconns 0%{!?_without_tcpconns:1}
%define with_teamspeak2 0%{!?_without_teamspeak2:1}
%define with_ted 0%{!?_without_ted:1}
%define with_thermal 0%{!?_without_thermal:1}
%define with_threshold 0%{!?_without_threshold:1}
%define with_tokyotyrant 0%{!?_without_tokyotyrant:0}
%define with_turbostat 0%{!?_without_turbostat:1}
%define with_unixsock 0%{!?_without_unixsock:1}
%define with_uptime 0%{!?_without_uptime:1}
%define with_ubi 0%{!?_without_ubi:1}
%define with_users 0%{!?_without_users:1}
%define with_uuid 0%{!?_without_uuid:1}
%define with_varnish 0%{!?_without_varnish:0}
%define with_virt 0%{!?_without_virt:1}
%define with_vmem 0%{!?_without_vmem:1}
%define with_vserver 0%{!?_without_vserver:1}
%define with_wireless 0%{!?_without_wireless:1}
%define with_wlm 0%{!?_without_wlm:0}
%define with_wpar 0%{!?_without_wpar:0}
%define with_write_graphite 0%{!?_without_write_graphite:1}
%define with_write_http 0%{!?_without_write_http:1}
%define with_write_influxdb_udp 0%{!?_without_write_influxdb_udp:1}
%define with_write_kafka 0%{!?_without_write_kafka:1}
%define with_write_log 0%{!?_without_write_log:1}
%define with_write_mongodb 0%{!?_without_write_mongodb:1}
%define with_write_prometheus 0%{!?_without_write_prometheus:1}
%define with_write_redis 0%{!?_without_write_redis:1}
%define with_write_riemann 0%{!?_without_write_riemann:1}
%define with_write_stackdriver 0%{!?_without_write_stackdriver:0}
%define with_write_sensu 0%{!?_without_write_sensu:1}
%define with_write_syslog 0%{!?_without_write_syslog:1}
%define with_write_tsdb 0%{!?_without_write_tsdb:1}
%define with_xencpu 0%{!?_without_xencpu:0}
%define with_xmms 0%{!?_without_xmms:0%{?_has_xmms}}
%define with_zfs_arc 0%{!?_without_zfs_arc:1}
%define with_zone 0%{!?_without_zone:0}
%define with_zookeeper 0%{!?_without_zookeeper:1}

# Plugins not buildable on RHEL 7
%if 0%{?rhel} && 0%{?rhel} == 7
%define with_mongodb 0
%endif

# Plugins not buildable on RHEL 8
%if 0%{?rhel} && 0%{?rhel} == 8
%define with_modbus 0
%endif

# Plugins not buildable on RHEL 9
%if 0%{?rhel} && 0%{?rhel} == 9
%define with_amqp1 0
%define with_dbi 0
%define with_iptables 0
%define with_perl 0
%define with_modbus 0
%define with_notify_dbi 0
%endif

Summary:        Statistics collection and monitoring daemon
Name:           collectd
Version:        %{?version}
Release:        %{?release}%{?dist}
URL:            https://collectd.org
Source0:        %{name}-%{version}.tar.gz
License:        GPLv2
Group:	        System Environment/Daemons
BuildRoot:      %{_tmppath}/%{name}-%{version}-root
BuildRequires:  libgcrypt-devel, kernel-headers, libcap-devel, which
BuildRequires:  flex, bison
Vendor:         collectd development team <collectd@verplant.org>

%if 0%{?fedora} || 0%{?rhel} >= 7
BuildRequires:      xfsprogs-devel
%{?systemd_requires}
BuildRequires:      systemd
BuildRequires:      libudev-devel
Requires:	          collectd-selinux
%else
Requires(post):     chkconfig
Requires(preun):    chkconfig, initscripts
Requires(postun):   initscripts
%endif

#Source1: clean.sh
#Source2: build.sh

%description
collectd is a small daemon which collects system information periodically and
provides mechanisms to monitor and store the values in a variety of ways. It
is written in C for performance. Since the daemon doesn't need to start up
every time it wants to update the values it's very fast and easy on the
system. Also, the statistics are very fine grained since the files are updated
every 10 seconds by default.

%if %{with_amqp}
%package amqp
Summary:	AMQP 0.9 plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	librabbitmq-devel
%description amqp
The AMQP 0.9 plugin transmits or receives values collected by collectd via the
Advanced Message Queuing Protocol v0.9 (AMQP).
%endif

%if %{with_amqp1}
%package amqp1
Summary:	AMQP 1.0 plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	qpid-proton-c-devel
%description amqp1
The AMQP 1.0 plugin transmits or receives values collected by collectd via the
Advanced Message Queuing Protocol v1.0 (AMQP1).
%endif

%if %{with_apache}
%package apache
Summary:	Apache plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	curl-devel
%description apache
This plugin collects data provided by Apache's `mod_status'.
%endif

%if %{with_aquaero}
%package aquaero
Summary:	aquaero plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%description aquaero
Various sensors in the Aquaero 5 watercooling board made by Aquacomputer.
%endif

%if %{with_ascent}
%package ascent
Summary:	Ascent plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libxml2-devel, curl-devel
%description ascent
The Ascent plugin reads and parses the statistics page of Ascent, a free and
open-source server software for the game World of Warcraft by Blizzard
Entertainment.
%endif

%if %{with_barometer}
%package barometer
Summary:       barometer plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
%description barometer
Collects pressure and temperature from digital barometers.
%endif

%if %{with_bind}
%package bind
Summary:	Bind plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libxml2-devel, curl-devel
%description bind
The BIND plugin retrieves this information that's encoded in XML and provided
via HTTP and submits the values to collectd.
%endif

%if %{with_capabilities}
%package capabilities
Summary:       Capabilities plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: jansson-devel
%description capabilities
Capabilities plugin for collectd
%endif

%if %{with_ceph}
%package ceph
Summary:       Ceph plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: yajl-devel
%description ceph
Ceph plugin for collectd
%endif

%if %{with_chrony}
%package chrony
Summary:       Chrony plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
%description chrony
Chrony plugin for collectd
%endif

%if %{with_connectivity}
%package connectivity
Summary:       Connectivity plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: libmnl-devel, yajl-devel
%description connectivity
Monitors network interface up/down status via netlink library.
%endif

%if %{with_curl}
%package curl
Summary:	Curl plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	curl-devel
%description curl
The cURL plugin uses libcurl to read files and then parses them according to
the configuration.
%endif

%if %{with_curl_json}
%package curl_json
Summary:	Curl_json plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	curl-devel, yajl-devel
%description curl_json
The cURL-JSON plugin queries JavaScript Object Notation (JSON) data using the
cURL library and parses it according to the user's configuration.
%endif

%if %{with_curl_xml}
%package curl_xml
Summary:	Curl_xml plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	curl-devel, libxml2-devel
%description curl_xml
The cURL-XML plugin reads files using libcurl and parses it as Extensible
Markup Language (XML).
%endif

%if %{with_dbi}
%package dbi
Summary:	DBI plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libdbi-devel
%description dbi
The DBI plugin uses libdbi, a database abstraction library, to execute SQL
statements on a database and read back the result.
%endif

%if %{with_dns}
%package dns
Summary:	DNS plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}, libpcap >= 1.0
BuildRequires:	libpcap-devel >= 1.0
%description dns
The DNS plugin has a similar functionality to dnstop: It uses libpcap to get a
copy of all traffic from/to port UDP/53 (that's the DNS port), interprets the
packets and collects statistics of your DNS traffic.
%endif

%if %{with_email}
%package email
Summary:	Email plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}, spamassassin
%description email
This plugin collects data provided by spamassassin.
%endif

%if %{with_gmond}
%package gmond
Summary:	Gmond plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	ganglia-devel
%description gmond
The gmond plugin subscribes to a Multicast group to receive data from gmond,
the client daemon of the Ganglia project.
%endif

%if %{with_gps}
%package gps
Summary:	GPS plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	gpsd-devel
%description gps
This plugin monitor gps related data through gpsd.
%endif

%if %{with_grpc}
%package grpc
Summary:	GRPC plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	protobuf-compiler
%description grpc
This plugin embeds a gRPC server into Collectd.
%endif

%if %{with_hddtemp}
%package hddtemp
Summary:	Hddtemp plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}, hddtemp
%description hddtemp
The HDDTemp plugin collects the temperature of hard disks. The temperatures are
provided via SMART and queried by the external hddtemp daemon.
%endif

%if %{with_intel_pmu}
%package intel_pmu
Summary:	Intel PMU plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%description intel_pmu
The intel_pmu plugin reads performance counters provided by the Linux
kernel perf interface.
%endif

%if %{with_intel_rdt}
%package intel_rdt
Summary:	Intel RDT plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	intel-cmt-cat
%description intel_rdt
The intel_rdt plugin collects information provided by monitoring features of
Intel Resource Director Technology (Intel(R) RDT).
%endif

%if %{with_ipmi}
%package ipmi
Summary:	IPMI plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	OpenIPMI-devel
%description ipmi
The IPMI plugin uses the OpenIPMI library to read hardware sensors from servers
using the Intelligent Platform Management Interface (IPMI).
%endif

%if %{with_iptables}
%package iptables
Summary:	IPtables plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	iptables-devel
%description iptables
The IPtables plugin can gather statistics from your ip_tables based packet
filter (aka. firewall) for both the IPv4 and the IPv6 protocol. It can collect
the byte- and packet-counters of selected rules and submit them to collectd.
%endif

%if %{with_java}
%package java
Summary:	Java plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	java-devel >= 1.6, jpackage-utils >= 1.6
Requires:	java >= 1.6, jpackage-utils >= 1.6
%description java
This plugin for collectd allows plugins to be written in Java and executed
in an embedded JVM.
%endif

%if %{with_log_logstash}
%package log_logstash
Summary:       log_logstash plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: yajl-devel
%description log_logstash
This plugin logs in logstash JSON format
%endif

%if %{with_lua}
%package lua
Summary:	Lua plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	lua-devel
%description lua
The Lua plugin embeds a Lua interpreter into collectd and exposes the
application programming interface (API) to Lua scripts.
%endif

%if %{with_lvm}
%package lvm
Summary:	LVM plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	lvm2-devel
%description lvm
This plugin collects size of “Logical Volumes” (LV) and “Volume Groups” (VG)
of Linux' “Logical Volume Manager” (LVM).
%endif

%if %{with_mcelog}
%package mcelog
Summary:	Mcelog plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%description mcelog
This plugin monitors machine check exceptions reported by mcelog and generates
appropriate notifications when machine check exceptions are detected.
%endif

%if %{with_memcachec}
%package memcachec
Summary:	Memcachec plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libmemcached-devel
%description memcachec
The Memcachec plugin uses libmemcached to read statistics from a Memcached
instance. Note that another plugin, named `memcached', exists and does a
similar job, without requiring the installation of libmemcached.
%endif

%if %{with_mic}
%package mic
Summary:	mic plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%description mic
The mic plugin collects CPU usage, memory usage, temperatures and power
consumption from Intel Many Integrated Core (MIC) CPUs.
%endif

%if %{with_modbus}
%package modbus
Summary:       modbus plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libmodbus-devel
%description modbus
The modbus plugin collects values from Modbus/TCP enabled devices
%endif

%if %{with_mysql}
%package mysql
Summary:	MySQL plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%if 0%{?rhel} && 0%{?rhel} > 5 && 0%{?rhel} < 8 
BuildRequires:	mysql-devel
%endif
%if 0%{?rhel} && 0%{?rhel} > 7 && 0%{?rhel} < 10
BuildRequires:	mariadb-connector-c-devel
%endif
%description mysql
MySQL querying plugin. This plugin provides data of issued commands, called
handlers and database traffic.
%endif

%if %{with_mongodb}
%package mongodb
Summary:	mongodb plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	mongo-c-driver-devel
%description mongodb
The mongodb plugin collect mongodb stats.
%endif

%if %{with_mqtt}
%package mqtt
Summary:	mqtt plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	mosquitto-devel
%description mqtt
The MQTT plugin publishes and subscribes to MQTT topics.
%endif

%if %{with_netlink}
%package netlink
Summary:	netlink plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libmnl-devel, iproute-devel
%description netlink
The netlink plugin collects detailed network interface and routing statistics.
%endif

%if %{with_nginx}
%package nginx
Summary:	Nginx plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	curl-devel
%description nginx
This plugin gets data provided by nginx.
%endif

%if %{with_notify_dbi}
%package notify_dbi
Summary:        A libdbi notifier for collectd.
Group:          System Environment/Daemons
Requires:       %{name}%{?_isa} = %{version}-%{release}
BuildRequires:  libdbi-devel
%description notify_dbi
The notify_dbi plugin dispatch notifications unsing libdbi to a Database.
%endif

%if %{with_notify_desktop}
%package notify_desktop
Summary:	Notify_desktop plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libnotify-devel, gtk2-devel
%description notify_desktop
The Notify Desktop plugin uses libnotify to display notifications to the user
via the desktop notification specification, i. e. on an X display.
%endif

%if %{with_notify_email}
%package notify_email
Summary:	Notify_email plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libesmtp-devel
%description notify_email
The Notify Email plugin uses libESMTP to send notifications to a configured
email address.
%endif

%if %{with_notify_snmp}
%package notify_snmp
Summary:        A SNMP notifier for collectd.
Group:          System Environment/Daemons
Requires:       %{name}%{?_isa} = %{version}-%{release}
BuildRequires:	net-snmp-devel
%description notify_snmp
The notify_snmp plugin dispatch notifications unsing SNMP
%endif

%if %{with_nut}
%package nut
Summary:	Nut plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	nut-devel
%description nut
This plugin for collectd provides Network UPS Tools support.
%endif

%if %{with_odbc}
%package odbc
Summary:	ODBC plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	unixODBC-devel
%description odbc
The ODBC plugin uses unixodbc, a database abstraction library, to execute SQL
statements on a database and read back the result.
%endif

%if %{with_openldap}
%package openldap
Summary:       Openldap plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: openldap-devel
%description openldap
This plugin reads monitoring information from OpenLDAP's cn=Monitor subtree.
%endif

%if %{with_oracle}
%package oracle
Summary:	Oracle plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:oracle-instantclient19.6-devel
%description oracle
The Oracle plugin connects to and executes SQL statements on a Oracle
database.
%endif

%if %{with_ovs_events}
%package ovs_events
Summary:       Open vSwitch events plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: yajl-devel
%description ovs_events
This plugin monitors the link status of Open vSwitch (OVS) connected
interfaces, dispatches the values to collectd and sends notifications
whenever a link state change occurs in the OVS database.
%endif

%if %{with_ovs_stats}
%package ovs_stats
Summary:       Open vSwitch statistics plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: yajl-devel
%description ovs_stats
This plugin collects statictics of OVS connected bridges and
interfaces.
%endif

%if %{with_perl}
%package perl
Summary:	Perl plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
Requires:	perl(:MODULE_COMPAT_%(eval "`%{__perl} -V:version`"; echo $version))
    %if 0%{?rhel} && 0%{?rhel} < 6
BuildRequires:	perl
    %else
BuildRequires:	perl-ExtUtils-Embed
    %endif
%description perl
The Perl plugin embeds a Perl interpreter into collectd and exposes the
application programming interface (API) to Perl-scripts.
%endif

%if %{with_pcie_errors}
%package pcie_errors
Summary:	PCI Express errors plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%description pcie_errors
The pcie_errors plugin collects PCI Express errors from Device Status in Capability
structure and from Advanced Error Reporting Extended Capability.
%endif

%if %{with_pinba}
%package pinba
Summary:	Pinba plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	protobuf-c-devel
%description pinba
The Pinba plugin receives and dispatches timing values from Pinba, a profiling
extension for PHP.
%endif

%if %{with_ping}
%package ping
Summary:	Ping plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	liboping-devel
%description ping
The Ping plugin measures network latency using ICMP “echo requests”, usually
known as “ping”.
%endif

%if %{with_postgresql}
%package postgresql
Summary:	PostgreSQL plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	postgresql-devel
%description postgresql
The PostgreSQL plugin connects to and executes SQL statements on a PostgreSQL
database.
%endif

%if %{with_procevent}
%package procevent
Summary:       Processes event plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: yajl-devel
%description procevent
Monitors process starts/stops via netlink library.
%endif

%if %{with_python}
%package python
Summary:	Python plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
    %if 0%{?rhel} && 0%{?rhel} < 6
BuildRequires: python26-devel
    %else
        %if 0%{?rhel} && 0%{?rhel} > 7
BuildRequires: python3-devel
        %else
BuildRequires: python-devel
    %endif
    %endif

%description python
The Python plugin embeds a Python interpreter into collectd and exposes the
application programming interface (API) to Python-scripts.
%endif

%if %{with_redis}
%package redis
Summary:	Redis plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	hiredis-devel
%description redis
The Redis plugin connects to one or more instances of Redis, a key-value store,
and collects usage information using the hiredis library.
%endif

%if %{with_redfish}
%package redfish
Summary:	Redfish plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libredfish
%description redfish
The Redfish plugin collects out-of-band sensor data from Redfish endpoints.
%endif

%if %{with_rrdcached}
%package rrdcached
Summary:        RRDCached plugin for collectd
Group:          System Environment/Daemons
Requires:       %{name}%{?_isa} = %{version}-%{release}, rrdtool >= 1.4
BuildRequires:  rrdtool-devel
%description rrdcached
The RRDCacheD plugin connects to the “RRD caching daemon”, rrdcached and
submits updates for RRD files to that daemon.
%endif

%if %{with_rrdtool}
%package rrdtool
Summary:	RRDtool plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	rrdtool-devel
%description rrdtool
The RRDtool plugin writes values to RRD-files using librrd.
%endif

%if %{with_sensors}
%package sensors
Summary:	Sensors plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	lm_sensors-devel
%description sensors
This plugin for collectd provides querying of sensors supported by lm_sensors.
%endif

%if %{with_sigrok}
%package sigrok
Summary:	sigrok plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%description sigrok
Uses libsigrok as a backend, allowing any sigrok-supported device to have its
measurements fed to collectd. This includes multimeters, sound level meters,
thermometers, and much more.
%endif

%if %{with_smart}
%package smart
Summary:       SMART plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: libatasmart-devel systemd-devel
%description smart
Collect SMART statistics, notably load cycle count, temperature and bad
sectors.
%endif

%if %{with_snmp}
%package snmp
Summary:	SNMP plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	net-snmp-devel
%description snmp
This plugin for collectd allows querying of network equipment using SNMP.
%endif

%if %{with_snmp_agent}
%package snmp_agent
Summary:	SNMP AgentX plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	net-snmp-devel
%description snmp_agent
This plugin for collectd to support AgentX integration.
%endif

%if %{with_sysevent}
%package sysevent
Summary:       Rsyslog event plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: yajl-devel
%description sysevent
Monitors rsyslog for system events.
%endif

%if %{with_varnish}
%package varnish
Summary:	Varnish plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	varnish-devel
%description varnish
The Varnish plugin collects information about Varnish, an HTTP accelerator.
%endif

%if %{with_virt}
%package virt
Summary:	Virt plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libvirt-devel
%description virt
This plugin collects information from virtualized guests.
%endif

%if %{with_write_http}
%package write_http
Summary:	Write-HTTP plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	curl-devel
%description write_http
The Write-HTTP plugin sends the values collected by collectd to a web-server
using HTTP POST requests.
%endif

%if %{with_write_log}
%package write_log
Summary:	Write-Log plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	yajl-devel
%description write_log
The Write-Log plugin writes metrics as INFO log messages.
%endif

%if %{with_write_kafka}
%package write_kafka
Summary:       Write-kafka plugin for collectd
Group:         System Environment/Daemons
Requires:      %{name}%{?_isa} = %{version}-%{release}
BuildRequires: librdkafka-devel
%description write_kafka
The write_kafka plugin sends values to kafka, a distributed messaging system.
%endif

%if %{with_write_mongodb}
%package write_mongodb
Summary:	Write-mongodb plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	mongo-c-driver-devel
%description write_mongodb
The write_mongodb plugin sends values to mongodb database.
%endif

%if %{with_write_prometheus}
%package write_prometheus
Summary:	Write-prometheus plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	libmicrohttpd-devel
%description write_prometheus
The Write Prometheus plugin exposes collected values using an embedded HTTP
server, turning the collectd daemon into a Prometheus exporter.
%endif

%if %{with_write_redis}
%package write_redis
Summary:	Write-Redis plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	hiredis-devel
%description write_redis
The Write Redis plugin stores values in Redis, a “data structures server”.
%endif

%if %{with_write_riemann}
%package write_riemann
Summary:	riemann plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	riemann-c-client-devel >= 1.6
%description write_riemann
The riemann plugin submits values to Riemann, an event stream processor.
%endif

%if %{with_write_stackdriver}
%package write_stackdriver
Summary:	stackdriver plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	curl-devel, yajl-devel, openssl-devel
%description write_stackdriver
The write_stackdriver collectd plugin writes metrics to the
Google Stackdriver Monitoring service.
%endif

%if %{with_write_syslog}
%package write_syslog
Summary:	write_syslog plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%description write_syslog
The write_syslog collectd plugin writes metrics to syslog
using JSON or RFC5424 formatting
%endif

%if %{with_gpu_nvidia}
%package gpu_nvidia
Summary:	stackdriver plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires: cuda-nvml-dev-10-1
%description gpu_nvidia
The gpu_nvidia collectd plugin collects NVidia GPU metrics.
%endif

%if %{with_xencpu}
%package xencpu
Summary:	xencpu plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	xen-devel
%description xencpu
The xencpu plugin collects CPU statistics from Xen.
%endif

%if %{with_xmms}
%package xmms
Summary:	XMMS plugin for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
BuildRequires:	xmms-devel
%description xmms
The xmms plugin collects information from the XMMS music player.
%endif

%package collection3
Summary:	Web-based viewer for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
Requires: httpd
%description collection3
collection3 is a graphing front-end for the RRD files created by and filled
with collectd. It is written in Perl and should be run as an CGI-script.
Graphs are generated on-the-fly, so no cron job or similar is necessary.

%package php-collection
Summary:	collect php webfrontent
Group:		System Environment/Daemons
Requires:	collectd = %{version}-%{release}
Requires:	httpd
Requires:	php
Requires:	php-rrdtool
%description php-collection
PHP graphing frontend for RRD files created by and filled with collectd.

%package contrib
Summary:	Contrib files for collectd
Group:		System Environment/Daemons
Requires:	%{name}%{?_isa} = %{version}-%{release}
%description contrib
All the files found under contrib/ in the source tree are bundled in this
package.

%package -n libcollectdclient
Summary:	Collectd client library
Group:		System Environment/Daemons
%description -n libcollectdclient
Collectd client library

%package -n libcollectdclient-devel
Summary:	Development files for libcollectdclient
Group:		System Environment/Daemons
Requires:	pkgconfig
Requires:	libcollectdclient%{?_isa} = %{version}-%{release}
%description -n libcollectdclient-devel
Development files for libcollectdclient

%package -n collectd-utils
Summary:	Collectd utilities
Group:		System Environment/Daemons
Requires:	libcollectdclient%{?_isa} = %{version}-%{release}
Requires:	collectd%{?_isa} = %{version}-%{release}
%description -n collectd-utils
Collectd utilities

%prep
%setup -q

%build
%if %{with_aggregation}
%define _with_aggregation --enable-aggregation
%else
%define _with_aggregation --disable-aggregation
%endif

%if %{with_amqp}
%define _with_amqp --enable-amqp
%else
%define _with_amqp --disable-amqp
%endif

%if %{with_amqp1}
%define _with_amqp1 --enable-amqp1
%else
%define _with_amqp1 --disable-amqp1
%endif

%if %{with_apache}
%define _with_apache --enable-apache
%else
%define _with_apache --disable-apache
%endif

%if %{with_apcups}
%define _with_apcups --enable-apcups
%else
%define _with_apcups --disable-apcups
%endif

%if %{with_apple_sensors}
%define _with_apple_sensors --enable-apple_sensors
%else
%define _with_apple_sensors --disable-apple_sensors
%endif

%if %{with_aquaero}
%define _with_aquaero --enable-aquaero
%else
%define _with_aquaero --disable-aquaero
%endif

%if %{with_ascent}
%define _with_ascent --enable-ascent
%else
%define _with_ascent --disable-ascent
%endif

%if %{with_barometer}
%define _with_barometer --enable-barometer
%else
%define _with_barometer --disable-barometer
%endif

%if %{with_battery}
%define _with_battery --enable-battery
%else
%define _with_battery --disable-battery
%endif

%if %{with_bind}
%define _with_bind --enable-bind
%else
%define _with_bind --disable-bind
%endif

%if %{with_buddyinfo}
%define _with_buddyinfo --enable-buddyinfo
%else
%define _with_buddyinfo --disable-buddyinfo
%endif

%if %{with_capabilities}
%define _with_capabilities --enable-capabilities
%else
%define _with_capabilities --disable-capabilities
%endif

%if %{with_cgroups}
%define _with_cgroups --enable-cgroups
%else
%define _with_cgroups --disable-cgroups
%endif

%if %{with_check_uptime}
%define _with_check_uptime --enable-check_uptime
%else
%define _with_check_uptime --disable-check_uptime
%endif

%if %{with_chrony}
%define _with_chrony --enable-chrony
%else
%define _with_chrony --disable-chrony
%endif

%if %{with_connectivity}
%define _with_connectivity --enable-connectivity
%else
%define _with_connectivity --disable-connectivity
%endif

%if %{with_conntrack}
%define _with_conntrack --enable-conntrack
%else
%define _with_conntrack --disable-conntrack
%endif

%if %{with_contextswitch}
%define _with_contextswitch --enable-contextswitch
%else
%define _with_contextswitch --disable-contextswitch
%endif

%if %{with_cpu}
%define _with_cpu --enable-cpu
%else
%define _with_cpu --disable-cpu
%endif

%if %{with_cpufreq}
%define _with_cpufreq --enable-cpufreq
%else
%define _with_cpufreq --disable-cpufreq
%endif

%if %{with_cpusleep}
%define _with_cpusleep --enable-cpusleep
%else
%define _with_cpusleep --disable-cpusleep
%endif

%if %{with_csv}
%define _with_csv --enable-csv
%else
%define _with_csv --disable-csv
%endif

%if %{with_ceph}
%define _with_ceph --enable-ceph
%else
%define _with_ceph --disable-ceph
%endif

%if %{with_curl}
%define _with_curl --enable-curl
%else
%define _with_curl --disable-curl
%endif

%if %{with_curl_json}
%define _with_curl_json --enable-curl_json
%else
%define _with_curl_json --disable-curl_json
%endif

%if %{with_curl_xml}
%define _with_curl_xml --enable-curl_xml
%else
%define _with_curl_xml --disable-curl_xml
%endif

%if %{with_dbi}
%define _with_dbi --enable-dbi
%else
%define _with_dbi --disable-dbi
%endif

%if %{with_dcpmm}
%define _with_dcpmm --enable-dcpmm
%else
%define _with_dcpmm --disable-dcpmm
%endif

%if %{with_df}
%define _with_df --enable-df
%else
%define _with_df --disable-df
%endif

%if %{with_disk}
%define _with_disk --enable-disk
%else
%define _with_disk --disable-disk
%endif

%if %{with_diskadapter}
%define _with_diskadapter --enable-diskadapter
%else
%define _with_diskadapter --disable-diskadapter
%endif

%if %{with_diskpath}
%define _with_diskpath --enable-diskpath
%else
%define _with_diskpath --disable-diskpath
%endif

%if %{with_dns}
%define _with_dns --enable-dns
%else
%define _with_dns --disable-dns
%endif

%if %{with_drbd}
%define _with_drbd --enable-drbd
%else
%define _with_drbd --disable-drbd
%endif

%if %{with_dpdkevents}
%define _with_dpdkevents --enable-dpdkevents
%else
%define _with_dpdkevents --disable-dpdkevents
%endif

%if %{with_dpdkstat}
%define _with_dpdkstat --enable-dpdkstat
%else
%define _with_dpdkstat --disable-dpdkstat
%endif

%if %{with_dpdk_telemetry}
%define _with_dpdk_telemetry --enable-dpdk_telemetry
%else
%define _with_dpdk_telemetry --disable-dpdk_telemetry
%endif

%if %{with_email}
%define _with_email --enable-email
%else
%define _with_email --disable-email
%endif

%if %{with_entropy}
%define _with_entropy --enable-entropy
%else
%define _with_entropy --disable-entropy
%endif

%if %{with_ethstat}
%define _with_ethstat --enable-ethstat
%else
%define _with_ethstat --disable-ethstat
%endif

%if %{with_exec}
%define _with_exec --enable-exec
%else
%define _with_exec --disable-exec
%endif

%if %{with_fhcount}
%define _with_fhcount --enable-fhcount
%else
%define _with_fhcount --disable-fhcount
%endif

%if %{with_filecount}
%define _with_filecount --enable-filecount
%else
%define _with_filecount --disable-filecount
%endif

%if %{with_fscache}
%define _with_fscache --enable-fscache
%else
%define _with_fscache --disable-fscache
%endif

%if %{with_gmond}
%define _with_gmond --enable-gmond
%else
%define _with_gmond --disable-gmond
%endif

%if %{with_gps}
%define _with_gps --enable-gps
%else
%define _with_gps --disable-gps
%endif

%if %{with_grpc}
%define _with_grpc --enable-grpc
%else
%define _with_grpc --disable-grpc
%endif

%if %{with_hba}
%define _with_hba --enable-hba
%else
%define _with_hba --disable-hba
%endif

%if %{with_hddtemp}
%define _with_hddtemp --enable-hddtemp
%else
%define _with_hddtemp --disable-hddtemp
%endif

%if %{with_hugepages}
%define _with_hugepages --enable-hugepages
%else
%define _with_hugepages --disable-hugepages
%endif

%if %{with_intel_pmu}
%define _with_intel_pmu --enable-intel_pmu
%else
%define _with_intel_pmu --disable-intel_pmu
%endif

%if %{with_intel_rdt}
%define _with_intel_rdt --enable-intel_rdt
%else
%define _with_intel_rdt --disable-intel_rdt
%endif

%if %{with_infiniband}
%define _with_infiniband --enable-infiniband
%else
%define _with_infiniband --disable-infiniband
%endif

%if %{with_interface}
%define _with_interface --enable-interface
%else
%define _with_interface --disable-interface
%endif

%if %{with_ipc}
%define _with_ipc --enable-ipc
%else
%define _with_ipc --disable-ipc
%endif

%if %{with_ipmi}
%define _with_ipmi --enable-ipmi
%else
%define _with_ipmi --disable-ipmi
%endif

%if %{with_ipstats}
%define _with_ipstats --enable-ipstats
%else
%define _with_ipstats --disable-ipstats
%endif

%if %{with_iptables}
%define _with_iptables --enable-iptables
%else
%define _with_iptables --disable-iptables
%endif

%if %{with_ipvs}
%define _with_ipvs --enable-ipvs
%else
%define _with_ipvs --disable-ipvs
%endif

%if %{with_irq}
%define _with_irq --enable-irq
%else
%define _with_irq --disable-irq
%endif

%if %{with_java}
%define _with_java --enable-java --with-java=%{java_home}/
%else
%define _with_java --disable-java
%endif

%if %{with_virt}
%define _with_virt --enable-virt
%else
%define _with_virt --disable-virt
%endif

%if %{with_load}
%define _with_load --enable-load
%else
%define _with_load --disable-load
%endif

%if %{with_logfile}
%define _with_logfile --enable-logfile
%else
%define _with_logfile --disable-logfile
%endif

%if %{with_logparser}
%define _with_logparser --enable-logparser
%else
%define _with_logparser --disable-logparser
%endif

%if %{with_log_logstash}
%define _with_log_logstash --enable-log_logstash
%else
%define _with_log_logstash --disable-log_logstash
%endif

%if %{with_lpar}
%define _with_lpar --enable-lpar
%else
%define _with_lpar --disable-lpar
%endif

%if %{with_lua}
%define _with_lua --enable-lua
%else
%define _with_lua --disable-lua
%endif

%if %{with_lvm}
%define _with_lvm --enable-lvm
%else
%define _with_lvm --disable-lvm
%endif

%if %{with_madwifi}
%define _with_madwifi --enable-madwifi
%else
%define _with_madwifi --disable-madwifi
%endif

%if %{with_mbmon}
%define _with_mbmon --enable-mbmon
%else
%define _with_mbmon --disable-mbmon
%endif

%if %{with_mcelog}
%define _with_mcelog --enable-mcelog
%else
%define _with_mcelog --disable-mcelog
%endif

%if %{with_md}
%define _with_md --enable-md
%else
%define _with_md --disable-md
%endif

%if %{with_mdevents}
%define _with_mdevents --enable-mdevents
%else
%define _with_mdevents --disable-mdevents
%endif

%if %{with_memcachec}
%define _with_memcachec --enable-memcachec
%else
%define _with_memcachec --disable-memcachec
%endif

%if %{with_memcached}
%define _with_memcached --enable-memcached
%else
%define _with_memcached --disable-memcached
%endif

%if %{with_memory}
%define _with_memory --enable-memory
%else
%define _with_memory --disable-memory
%endif

%if %{with_mic}
%define _with_mic --enable-mic
%else
%define _with_mic --disable-mic
%endif

%if %{with_mmc}
%define _with_mmc --enable-mmc
%else
%define _with_mmc --disable-mmc
%endif

%if %{with_modbus}
%define _with_modbus --enable-modbus
%else
%define _with_modbus --disable-modbus
%endif

%if %{with_multimeter}
%define _with_multimeter --enable-multimeter
%else
%define _with_multimeter --disable-multimeter
%endif

%if %{with_mqtt}
%define _with_mqtt --enable-mqtt
%else
%define _with_mqtt --disable-mqtt
%endif

%if %{with_mongodb}
%define _with_mongodb --enable-mongodb
%else
%define _with_mongodb --disable-mongodb
%endif

%if %{with_mysql}
%define _with_mysql --enable-mysql
%else
%define _with_mysql --disable-mysql
%endif

%if %{with_netapp}
%define _with_netapp --enable-netapp
%else
%define _with_netapp --disable-netapp
%endif

%if %{with_netlink}
%define _with_netlink --enable-netlink
%else
%define _with_netlink --disable-netlink
%endif

%if %{with_netstat_udp}
%define _with_netstat_udp --enable-netstat_udp
%else
%define _with_netstat_udp --disable-netstat_udp
%endif

%if %{with_network}
%define _with_network --enable-network
%else
%define _with_network --disable-network
%endif

%if %{with_nfs}
%define _with_nfs --enable-nfs
%else
%define _with_nfs --disable-nfs
%endif

%if %{with_nginx}
%define _with_nginx --enable-nginx
%else
%define _with_nginx --disable-nginx
%endif

%if %{with_notify_dbi}
%define _with_notify_dbi --enable-notify_dbi
%else
%define _with_notify_dbi --disable-notify_dbi
%endif

%if %{with_notify_desktop}
%define _with_notify_desktop --enable-notify_desktop
%else
%define _with_notify_desktop --disable-notify_desktop
%endif

%if %{with_notify_email}
%define _with_notify_email --enable-notify_email
%else
%define _with_notify_email --disable-notify_email
%endif

%if %{with_notify_nagios}
%define _with_notify_nagios --enable-notify_nagios
%else
%define _with_notify_nagios --disable-notify_nagios
%endif

%if %{with_notify_snmp}
%define _with_notify_snmp --enable-notify_snmp
%else
%define _with_notify_snmp --disable-notify_snmp
%endif

%if %{with_ntpd}
%define _with_ntpd --enable-ntpd
%else
%define _with_ntpd --disable-ntpd
%endif

%if %{with_numa}
%define _with_numa --enable-numa
%else
%define _with_numa --disable-numa
%endif

%if %{with_nut}
%define _with_nut --enable-nut
%else
%define _with_nut --disable-nut
%endif

%if %{with_odbc}
%define _with_odbc --enable-odbc
%else
%define _with_odbc --disable-odbc
%endif

%if %{with_olsrd}
%define _with_olsrd --enable-olsrd
%else
%define _with_olsrd --disable-olsrd
%endif

%if %{with_onewire}
%define _with_onewire --enable-onewire
%else
%define _with_onewire --disable-onewire
%endif

%if %{with_openldap}
%define _with_openldap --enable-openldap
%else
%define _with_openldap --disable-openldap
%endif

%if %{with_openvpn}
%define _with_openvpn --enable-openvpn
%else
%define _with_openvpn --disable-openvpn
%endif

%if %{with_oracle}
%define _with_oracle --enable-oracle
%else
%define _with_oracle --disable-oracle
%endif

%if %{with_ovs_events}
%define _with_ovs_events --enable-ovs_events
%else
%define _with_ovs_events --disable-ovs_events
%endif

%if %{with_ovs_stats}
%define _with_ovs_stats --enable-ovs_stats
%else
%define _with_ovs_stats --disable-ovs_stats
%endif

%if %{with_perl}
%define _with_perl --enable-perl --with-perl-bindings="INSTALLDIRS=vendor"
%else
%define _with_perl --disable-perl
%endif

%if %{with_pcie_errors}
%define _with_pcie_errors --enable-pcie_errors
%else
%define _with_pcie_errors --disable-pcie_errors
%endif

%if %{with_pf}
%define _with_pf --enable-pf
%else
%define _with_pf --disable-pf
%endif

%if %{with_pinba}
%define _with_pinba --enable-pinba
%else
%define _with_pinba --disable-pinba
%endif

%if %{with_ping}
%define _with_ping --enable-ping
%else
%define _with_ping --disable-ping
%endif

%if %{with_postgresql}
%define _with_postgresql --enable-postgresql
%else
%define _with_postgresql --disable-postgresql
%endif

%if %{with_powerdns}
%define _with_powerdns --enable-powerdns
%else
%define _with_powerdns --disable-powerdns
%endif

%if %{with_processes}
%define _with_processes --enable-processes
%else
%define _with_processes --disable-processes
%endif

%if %{with_procevent}
%define _with_procevent --enable-procevent
%else
%define _with_procevent --disable-procevent
%endif

%if %{with_protocols}
%define _with_protocols --enable-protocols
%else
%define _with_protocols --disable-protocols
%endif

%if %{with_python}
    %if 0%{?rhel} && 0%{?rhel} < 6
%define _with_python --enable-python --with-python=%{_bindir}/python2.6
%define _python_config PYTHON_CONFIG="%{_bindir}/python2.6-config"
    %else
%define _with_python --enable-python
    %endif
%else
%define _with_python --disable-python
%endif

%if %{with_ras}
%define _with_ras --enable-ras
%else
%define _with_ras --disable-ras
%endif

%if %{with_redis}
%define _with_redis --enable-redis
%else
%define _with_redis --disable-redis
%endif

%if %{with_redfish}
%define _with_redfish --enable-redfish
%else
%define _with_redfish --disable-redfish
%endif

%if %{with_routeros}
%define _with_routeros --enable-routeros
%else
%define _with_routeros --disable-routeros
%endif

%if %{with_rrdcached}
%define _with_rrdcached --enable-rrdcached
%else
%define _with_rrdcached --disable-rrdcached
%endif

%if %{with_rrdtool}
%define _with_rrdtool --enable-rrdtool
%else
%define _with_rrdtool --disable-rrdtool
%endif

%if %{with_sensors}
%define _with_sensors --enable-sensors
%else
%define _with_sensors --disable-sensors
%endif

%if %{with_serial}
%define _with_serial --enable-serial
%else
%define _with_serial --disable-serial
%endif

%if %{with_sigrok}
%define _with_sigrok --enable-sigrok
%else
%define _with_sigrok --disable-sigrok
%endif

%if %{with_slurm}
%define _with_slurm --enable-slurm
%else
%define _with_slurm --disable-slurm
%endif

%if %{with_smart}
%define _with_smart --enable-smart
%else
%define _with_smart --disable-smart
%endif

%if %{with_snmp}
%define _with_snmp --enable-snmp
%else
%define _with_snmp --disable-snmp
%endif

%if %{with_snmp_agent}
%define _with_snmp_agent --enable-snmp_agent
%else
%define _with_snmp_agent --disable-snmp_agent
%endif

%if %{with_statsd}
%define _with_statsd --enable-statsd
%else
%define _with_statsd --disable-statsd
%endif

%if %{with_swap}
%define _with_swap --enable-swap
%else
%define _with_swap --disable-swap
%endif

%if %{with_synproxy}
%define _with_synproxy --enable-synproxy
%else
%define _with_synproxy --disable-synproxy
%endif

%if %{with_sysevent}
%define _with_sysevent --enable-sysevent
%else
%define _with_sysevent --disable-sysevent
%endif

%if %{with_syslog}
%define _with_syslog --enable-syslog
%else
%define _with_syslog --disable-syslog
%endif

%if %{with_table}
%define _with_table --enable-table
%else
%define _with_table --disable-table
%endif

%if %{with_tail}
%define _with_tail --enable-tail
%else
%define _with_tail --disable-tail
%endif

%if %{with_tail_csv}
%define _with_tail_csv --enable-tail_csv
%else
%define _with_tail_csv --disable-tail_csv
%endif

%if %{with_tape}
%define _with_tape --enable-tape
%else
%define _with_tape --disable-tape
%endif

%if %{with_tcpconns}
%define _with_tcpconns --enable-tcpconns
%else
%define _with_tcpconns --disable-tcpconns
%endif

%if %{with_teamspeak2}
%define _with_teamspeak2 --enable-teamspeak2
%else
%define _with_teamspeak2 --disable-teamspeak2
%endif

%if %{with_ted}
%define _with_ted --enable-ted
%else
%define _with_ted --disable-ted
%endif

%if %{with_thermal}
%define _with_thermal --enable-thermal
%else
%define _with_thermal --disable-thermal
%endif

%if %{with_threshold}
%define _with_threshold --enable-threshold
%else
%define _with_threshold --disable-threshold
%endif

%if %{with_tokyotyrant}
%define _with_tokyotyrant --enable-tokyotyrant
%else
%define _with_tokyotyrant --disable-tokyotyrant
%endif

%if %{with_turbostat}
%define _with_turbostat --enable-turbostat
%else
%define _with_turbostat --disable-turbostat
%endif

%if %{with_unixsock}
%define _with_unixsock --enable-unixsock
%else
%define _with_unixsock --disable-unixsock
%endif

%if %{with_uptime}
%define _with_uptime --enable-uptime
%else
%define _with_uptime --disable-uptime
%endif

%if %{with_ubi}
%define _with_ubi --enable-ubi
%else
%define _with_ubi --disable-ubi
%endif

%if %{with_users}
%define _with_users --enable-users
%else
%define _with_users --disable-users
%endif

%if %{with_uuid}
%define _with_uuid --enable-uuid
%else
%define _with_uuid --disable-uuid
%endif

%if %{with_varnish}
%define _with_varnish --enable-varnish
%else
%define _with_varnish --disable-varnish
%endif

%if %{with_vmem}
%define _with_vmem --enable-vmem
%else
%define _with_vmem --disable-vmem
%endif

%if %{with_vserver}
%define _with_vserver --enable-vserver
%else
%define _with_vserver --disable-vserver
%endif

%if %{with_wlm}
%define _with_wlm --enable-wlm
%else
%define _with_wlm --disable-wlm
%endif

%if %{with_wpar}
%define _with_wpar --enable-wpar
%else
%define _with_wpar --disable-wpar
%endif

%if %{with_wireless}
%define _with_wireless --enable-wireless
%else
%define _with_wireless --disable-wireless
%endif

%if %{with_write_graphite}
%define _with_write_graphite --enable-write_graphite
%else
%define _with_write_graphite --disable-write_graphite
%endif

%if %{with_write_http}
%define _with_write_http --enable-write_http
%else
%define _with_write_http --disable-write_http
%endif

%if %{with_write_kafka}
%define _with_write_kafka --enable-write_kafka
%else
%define _with_write_kafka --disable-write_kafka
%endif

%if %{with_write_log}
%define _with_write_log --enable-write_log
%else
%define _with_write_log --disable-write_log
%endif

%if %{with_write_mongodb}
%define _with_write_mongodb --enable-write_mongodb
%else
%define _with_write_mongodb --disable-write_mongodb
%endif

%if %{with_write_prometheus}
%define _with_write_prometheus --enable-write_prometheus
%else
%define _with_write_prometheus --disable-write_prometheus
%endif

%if %{with_write_redis}
%define _with_write_redis --enable-write_redis
%else
%define _with_write_redis --disable-write_redis
%endif

%if %{with_write_riemann}
%define _with_write_riemann --enable-write_riemann
%else
%define _with_write_riemann --disable-write_riemann
%endif

%if %{with_write_stackdriver}
%define _with_write_stackdriver --enable-write_stackdriver
%else
%define _with_write_stackdriver --disable-write_stackdriver
%endif

%if %{with_gpu_nvidia}
%define _with_gpu_nvidia --enable-gpu_nvidia
%else
%define _with_gpu_nvidia --disable-gpu_nvidia
%endif

%if %{with_write_sensu}
%define _with_write_sensu --enable-write_sensu
%else
%define _with_write_sensu --disable-write_sensu
%endif

%if %{with_write_syslog}
%define _with_write_syslog --enable-write_syslog
%else
%define _with_write_syslog --disable-write_syslog
%endif

%if %{with_write_tsdb}
%define _with_write_tsdb --enable-write_tsdb
%else
%define _with_write_tsdb --disable-write_tsdb
%endif

%if %{with_xencpu}
%define _with_xencpu --enable-xencpu
%else
%define _with_xencpu --disable-xencpu
%endif

%if %{with_xmms}
%define _with_xmms --enable-xmms
%else
%define _with_xmms --disable-xmms
%endif

%if %{with_zfs_arc}
%define _with_zfs_arc --enable-zfs_arc
%else
%define _with_zfs_arc --disable-zfs_arc
%endif

%if %{with_zone}
%define _with_zone --enable-zone
%else
%define _with_zone --disable-zone
%endif

%if %{with_zookeeper}
%define _with_zookeeper --enable-zookeeper
%else
%define _with_zookeeper --disable-zookeeper
%endif

%if %{with debug}
%define _feature_debug --enable-debug
%else
%define _feature_debug --disable-debug
%endif

#%{SOURCE1}
#%{SOURCE2}

./build.sh
%configure CFLAGS="%{optflags} -I/usr/include/libredfish -DLT_LAZY_OR_NOW=\"RTLD_LAZY|RTLD_GLOBAL\"" \
    %{?_python_config} \
    %{?_feature_debug} \
    --disable-static \
    --enable-all-plugins=yes \
    --enable-match_empty_counter \
    --enable-match_hashed \
    --enable-match_regex \
    --enable-match_timediff \
    --enable-match_value \
    --enable-target_notification \
    --enable-target_replace \
    --enable-target_scale \
    --enable-target_set \
    --enable-target_v5upgrade \
    %{?_with_aggregation} \
    %{?_with_amqp} \
    %{?_with_amqp1} \
    %{?_with_apache} \
    %{?_with_apcups} \
    %{?_with_apple_sensors} \
    %{?_with_aquaero} \
    %{?_with_ascent} \
    %{?_with_barometer} \
    %{?_with_battery} \
    %{?_with_bind} \
    %{?_with_buddyinfo} \
    %{?_with_capabilities} \
    %{?_with_ceph} \
    %{?_with_cgroups} \
    %{?_with_check_uptime} \
    %{?_with_chrony} \
    %{?_with_connectivity} \
    %{?_with_conntrack} \
    %{?_with_contextswitch} \
    %{?_with_cpufreq} \
    %{?_with_cpusleep} \
    %{?_with_cpu} \
    %{?_with_csv} \
    %{?_with_curl_json} \
    %{?_with_curl_xml} \
    %{?_with_curl} \
    %{?_with_dbi} \
    %{?_with_dcpmm} \
    %{?_with_df} \
    %{?_with_disk} \
    %{?_with_diskadapter} \
    %{?_with_diskpath} \
    %{?_with_dns} \
    %{?_with_drbd} \
    %{?_with_dpdkevents} \
    %{?_with_dpdkstat} \
    %{?_with_dpdk_telemetry} \
    %{?_with_email} \
    %{?_with_entropy} \
    %{?_with_ethstat} \
    %{?_with_exec} \
    %{?_with_fhcount} \
    %{?_with_filecount} \
    %{?_with_fscache} \
    %{?_with_gmond} \
    %{?_with_gps} \
    %{?_with_grpc} \
    %{?_with_hba} \
    %{?_with_hddtemp} \
    %{?_with_hugepages} \
    %{?_with_intel_pmu} \
    %{?_with_intel_rdt} \
    %{?_with_interface} \
    %{?_with_infiniband} \
    %{?_with_ipc} \
    %{?_with_ipmi} \
    %{?_with_iptables} \
    %{?_with_ipstats} \
    %{?_with_ipvs} \
    %{?_with_irq} \
    %{?_with_java} \
    %{?_with_load} \
    %{?_with_log_logstash} \
    %{?_with_logfile} \
    %{?_with_logparser} \
    %{?_with_lpar} \
    %{?_with_lua} \
    %{?_with_lvm} \
    %{?_with_madwifi} \
    %{?_with_mbmon} \
    %{?_with_mcelog} \
    %{?_with_md} \
    %{?_with_mdevents} \
    %{?_with_memcachec} \
    %{?_with_memcached} \
    %{?_with_memory} \
    %{?_with_mic} \
    %{?_with_mmc} \
    %{?_with_modbus} \
    %{?_with_mqtt} \
    %{?_with_mongodb} \
    %{?_with_multimeter} \
    %{?_with_mysql} \
    %{?_with_netapp} \
    %{?_with_netlink} \
    %{?_with_netstat_udp} \
    %{?_with_network} \
    %{?_with_nfs} \
    %{?_with_nginx} \
    %{?_with_notify_dbi} \
    %{?_with_notify_desktop} \
    %{?_with_notify_email} \
    %{?_with_notify_nagios} \
    %{?_with_notify_snmp} \
    %{?_with_ntpd} \
    %{?_with_numa} \
    %{?_with_nut} \
    %{?_with_odbc} \
    %{?_with_olsrd} \
    %{?_with_onewire} \
    %{?_with_openldap} \
    %{?_with_openvpn} \
    %{?_with_oracle} \
    %{?_with_ovs_events} \
    %{?_with_ovs_stats} \
    %{?_with_perl} \
    %{?_with_pcie_errors} \
    %{?_with_pf} \
    %{?_with_pinba} \
    %{?_with_ping} \
    %{?_with_postgresql} \
    %{?_with_powerdns} \
    %{?_with_processes} \
    %{?_with_procevent} \
    %{?_with_protocols} \
    %{?_with_python} \
    %{?_with_ras} \
    %{?_with_redis} \
    %{?_with_redfish} \
    %{?_with_routeros} \
    %{?_with_rrdcached} \
    %{?_with_rrdtool} \
    %{?_with_sensors} \
    %{?_with_serial} \
    %{?_with_sigrok} \
    %{?_with_smart} \
    %{?_with_snmp} \
    %{?_with_snmp_agent} \
    %{?_with_slurm} \
    %{?_with_statsd} \
    %{?_with_swap} \
    %{?_with_synproxy} \
    %{?_with_sysevent} \
    %{?_with_syslog} \
    %{?_with_table} \
    %{?_with_tail_csv} \
    %{?_with_tail} \
    %{?_with_tape} \
    %{?_with_tcpconns} \
    %{?_with_teamspeak2} \
    %{?_with_ted} \
    %{?_with_thermal} \
    %{?_with_threshold} \
    %{?_with_tokyotyrant} \
    %{?_with_turbostat} \
    %{?_with_unixsock} \
    %{?_with_uptime} \
    %{?_with_ubi} \
    %{?_with_users} \
    %{?_with_uuid} \
    %{?_with_varnish} \
    %{?_with_virt} \
    %{?_with_vmem} \
    %{?_with_vserver} \
    %{?_with_wireless}\
    %{?_with_wlm}\
    %{?_with_wpar}\
    %{?_with_write_graphite} \
    %{?_with_write_http} \
    %{?_with_write_influxdb_udp} \
    %{?_with_write_kafka} \
    %{?_with_write_log} \
    %{?_with_write_mongodb} \
    %{?_with_write_prometheus} \
    %{?_with_write_redis} \
    %{?_with_write_riemann} \
    %{?_with_write_stackdriver} \
    %{?_with_gpu_nvidia} \
    %{?_with_write_sensu} \
    %{?_with_write_syslog} \
    %{?_with_write_tsdb} \
    %{?_with_xencpu} \
    %{?_with_xmms} \
    %{?_with_zfs_arc} \
    %{?_with_zone} \
    %{?_with_zookeeper}


%{__make} %{?_smp_mflags}


%install
rm -rf %{buildroot}
%{__make} install DESTDIR=%{buildroot}
%if 0%{?fedora} || 0%{?rhel} >= 7
%{__install} -Dp -m0644 contrib/systemd.collectd.service %{buildroot}%{_unitdir}/collectd.service
%else
%{__install} -Dp -m0755 contrib/redhat/init.d-collectd %{buildroot}%{_initrddir}/collectd
%endif
%{__install} -Dp -m0644 src/collectd.conf %{buildroot}%{_sysconfdir}/collectd.conf
%{__install} -d %{buildroot}%{_sharedstatedir}/collectd/
%{__install} -d %{buildroot}%{_sysconfdir}/collectd.d/

%{__mkdir} -p %{buildroot}%{_localstatedir}/www
%{__mkdir} -p %{buildroot}/%{_sysconfdir}/httpd/conf.d

%{__mv} contrib/collection3 %{buildroot}%{_localstatedir}/www
%{__mv} contrib/redhat/collection3.conf %{buildroot}/%{_sysconfdir}/httpd/conf.d/

%{__mv} contrib/php-collection %{buildroot}%{_localstatedir}/www
%{__mv} contrib/redhat/php-collection.conf %{buildroot}/%{_sysconfdir}/httpd/conf.d/

### Clean up docs
find contrib/ -type f -exec %{__chmod} a-x {} \;
# *.la files shouldn't be distributed.
rm -f %{buildroot}/%{_libdir}/{collectd/,}*.la

# Remove Perl hidden .packlist files.
find %{buildroot} -type f -name .packlist -delete
# Remove Perl temporary file perllocal.pod
find %{buildroot} -type f -name perllocal.pod -delete

%if ! %{with_java}
rm -f %{buildroot}%{_datadir}/collectd/java/collectd-api.jar
rm -f %{buildroot}%{_datadir}/collectd/java/generic-jmx.jar
rm -f %{buildroot}%{_mandir}/man5/collectd-java.5*
%endif

%if ! %{with_lua}
rm -f %{buildroot}%{_mandir}/man5/collectd-lua.5*
%endif

%if ! %{with_perl}
rm -f %{buildroot}%{_mandir}/man5/collectd-perl.5*
rm -f %{buildroot}%{_mandir}/man3/Collectd::Unixsock.3pm*
rm -fr %{buildroot}/usr/lib/perl5/
%endif

%if ! %{with_postgresql}
rm -f %{buildroot}%{_datadir}/collectd/postgresql_default.conf
%endif

%if ! %{with_python}
rm -f %{buildroot}%{_mandir}/man5/collectd-python.5*
%endif

%if ! %{with_snmp}
rm -f %{buildroot}%{_mandir}/man5/collectd-snmp.5*
%endif


%clean
rm -rf %{buildroot}

%post
%if 0%{?fedora} || 0%{?rhel} >= 7
%systemd_post collectd.service
%else
/sbin/chkconfig --add collectd || :
%endif

%preun
%if 0%{?fedora} || 0%{?rhel} >= 7
%systemd_preun collectd.service
%else
# stop collectd only when uninstalling
if [ $1 -eq 0 ]; then
    /sbin/service collectd stop >/dev/null 2>&1 || :
    /sbin/chkconfig --del collectd || :
fi
%endif

%postun
%if 0%{?fedora} || 0%{?rhel} >= 7
%systemd_postun_with_restart collectd.service
%else
# restart collectd only when upgrading
if [ $1 -eq 1 ]; then
    /sbin/service collectd condrestart >/dev/null 2>&1 || :
fi
%endif

%post -n libcollectdclient -p /sbin/ldconfig
%postun -n libcollectdclient -p /sbin/ldconfig


%files
%doc AUTHORS COPYING ChangeLog README
%config(noreplace) %{_sysconfdir}/collectd.conf
%if 0%{?fedora} || 0%{?rhel} >= 7
%{_unitdir}/collectd.service
%else
%{_initrddir}/collectd
%endif
%{_sbindir}/collectd
%{_sbindir}/collectdmon
%{_datadir}/collectd/types.db
%{_sharedstatedir}/collectd
%{_mandir}/man1/collectd.1*
%{_mandir}/man1/collectdmon.1*
%{_mandir}/man5/collectd-email.5*
%{_mandir}/man5/collectd-exec.5*
%{_mandir}/man5/collectd-threshold.5*
%{_mandir}/man5/collectd-unixsock.5*
%{_mandir}/man5/collectd.conf.5*
%{_mandir}/man5/types.db.5*

# all plugins bundled with the main collectd package
%{_libdir}/%{name}/match_empty_counter.so
%{_libdir}/%{name}/match_hashed.so
%{_libdir}/%{name}/match_regex.so
%{_libdir}/%{name}/match_timediff.so
%{_libdir}/%{name}/match_value.so
%{_libdir}/%{name}/target_notification.so
%{_libdir}/%{name}/target_replace.so
%{_libdir}/%{name}/target_scale.so
%{_libdir}/%{name}/target_set.so
%{_libdir}/%{name}/target_v5upgrade.so

%if %{with_aggregation}
%{_libdir}/%{name}/aggregation.so
%endif
%if %{with_apcups}
%{_libdir}/%{name}/apcups.so
%endif
%if %{with_battery}
%{_libdir}/%{name}/battery.so
%endif
%if %{with_buddyinfo}
%{_libdir}/%{name}/buddyinfo.so
%endif
%if %{with_cgroups}
%{_libdir}/%{name}/cgroups.so
%endif
%if %{with_check_uptime}
%{_libdir}/%{name}/check_uptime.so
%endif
%if %{with_conntrack}
%{_libdir}/%{name}/conntrack.so
%endif
%if %{with_contextswitch}
%{_libdir}/%{name}/contextswitch.so
%endif
%if %{with_cpu}
%{_libdir}/%{name}/cpu.so
%endif
%if %{with_cpufreq}
%{_libdir}/%{name}/cpufreq.so
%endif
%if %{with_cpusleep}
%{_libdir}/%{name}/cpusleep.so
%endif
%if %{with_csv}
%{_libdir}/%{name}/csv.so
%endif
%if %{with_df}
%{_libdir}/%{name}/df.so
%endif
%if %{with_disk}
%{_libdir}/%{name}/disk.so
%endif
%if %{with_dpdk_telemetry}
%{_libdir}/%{name}/dpdk_telemetry.so
%endif
%if %{with_drbd}
%{_libdir}/%{name}/drbd.so
%endif
%if %{with_ethstat}
%{_libdir}/%{name}/ethstat.so
%endif
%if %{with_entropy}
%{_libdir}/%{name}/entropy.so
%endif
%if %{with_exec}
%{_libdir}/%{name}/exec.so
%endif
%if %{with_fhcount}
%{_libdir}/%{name}/fhcount.so
%endif
%if %{with_filecount}
%{_libdir}/%{name}/filecount.so
%endif
%if %{with_fscache}
%{_libdir}/%{name}/fscache.so
%endif
%if %{with_hugepages}
%{_libdir}/%{name}/hugepages.so
%endif
%if %{with_infiniband}
%{_libdir}/%{name}/infiniband.so
%endif
%if %{with_interface}
%{_libdir}/%{name}/interface.so
%endif
%if %{with_ipc}
%{_libdir}/%{name}/ipc.so
%endif
%if %{with_ipvs}
%{_libdir}/%{name}/ipvs.so
%endif
%if %{with_irq}
%{_libdir}/%{name}/irq.so
%endif
%if %{with_load}
%{_libdir}/%{name}/load.so
%endif
%if %{with_logfile}
%{_libdir}/%{name}/logfile.so
%endif
%if %{with_logparser}
%{_libdir}/%{name}/logparser.so
%endif
%if %{with_madwifi}
%{_libdir}/%{name}/madwifi.so
%endif
%if %{with_mbmon}
%{_libdir}/%{name}/mbmon.so
%endif
%if %{with_mcelog}
%{_libdir}/%{name}/mcelog.so
%endif
%if %{with_md}
%{_libdir}/%{name}/md.so
%endif
%if %{with_mdevents}
%{_libdir}/%{name}/mdevents.so
%endif
%if %{with_memcached}
%{_libdir}/%{name}/memcached.so
%endif
%if %{with_memory}
%{_libdir}/%{name}/memory.so
%endif
%if %{with_mmc}
%{_libdir}/%{name}/mmc.so
%endif
%if %{with_multimeter}
%{_libdir}/%{name}/multimeter.so
%endif
%if %{with_network}
%{_libdir}/%{name}/network.so
%endif
%if %{with_nfs}
%{_libdir}/%{name}/nfs.so
%endif
%if %{with_notify_nagios}
%{_libdir}/%{name}/notify_nagios.so
%endif
%if %{with_ntpd}
%{_libdir}/%{name}/ntpd.so
%endif
%if %{with_numa}
%{_libdir}/%{name}/numa.so
%endif
%if %{with_openvpn}
%{_libdir}/%{name}/openvpn.so
%endif
%if %{with_olsrd}
%{_libdir}/%{name}/olsrd.so
%endif
%if %{with_powerdns}
%{_libdir}/%{name}/powerdns.so
%endif
%if %{with_processes}
%{_libdir}/%{name}/processes.so
%endif
%if %{with_protocols}
%{_libdir}/%{name}/protocols.so
%endif
%if %{with_serial}
%{_libdir}/%{name}/serial.so
%endif
%if %{with_statsd}
%{_libdir}/%{name}/statsd.so
%endif
%if %{with_swap}
%{_libdir}/%{name}/swap.so
%endif
%if %{with_synproxy}
%{_libdir}/%{name}/synproxy.so
%endif
%if %{with_syslog}
%{_libdir}/%{name}/syslog.so
%endif
%if %{with_table}
%{_libdir}/%{name}/table.so
%endif
%if %{with_tail}
%{_libdir}/%{name}/tail.so
%endif
%if %{with_tail_csv}
%{_libdir}/%{name}/tail_csv.so
%endif
%if %{with_tcpconns}
%{_libdir}/%{name}/tcpconns.so
%endif
%if %{with_teamspeak2}
%{_libdir}/%{name}/teamspeak2.so
%endif
%if %{with_ted}
%{_libdir}/%{name}/ted.so
%endif
%if %{with_thermal}
%{_libdir}/%{name}/thermal.so
%endif
%if %{with_threshold}
%{_libdir}/%{name}/threshold.so
%endif
%if %{with_turbostat}
%{_libdir}/%{name}/turbostat.so
%endif
%if %{with_unixsock}
%{_libdir}/%{name}/unixsock.so
%endif
%if %{with_uptime}
%{_libdir}/%{name}/uptime.so
%endif
%if %{with_ubi}
%{_libdir}/%{name}/ubi.so
%endif
%if %{with_users}
%{_libdir}/%{name}/users.so
%endif
%if %{with_uuid}
%{_libdir}/%{name}/uuid.so
%endif
%if %{with_vmem}
%{_libdir}/%{name}/vmem.so
%endif
%if %{with_vserver}
%{_libdir}/%{name}/vserver.so
%endif
%if %{with_wireless}
%{_libdir}/%{name}/wireless.so
%endif
%if %{with_write_graphite}
%{_libdir}/%{name}/write_graphite.so
%endif
%if %{with_write_influxdb_udp}
%{_libdir}/%{name}/write_influxdb_udp.so
%endif
%if %{with_write_syslog}
%{_libdir}/%{name}/write_syslog.so
%endif
%if %{with_write_sensu}
%{_libdir}/%{name}/write_sensu.so
%endif
%if %{with_write_tsdb}
%{_libdir}/%{name}/write_tsdb.so
%endif
%if %{with_zfs_arc}
%{_libdir}/%{name}/zfs_arc.so
%endif
%if %{with_zookeeper}
%{_libdir}/%{name}/zookeeper.so
%endif

%files -n libcollectdclient-devel
%{_includedir}/collectd/client.h
%{_includedir}/collectd/network.h
%{_includedir}/collectd/network_buffer.h
%{_includedir}/collectd/lcc_features.h
%{_libdir}/pkgconfig/libcollectdclient.pc
%{_includedir}/collectd/network_parse.h
%{_includedir}/collectd/server.h
%{_includedir}/collectd/types.h
%{_libdir}/libcollectdclient.so

%files -n libcollectdclient
%{_libdir}/libcollectdclient.so.*

%files -n collectd-utils
%{_bindir}/collectd-nagios
%{_bindir}/collectd-tg
%{_bindir}/collectdctl
%{_mandir}/man1/collectdctl.1*
%{_mandir}/man1/collectd-nagios.1*
%{_mandir}/man1/collectd-tg.1*

%if %{with_amqp}
%files amqp
%{_libdir}/%{name}/amqp.so
%endif

%if %{with_amqp1}
%files amqp1
%{_libdir}/%{name}/amqp1.so
%endif

%if %{with_apache}
%files apache
%{_libdir}/%{name}/apache.so
%endif

%if %{with_aquaero}
%files aquaero
%{_libdir}/%{name}/aquaero.so
%endif

%if %{with_ascent}
%files ascent
%{_libdir}/%{name}/ascent.so
%endif

%if %{with_barometer}
%files barometer
%{_libdir}/%{name}/barometer.so
%endif

%if %{with_bind}
%files bind
%{_libdir}/%{name}/bind.so
%endif

%if %{with_capabilities}
%files capabilities
%{_libdir}/%{name}/capabilities.so
%endif

%if %{with_ceph}
%files ceph
%{_libdir}/%{name}/ceph.so
%endif

%if %{with_chrony}
%files chrony
%{_libdir}/%{name}/chrony.so
%endif

%if %{with_connectivity}
%files connectivity
%{_libdir}/%{name}/connectivity.so
%endif

%if %{with_curl}
%files curl
%{_libdir}/%{name}/curl.so
%endif

%if %{with_curl_json}
%files curl_json
%{_libdir}/%{name}/curl_json.so
%endif

%if %{with_curl_xml}
%files curl_xml
%{_libdir}/%{name}/curl_xml.so
%endif

%if %{with_dns}
%files dns
%{_libdir}/%{name}/dns.so
%endif

%if %{with_dbi}
%files dbi
%{_libdir}/%{name}/dbi.so
%endif

%if %{with_dpdkevents}
%files dpdkevents
%{_libdir}/%{name}/dpdkevents.so
%endif

%if %{with_dpdkstat}
%files dpdkstat
%{_libdir}/%{name}/dpdkstat.so
%endif

%if %{with_email}
%files email
%{_libdir}/%{name}/email.so
%endif

%if %{with_gmond}
%files gmond
%{_libdir}/%{name}/gmond.so
%endif

%if %{with_gps}
%files gps
%{_libdir}/%{name}/gps.so
%endif

%if %{with_grpc}
%files grpc
%{_libdir}/%{name}/grpc.so
%endif

%if %{with_hddtemp}
%files hddtemp
%{_libdir}/%{name}/hddtemp.so
%endif

%if %{with_intel_pmu}
%files intel_pmu
%{_libdir}/%{name}/intel_pmu.so
%endif

%if %{with_intel_rdt}
%files intel_rdt
%{_libdir}/%{name}/intel_rdt.so
%endif

%if %{with_ipmi}
%files ipmi
%{_libdir}/%{name}/ipmi.so
%endif

%if %{with_iptables}
%files iptables
%{_libdir}/%{name}/iptables.so
%endif

%if %{with_java}
%files java
%{_datadir}/collectd/java/collectd-api.jar
%{_datadir}/collectd/java/generic-jmx.jar
%{_libdir}/%{name}/java.so
%{_mandir}/man5/collectd-java.5*
%endif

%if %{with_virt}
%files virt
%{_libdir}/%{name}/virt.so
%endif

%if %{with_log_logstash}
%files log_logstash
%{_libdir}/%{name}/log_logstash.so
%endif

%if %{with_lua}
%files lua
%{_mandir}/man5/collectd-lua*
%{_libdir}/%{name}/lua.so
%endif

%if %{with_lvm}
%files lvm
%{_libdir}/%{name}/lvm.so
%endif

%if %{with_memcachec}
%files memcachec
%{_libdir}/%{name}/memcachec.so
%endif

%if %{with_mic}
%files mic
%{_libdir}/%{name}/mic.so
%endif

%if %{with_modbus}
%files modbus
%{_libdir}/%{name}/modbus.so
%endif

%if %{with_mqtt}
%files mqtt
%{_libdir}/%{name}/mqtt.so
%endif

%if %{with_mongodb}
%files mongodb
%{_libdir}/%{name}/mongodb.so
%endif

%if %{with_mysql}
%files mysql
%{_libdir}/%{name}/mysql.so
%endif

%if %{with_netlink}
%files netlink
%{_libdir}/%{name}/netlink.so
%endif

%if %{with_nginx}
%files nginx
%{_libdir}/%{name}/nginx.so
%endif

%if %{with_notify_dbi}
%files notify_dbi
%{_libdir}/%{name}/notify_dbi.so
%endif

%if %{with_notify_desktop}
%files notify_desktop
%{_libdir}/%{name}/notify_desktop.so
%endif

%if %{with_notify_email}
%files notify_email
%{_libdir}/%{name}/notify_email.so
%endif

%if %{with_notify_snmp}
%files notify_snmp
%{_libdir}/%{name}/notify_snmp.so
%endif

%if %{with_nut}
%files nut
%{_libdir}/%{name}/nut.so
%endif

%if %{with_odbc}
%files odbc
%{_libdir}/%{name}/odbc.so
%endif

%if %{with_openldap}
%files openldap
%{_libdir}/%{name}/openldap.so
%endif

%if %{with_oracle}
%files oracle
%{_libdir}/%{name}/oracle.so
%endif

%if %{with_ovs_events}
%files ovs_events
%{_libdir}/%{name}/ovs_events.so
%endif

%if %{with_ovs_stats}
%files ovs_stats
%{_libdir}/%{name}/ovs_stats.so
%endif

%if %{with_perl}
%files perl
%{perl_vendorlib}/Collectd.pm
%{perl_vendorlib}/Collectd/
%{_mandir}/man3/Collectd::Unixsock.3pm*
%{_mandir}/man5/collectd-perl.5*
%{_libdir}/%{name}/perl.so
%endif

%if %{with_pcie_errors}
%files pcie_errors
%{_libdir}/%{name}/pcie_errors.so
%endif

%if %{with_pinba}
%files pinba
%{_libdir}/%{name}/pinba.so
%endif

%if %{with_ping}
%files ping
%{_libdir}/%{name}/ping.so
%endif

%if %{with_postgresql}
%files postgresql
%{_datadir}/collectd/postgresql_default.conf
%{_libdir}/%{name}/postgresql.so
%endif

%if %{with_procevent}
%files procevent
%{_libdir}/%{name}/procevent.so
%endif

%if %{with_python}
%files python
%{_mandir}/man5/collectd-python*
%{_libdir}/%{name}/python.so
%endif

%if %{with_redis}
%files redis
%{_libdir}/%{name}/redis.so
%endif

%if %{with_redfish}
%files redfish
%{_libdir}/%{name}/redfish.so
%endif

%if %{with_rrdcached}
%files rrdcached
%{_libdir}/%{name}/rrdcached.so
%endif

%if %{with_rrdtool}
%files rrdtool
%{_libdir}/%{name}/rrdtool.so
%endif

%if %{with_sensors}
%files sensors
%{_libdir}/%{name}/sensors.so
%endif

%if %{with_sigrok}
%files sigrok
%{_libdir}/%{name}/sigrok.so
%endif

%if %{with_smart}
%files smart
%{_libdir}/%{name}/smart.so
%endif

%if %{with_snmp}
%files snmp
%{_mandir}/man5/collectd-snmp.5*
%{_libdir}/%{name}/snmp.so
%endif

%if %{with_snmp_agent}
%files snmp_agent
%{_libdir}/%{name}/snmp_agent.so
%endif

%if %{with_sysevent}
%files sysevent
%{_libdir}/%{name}/sysevent.so
%endif

%if %{with_varnish}
%files varnish
%{_libdir}/%{name}/varnish.so
%endif

%if %{with_write_http}
%files write_http
%{_libdir}/%{name}/write_http.so
%endif

%if %{with_write_kafka}
%files write_kafka
%{_libdir}/%{name}/write_kafka.so
%endif

%if %{with_write_log}
%files write_log
%{_libdir}/%{name}/write_log.so
%endif

%if %{with_write_mongodb}
%files write_mongodb
%{_libdir}/%{name}/write_mongodb.so
%endif

%if %{with_write_prometheus}
%files write_prometheus
%{_libdir}/%{name}/write_prometheus.so
%endif

%if %{with_write_redis}
%files write_redis
%{_libdir}/%{name}/write_redis.so
%endif

%if %{with_write_riemann}
%files write_riemann
%{_libdir}/%{name}/write_riemann.so
%endif

%if %{with_write_stackdriver}
%files write_stackdriver
%{_libdir}/%{name}/write_stackdriver.so
%endif

%if %{with_write_syslog}
%files write_syslog
%{_libdir}/%{name}/write_syslog.so
%endif

%if %{with_gpu_nvidia}
%files write_gpu_nvidia
%{_libdir}/%{name}/write_gpu_nvidia.so
%endif

%if %{with_xencpu}
%files xencpu
%{_libdir}/%{name}/xencpu.so
%endif

%if %{with_xmms}
%files xmms
%{_libdir}/%{name}/xmms.so
%endif

%files collection3
%{_localstatedir}/www/collection3
%{_sysconfdir}/httpd/conf.d/collection3.conf

%files php-collection
%{_localstatedir}/www/php-collection
%{_sysconfdir}/httpd/conf.d/php-collection.conf

%files contrib
%doc contrib/

%changelog
* Tue Dec 6 2022 Carlos Peon <carlospeon@gmail.com> - 5.12.0.6-1
- Initial version
