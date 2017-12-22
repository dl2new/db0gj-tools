## Anpassungen DNS ##

/etc/resolv.conf
```
domain fritz.box
nameserver 192.168.x.x
nameserver fd00::...
nameserver 44.225.56.129
nameserver 44.225.28.20
```
Aktivierung: /etc/init.d/networking reload
