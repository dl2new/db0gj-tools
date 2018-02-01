## Einrichtung statische IP und statische Route ##

Statische IP
```
/etc/dhcpcd.conf
```


Statische Route permanent
```
vi /lib/dhcpcd/dhcpcd-hooks/40-route

ip route add 192.168.100.0/24 via 192.168.0.2
```


