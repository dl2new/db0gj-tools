## Einrichtung VPN auf Raspi ##

### Einrichtung manuell / Test ###
```
sudo pptpsetup --create HamnetDB0SDA --server vpn.afu.rwth-aachen.de --username XXXXX --password XXXXX
sudo pppd call HamnetDB0SDA updetach
mit ifconfig ip und GW kontrollieren - GW dann für die nächste Zeile
sudo route add -net 44.0.0.0/8 gw 44.225.57.65 ppp0
ping db0kwe.ampr.org
```
### Einrichtung permanent ###

/etc/ppp/HamnetDB0SDA
```
pty "pptp vpn.afu.rwth-aachen.de --nolaunchpppd"
lock
noauth
nobsdcomp
nodeflate
name <rufzeichen>
password <pass vom vpn>
remotename HamnetDB0SDA
ipparam HamnetDB0SDA
```
Crontab -e (user pi)
```
0 6 * * * /home/pi/Hamnet.sh
0 12 * * * /home/pi/Hamnet.sh
0 18 * * * /home/pi/Hamnet.sh
0 23 * * * /home/pi/Hamnet.sh
```
/home/pi/Hamnet.sh - chmod 755 Hamnet.sh
```
sudo killall pppd
sudo pppd call HamnetDB0SDA updetach
sleep 5
sudo route add -net 44.0.0.0/8 gw 44.225.57.65 ppp0
```
Autostart beim Hochlauf /etc/rc.local
```
echo "rc.local: Starting Hamnet"
sleep 20
sudo pppd call HamnetDB0SDA updetach
sleep 5
sudo route add -net 44.0.0.0/8 gw 44.225.166.1 ppp0
echo "Done!"
```
