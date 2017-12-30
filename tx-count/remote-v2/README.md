## DB0GJ Erfassung Aktivitaet ##

crontab -e
```
* * * * * /home/pi/db0gj/db0gj-start.sh
*/10 * * * * bash /home/pi/db0gj/ftpupload.sh
```

ftpupload.sh
```
#!/bin/bash
HOST='xxx'
USER='xxx"
PASSWD='xxx'
cd /home/pi/db0gj
ftp -n -v $HOST << EOT
ascii
user $USER $PASSWD
prompt
cd db0gj
mput index.html
mput db0gj-log.txt
bye
EOT
```

Raspberry Pins
```
Leiste oben
2 4 6 8 10 12 14 16 18 20 22
1 3 5 7 9  11 13 15 17 19 21
LED:  Pin6=GND  Pin22=GPIO25, Vorwiderstand 330Ohm
Foto: Pin1=3,3V Pin11=GPIO17
```
