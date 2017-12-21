#!/usr/bin/python
# -*- coding: utf-8
 
import urllib2
response = urllib2.urlopen('http://192.168.2.50')
raw = response.read()
 
#Python3
#import urllib.request
#with urllib.request.urlopen('http://192.168.2.50') as response:
#   raw = response.read()
 
#import wget
#url = '192.168.2.50' 
#wget.download(url, '/home/pi/tmp/weather.html') 
 
# Datei öffnen und in rawtext ablegen
#datei="/home/pi/tmp/weather.html"
#in_file = open(datei,"r")
#raw_text = in_file.read()
#in_file.close()
 
# Wörter trennen
split_text = raw.split(" ")
 
# Zeichenketten suchen und Werte ermitteln
for i in range(1, 100):
   if split_text[i] == "<p>Temperature:":
      temp = split_text[i+1]
   if split_text[i] == "<p>Pressure:":
      druck = split_text[i+1]
   if split_text[i] == "<p>Humidity:":
      feuchte = split_text[i+1]
      break
 
# Ausgabe
print temp,  druck,  feuchte
