#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# 11.04.2020 DL2NEW
#
# APRS Message senden inkl. Raspi Temperatur
#

import sys
import time
import socket
import re
import subprocess

#
# Definitionen
#
VERSION = "PyAPRS DL2NEW v1.0"
CALL= "call"
PASSWORD = "passcode"
#serverHost = 'rotate.aprs2.net'
serverHost = 'aprs.oe2xzr.ampr.org' # IP=44.143.40.90
serverPort = 14580
address = CALL + '>APZ091:'

#
# Raspi Temperatur auslesen mit SystemCMD und Steuerzeichen entfernen
#
temp = subprocess.check_output((['vcgencmd','measure_temp']))
# Steuerzeichen entfernen - import re
tempclean1 = re.sub(r"\n","",temp)
tempclean2 = re.sub(r"'","",tempclean1)

#
# comment length is supposed to be 0 to 43 char. long-this is 53 char. but it works
#
comment1 = ';DL2NEW-2 *111111z4936.44NR01103.49E&Rx-only iGate Spardorf via Hamnet 13cm - Raspi '
comment = comment1 + tempclean2

#
# Senden
#
my_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
my_sock.connect((serverHost, serverPort))
my_sock.send('user ' + CALL + ' pass ' + PASSWORD + ' vers ' + VERSION + 'filter p/LU/LW\n')
data = my_sock.recv(4096)
if data.startswith("#"):
    print("Succesfully login to " +serverHost)
my_sock.send(address + comment + '\n')
print "\033[1;36m"+address+comment+"\033[1;m"
print("\033[1;32mpacket sent: " + time.ctime()+"\033[1;m" )
time.sleep(2) # 2 sec. delay
my_sock.shutdown(0)
my_sock.close()
