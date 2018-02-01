## Einrichtung statische IP und statische Route ##

Statische IP: /etc/dhcpcd.conf
```
interface eth0
metric 400;
static ip_address=192.168.100.2
static routers=192.168.100.1
static domain_name_servers=192.168.100.1
static domain_search=
noipv6

interface wlan0
metric 200;
static 192.168.200.2
static routers=192.168.200.1
static domain_name_servers=192.168.200.1
noipv6 

```


Statische Route permanent
```
vi /lib/dhcpcd/dhcpcd-hooks/40-route

ip route add 44.0.0.0/8 via 192.168.100.1
```


