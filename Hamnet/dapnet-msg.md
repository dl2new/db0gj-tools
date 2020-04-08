## DAPNET Message senden ##

### Python code ###
```
#!/usr/bin/env python
#
# Basis: Philipp DL7FL mit unterstuezung von DH3RW (RWTH-AFU)
#
# Modification: DL2NEW - Sven - 08.04.2020
#

import os, sys
import logging
import requests
from requests.auth import HTTPBasicAuth
import json
import subprocess
import re

logging.basicConfig(level=logging.DEBUG, format="%(asctime)s;%(levelname)s;%(message)s")
logger = logging.getLogger(sys.argv[0])

#
# Variable
#
login = 'CALL' #  DAPNET Benutzername
passwd = 'PASSWD'  #  DAPNET Passwort
url = 'http://dapnet.db0sda.ampr.org:8080/calls'  #  versenden ueber HAMNET
callsign = "dl2new"  # Emfaenger Rufzeichen
txgroup = "dl-by"  #  Sendergruppe

#
# Raspi Temperatur auslesen mit SystemCMD und text zusammenstellen
#
temp = subprocess.check_output((['vcgencmd','measure_temp']))
# Steuerzeichen entfernen - import re
temp_clean1 = re.sub(r"\n","",temp)
temp_clean2 = re.sub(r"'","",temp_clean1)
text = "Raspi-DB0GJ OK " + temp_clean2

#
# Sendefunktion
#
def send(text, callsign, login, passwd, url, txgroup): # mit json modul machen
        """ Sendet JASON-String """
        json_string =json.dumps({"text": text, "callSignNames": [callsign], "transmitterGroupNames": [txgroup], "emergency": True})
        import pprint; pprint.pprint(json_string)
        response = requests.post(url, data=json_string, auth=HTTPBasicAuth(login, passwd)) # Exception handling einbauen
        return response.status_code

#
### Senden ###
#
send(text, callsign, login, passwd, url, txgroup)
```
