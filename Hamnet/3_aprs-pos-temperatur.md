## APRS Position senden inkl. Raspi Temperatur ##

### Python code ###
```
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# 12.04.2020 DL2NEW
#
# APRS Objekt senden: Pos und Text
# APRS Telemetrie senden: Raspi Temperatur
#

import sys
import time
import socket
import re
import subprocess

#--------------------------------------------------------------------
#
# Definitionen
#
VERSION = "PyAPRS DL2NEW v1.1"
CALL= "DL2NEW-2"
PASSWORD = "16567"
#iGateHost = 'rotate.aprs2.net'
iGateHost = 'aprs.oe2xzr.ampr.org' # IP=44.143.40.90
iGatePort = 14580
address = CALL + '>APZ091:'
seqFileName = '/tmp/aprs-tele-seq.txt' # Datei muss vorhanden sein mit 3 Zeichen 000 bis 999

#--------------------------------------------------------------------
#
# Raspi Temperatur auslesen mit SystemCMD und Steuerzeichen entfernen
#
temp = subprocess.check_output((['vcgencmd','measure_temp']))

# Temperatur "ganze Zahl" ermitteln - CPU Temperatur ist immer positiv
tempStrip = temp[temp.find('temp=')+5:temp.find('.')]
# Länge ermitteln und ggf. mit NULL ergaenzen
c=len(tempStrip)
if (c == 1):
    tempFinal = "00" + tempStrip
else:
    if (c == 2):
        tempFinal = "0" + tempStrip
    else:
        if (c == 3):
            tempFinal = tempStrip
        else:
            tempFinal = "999"

#--------------------------------------------------------------------
#
# Telemetrie-Sequenznummer hochzählen
#
seqFile = open(seqFileName,'r')
string = seqFile.read()
seqFile.close()
#
zahl = int(string)
zahl += 1
if(zahl==1000): zahl = 0
stringNeu = str(zahl)
c=len(stringNeu)
if (c == 1): stringFinal = "00" + stringNeu
if (c == 2): stringFinal = "0" + stringNeu
if (c == 3): stringFinal = stringNeu
#
seqFile = open(seqFileName,'w')
seqFile.write(stringFinal)
seqFile.close()

#--------------------------------------------------------------------
#
# Comment-Länge: 0 bis 43 oder auch mehr
# comment1: Position und Text
# comment2: Telemetrie - Raspi Temperatur
#
comment1 = ';DL2NEW-2 *111111z4936.44NR01103.49E&Rx-only iGate Spardorf via Hamnet 13cm - Telemetry Raspi-Temp'
comment2 = "T#" + stringFinal + "," + tempFinal + ",000,000,000,000,00000000"

#--------------------------------------------------------------------
#
# Senden: coment1 un dcomment2 mit 2s Abstand
#
my_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
my_sock.connect((iGateHost, iGatePort))
my_sock.send('user ' + CALL + ' pass ' + PASSWORD + ' vers ' + VERSION + 'filter p/LU/LW\n')
data = my_sock.recv(4096)
if data.startswith("#"):
    print("Succesfully login to " +iGateHost)
my_sock.send(address + comment1 + '\n')
print "\033[1;36m"+address+comment1+"\033[1;m"
print("\033[1;32mpacket sent: " + time.ctime()+"\033[1;m" )
time.sleep(2) # 2 sec. delay
my_sock.send(address + comment2 + '\n')
print "\033[1;36m"+address+comment2+"\033[1;m"
print("\033[1;32mpacket sent: " + time.ctime()+"\033[1;m" )
time.sleep(2) # 2 sec. delay
my_sock.shutdown(0)
my_sock.close()
```
