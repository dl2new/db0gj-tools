## NTP anpassen ##

# /etc/ntp.conf
server 44.225.56.129 iburst
server 44.225.28.20 iburst

Aktivierung:
- sudo mv /var/lib/ntp/ntp.conf.dhcp /var/lib/ntp/ntp.conf.dhcp.org
- sudo /etc/init.d/ntp restart

Test:
- ntpq -p
- date
