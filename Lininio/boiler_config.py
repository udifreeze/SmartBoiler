#!/usr/bin/python

import cgi
import os

ConfigFilePath = "/boiler/config.txt"
GUIFilePath = "/boiler/gui_config.txt"


print "Content-Type: text/html"
print ""

arguments = cgi.FieldStorage()
	
OnList = arguments.getlist("on")
OffList = arguments.getlist("off")

if(len(OnList) != len(OffList)):
	print("On's and Off's should be even <br>")
else:
	print("Writing to configuration <br>")
	fd = open(ConfigFilePath, 'w')
	for i in xrange(0, len(OnList)):
		fd.write(OnList[i] + " " + OffList[i] + "\n")
	fd.close()

GUIString = ['-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-']

fd = open(GUIFilePath, 'w')
for i in xrange(0, len(OnList)):
	TimeOn = float(OnList[i].replace(":30", ":50").replace(':','.'))	
	TimeOff = float(OffList[i].replace(":30",":50").replace(':','.'))
	for i in xrange(int(TimeOn),int(TimeOff)):
		GUIString[i] = '*'
		
fd.write("".join(GUIString))
fd.close()
fd.close()