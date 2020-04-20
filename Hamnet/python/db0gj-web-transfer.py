#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Webseite von Hamnet lesen und im Internet speichern
# 20.04.2020 DL2NEW Sven
#

import re
from urllib import request # Ubuntu
# import urllib # Raspi
from ftplib import FTP_TLS

htmlfile = "/tmp/db0gj-cpy.html"
url = "http://44.225.178.194"
ftpurl = "dl2new.darc.de"
ftpuser = "ftpuser"
ftppass = "ftppass"

#
# Copy URL
#

url_requested = request.urlopen(url) # Ubuntu
#url_requested = urllib.urlopen(url) # Raspi
if 200 == url_requested.code:
    html_content = str(url_requested.read())

# "b'" am Anfang, CR und "'" am Ende entfernen
temp_clean0 = re.sub(r"b'<!DOC","<!DOC",html_content)
temp_clean1 = re.sub(r"\\n","",temp_clean0)
temp_clean2 = re.sub(r"ml>'","ml>",temp_clean1)
print(temp_clean2)

datei = open(htmlfile,'w')
datei.write(temp_clean2)
datei.close()

#
# ftp/TLS Transfer
#
ftps = FTP_TLS(ftpurl)
ftps.login(ftpuser, ftppass)
ftps.prot_p()          # switch to secure data connection.. IMPORTANT! Otherwise, only the user and password is encrypted and not all the file data.
ftps.retrlines('LIST')

myfile = open(htmlfile, 'rb')
ftps.storbinary("STOR test.html", myfile, blocksize=8192)

ftps.close()
myfile.close()
