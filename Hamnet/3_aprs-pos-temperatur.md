## APRS Position senden inkl. Raspi Temperatur ##

### Python code ###
```
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# 13.04.2020 DL2NEW
#
# APRS Objekte senden: DB0GJ Pos+Telemetrie und Objekte
# Telemetrie: Raspi Temperatur
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
VERSION = "PyAPRS DL2NEW v1a"
CALL= "DB0GJ"
PASSWORD = "passcode"
iGateHost = 'aprs.oe2xzr.ampr.org' # IP=44.143.40.90
iGatePort = 14580
aprsAddress = CALL + '>APZ091:'
seqFileName = '/tmp/aprs-tele-seq.txt' # Datei muss vorhanden sein mit 3 Zeichen 000 bis 999
debug = 1

#--------------------------------------------------------------------
#
# Raspi Temperatur auslesen mit SystemCMD und Steuerzeichen entfernen
#
temp = subprocess.check_output((['vcgencmd','measure_temp']))
#
# Temperatur "ganze Zahl" ermitteln - CPU Temperatur ist immer positiv
tempStrip = temp[temp.find('temp=')+5:temp.find('.')]
#
# Länge ermitteln und ggf. mit NULL ergaenzen
tempLen = len(tempStrip)
if (tempLen == 1):
    tempFinal = "00" + tempStrip
else:
    if (tempLen == 2):
        tempFinal = "0" + tempStrip
    else:
        if (tempLen == 3):
            tempFinal = tempStrip
        else:
            tempFinal = "999"
if(debug == 1): print("RaspiTemp=" + tempFinal)

#--------------------------------------------------------------------
#
# Telemetrie-Sequenznummer hochzählen
#
seqFile = open(seqFileName,'r')
seqString = seqFile.read()
seqFile.close()
#
seqZahl = int(seqString)
seqZahl += 1
if(seqZahl == 1000): seqZahl = 0
seqStringNeu = str(seqZahl)
seqStringLen = len(seqStringNeu)
if (seqStringLen == 1): seqStringFinal = "00" + seqStringNeu
if (seqStringLen == 2): seqStringFinal = "0" + seqStringNeu
if (seqStringLen == 3): seqStringFinal = seqStringNeu
#
seqFile = open(seqFileName,'w')
seqFile.write(seqStringFinal)
seqFile.close()
#
if(debug == 1): print("Seq=" + seqStringFinal)

#--------------------------------------------------------------------
#
# Function Report senden
#
def aprsSend(report):
    if(debug == 1): print("Connect ...")
    my_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    my_sock.connect((iGateHost, iGatePort))
    my_sock.send('user ' + CALL + ' pass ' + PASSWORD + ' vers ' + VERSION + 'filter p/LU/LW\n')
    data = my_sock.recv(4096)
    if data.startswith("#"):
        print("Succesfully login to " +iGateHost)
    my_sock.send(aprsAddress + report + '\n')
    print "\033[1;36m"+aprsAddress+report+"\033[1;m"
    print("\033[1;32mpacket sent: " + time.ctime()+"\033[1;m" )
    time.sleep(2) # 2 sec. delay
    my_sock.shutdown(0)
    my_sock.close()

#--------------------------------------------------------------------
#
# Report-Länge: 0 bis 43 oder auch mehr
# Report1: Position und Text
# Report2: Telemetrie - Raspi Temperatur
# Telemetrie einmalig Bezeichnung und Einheit: ":DB0GJ    :PARM.RaTemp" ":DB0GJ    :UNIT.C"
#
reportObj1 = ";DB0GJ    *111111z4937.02N/01103.80Er439.125MHz Relais Erlangen B31 and 5GHz Hamnet to DB0VOX - Telemetry Raspi-Temp"
reportTele1 = "T#" + seqStringFinal + "," + tempFinal + ",000,000,000,000,00000000"
reportObj2 = ";B31 OV   *111111z4935.81N\\01103.87ERBis auf weiteres kein OV Abend im Gasthaus Alter Brunnen in Marloffstein"
reportObj3 = ";B31 Field*111111z4937.25N/01103.31E;B31 Fieldday wieder im Sommer 2020"

#--------------------------------------------------------------------
#
# Einzelne Reports senden
#
aprsSend(reportObj1)
aprsSend(reportTele1)
aprsSend(reportObj2)
aprsSend(reportObj3)
```
