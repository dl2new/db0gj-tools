#!/usr/bin/python
# -*- coding: utf-8
#
# Auslesen der Wetterdaten von Node-MCU
# Weiterverarbeitung dann durch Direwolf-APRS
# Test/Beispiel für DB0GJ-Telemetrie
# DL2NEW 22.12.2017
 
import urllib2
response = urllib2.urlopen('http://192.168.2.50')
raw = response.read()
 
# Woerter trennen
split_text = raw.split(" ")
 
# Zeichenketten suchen und Werte ermitteln
# Temp in C, Druck in hpa, Feuchte in %
for i in range(1, 100):
   if split_text[i] == "<p>Temperature:":
      temp = int(split_text[i+1])
   if split_text[i] == "<p>Pressure:":
      druck = int(split_text[i+1]) / 100
   if split_text[i] == "<p>Humidity:":
      feuchte = int(split_text[i+1])
      break
 
# Ausgabe
print temp,  druck,  feuchte
