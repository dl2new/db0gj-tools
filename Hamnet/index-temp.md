## Raspi Webseite dynamisch mit Temperatur ##

### Python code ###
```
#!/usr/bin/env python
#
# DL2NEW - Sven, 08.04.2020
#
import fileinput
import re
import sys
import os
import subprocess

#
# Raspi Temperatur auslesen mit SystemCMD und text zusammenstellen
#
temp = subprocess.check_output((['vcgencmd','measure_temp']))
# Steuerzeichen entfernen - import re
temp_clean1 = re.sub(r"\n","",temp)
temp_clean2 = re.sub(r"'","",temp_clean1)

#
# Urdatei kopieren
#
resultcode = os.system("cp /var/www/html/index.html.orig /var/www/html/index.html")

file = fileinput.FileInput("/var/www/html/index.html", inplace=True, backup=".bak")

for line in file:
    line = re.sub(r"ttt_ttt", temp_clean2, line)
    sys.stdout.write(line)

file.close()
```
