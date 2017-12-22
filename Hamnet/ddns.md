## Einrichtung DDNS auf Raspi fuer Hamnet ##

http://http://ddns.de.ampr.org/howto.php

crontab -e (jede Stunde / Minute 0 Abgleich durchführen – unter pi)
```
0 * * * * wget -q http://ddns.de.ampr.org/ddns.php?call=dl2new
```
FQDN: dl2new.ddns.de.ampr.org
