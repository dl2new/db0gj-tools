#!/bin/bash

sleep 30

#
# Nothing to do if it is already running.
#

a=`ps -ef | grep db0gj-v8 | grep -v grep`
if [ "$a" != "" ]
then
  #date >> /tmp/db0gj-start.log
  #echo "Already running." >> /tmp/db0gj-start.log
  exit
fi

#
# Restart
#

sudo /home/pi/db0gj/db0gj-v8
