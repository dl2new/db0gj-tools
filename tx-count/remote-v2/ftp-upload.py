#!/usr/bin/python
# -*- coding: utf-8
#
# ftp upload von Web-Seite
#
# DL2NEW 27.12.2017

import ftplib

server = 'dl2new.darc.de'
username = 'user'
password = 'pass'
file = '/home/pi/db0gj/index.html'

ftp_connection = ftplib.FTP(server, username, password)

remote_path = "/db0gj/"
ftp_connection.cwd(remote_path)

fh = open(file, 'rb')
ftp_connection.storbinary('STOR index.html', fh)
fh.close()
