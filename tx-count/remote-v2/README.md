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
