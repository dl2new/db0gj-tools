#!/usr/bin/python
# -*- coding: utf-8
#
# Web-Seite mit Zaehlerstaenden generieren
#
# DL2NEW 27.12.2017

import datetime

index_file = '/home/pi/db0gj/index.html'
cnt_file = '/home/pi/db0gj/db0gj.cnt'
#index_file = '/home/sven/src/db0gj-tools/tx-count/remote-v2/index.html'
#cnt_file = '/home/sven/src/db0gj-tools/tx-count/remote-v2/db0gj.cnt'

# Datei mit Zählerständen öffnen und in rawtext ablegen
in_file = open(cnt_file,"r")
raw = in_file.read()
in_file.close()

# Werte trennen
split_text = raw.split("\n")

ohne_qso = split_text[2]
mit_qso = split_text[3]
qso_total = split_text[4]
qso_max = split_text[5]

zeit_start = datetime.datetime.fromtimestamp(int(split_text[0])).strftime('%Y-%m-%d %H:%M:%S')
zeit_aktuell = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

message1 = """<html>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>DB0GJ Relais Statistik</title>
<style type="text/css">
body {
	background-color:transparent;
}
#db0gj {
	margin:auto;
	font-family:Verdana, Arial, Helvetica, sans-serif;
	text-align:left;
	width:99%;
	padding:0;
	background:##BCF5A9;
	border:1px solid #088A08;
}
#db0gj h4 {
	font-size:10pt;
	font-weight:bold;
	display:block;
	background-color:#088A08;
	margin:0;
	padding:2px 8px;
}
#db0gj p {
	font-size:9pt;
	margin:0;
	padding:2px 8px;
}
.strap {
	font-size:7pt;
}
</style>
</head>
<body>
<div id="db0gj">
	<h4>DB0GJ Relais Statistik</h4>
	<p><b>ohne QSO:</b> """

message2 = """</p>
	<p><b>mit QSO:</b> """

message3 = """</p>
	<p><b>QSO-total:</b> """

message4 = """s</p>
	<p><b>QSO-max:</b> """

message5 = """s</p>
	<p><span class="strap">Zeit Start: """

message6 = """</a></span></p>
	<p><span class="strap">Zeit aktuell: """

message7 = """</a></span></p>
	<p><span class="strap">Info und Log: <a href="http://dl2new.darc.de/db0gj/info.html" rel="nofollow" target="_blank">hier</a></span></p>
</div>
</body>
</html>"""

message = message1 + ohne_qso + message2 + mit_qso + message3 + qso_total + message4 + qso_max + message5 + zeit_start + message6 + zeit_aktuell + message7

f = open(index_file,'w')
f.write(message)
f.close()
