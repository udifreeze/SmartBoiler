#!/usr/sbin

#create dir
mkdir /boiler

#copy files
cp ./boiler_daemon /etc/init.d/
cp ./boiler.py /boiler
cp ./boiler_config.py /www/cgi-bin/
cp ./boiler.html /www/

#make scripts exec
chmod +x /www/cgi-bin/boiler_config.py
chmod +x /www/boiler.html
chmod +x /etc/init.d/boiler_daemon
chmod +x /boiler/boiler.py

#start boiler.py
/etc/init.d/boiler_daemon enable
/etc/init.d/boiler_daemon start