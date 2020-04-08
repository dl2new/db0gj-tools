## Raspi Webseite dynamisch mit Temperatur ##

### Python code ###
```
!/usr/bin/env python
#
# Raspi Temberatur und Zeit auslesen und in index.html Platzhalter ersetzen
# DL2NEW - Sven, 08.04.2020
#
import fileinput
import re
import sys
import os
import subprocess
import time

#
# Filenamen
#
file1 = "/var/www/html/index.html.orig"
file2 = "/var/www/html/index.html"

#
# Raspi Temperatur auslesen mit SystemCMD und Steuerzeichen entfernen
#
temp = subprocess.check_output((['vcgencmd','measure_temp']))
# Steuerzeichen entfernen - import re
tempclean1 = re.sub(r"\n","",temp)
tempclean2 = re.sub(r"'","",tempclean1)

#
# Raspi Datum-Zeit auslesen mit SystemCMD
#
datetime2 = time.strftime("%d.%m.%Y %H:%M:%S")

#
# Urdatei kopieren
#
cmd1 = "cp " + file1 + " " + file2
resultcode = os.system(cmd1)

#
# Temperatur und DateTime erstetzen
#
file = fileinput.FileInput(file2, inplace=True, backup=".bak")

for line in file:
    line = re.sub(r"ttt_ttt", tempclean2, line)
    line = re.sub(r"ddd_ddd", datetime2, line)
    sys.stdout.write(line)

file.close()
```
