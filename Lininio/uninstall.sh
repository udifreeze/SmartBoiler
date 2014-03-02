#! /usr/sbin

pkill -f boiler
rm /boiler/boiler.py
rm -r /boiler
/etc/init.d/boiler_daemon disable
rm /etc/init.d/boiler_daemon
rm /www/boiler.html
rm /www/cgi-bin/boiler_config.py

