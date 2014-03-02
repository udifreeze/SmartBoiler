#!/usr/bin/python

import time
import os
from datetime import datetime
import subprocess


ConfigFilePath = "/boiler/config.txt"
RefreshTime = 1
RelayOutputPin = 5
ArduinoRESTStr = "http://localhost/arduino/"


def BoilerOn():
	os.system("curl " + ArduinoRESTStr + "StartBoiler")
	
def BoilerOff():
	os.system("curl " + ArduinoRESTStr + "StopBoiler")
	
def CheckButton():
	ret = subprocess.check_output(["curl", "-s", ArduinoRESTStr + "CheckButton"])
	if(ret == "On"):
		return True	
	else:
		return False

def GetConfig():
	try:
		fd = open(ConfigFilePath, 'r')
	except(IOError):
		fd = open(ConfigFilePath, 'w')
		fd.close()
	finally:
		fd = open(ConfigFilePath, 'r')

	OnOffTimesList = list()
		
	for line in fd:
		(OnTime,OffTime) = line.split(" ", 2)
		(OnTime,OffTime) = (OnTime.strip(),OffTime.strip())
		OnOffTimesList.append((OnTime,OffTime))
		
	fd.close()
	return OnOffTimesList
		
def main():
	
	while(True):
		BoilerState = False
		BoilerState |= CheckButton()
	
		OnOffTimesList = GetConfig()
		if(len(OnOffTimesList) != 0):
			for OnOffTimes in OnOffTimesList:
				OnTime = datetime.strptime(OnOffTimes[0], "%H:%M") 
				OffTime = datetime.strptime(OnOffTimes[1], "%H:%M") 
			
				if( (datetime.now().time() >= OnTime.time()) & 
					(datetime.now().time() < OffTime.time()) ):
					BoilerState |= True
				else:
					BoilerState |= False
	
		if(BoilerState):
			BoilerOn()
		else:
			BoilerOff()
					
		time.sleep(RefreshTime)
	
if __name__ == "__main__":
    main()
