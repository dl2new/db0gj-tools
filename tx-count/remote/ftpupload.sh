#!/bin/bash
HOST='dl2new.darc.de'
#USER='w012ebdf'
USER='f00d52c7'
PASSWD='Speedz#23'
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

