## DB0GJ Erfassung Aktivitaet ##

crontab -e
* * * * * /home/pi/db0gj/db0gj-start.sh
*/10 * * * * bash /home/pi/db0gj/ftpupload.sh
