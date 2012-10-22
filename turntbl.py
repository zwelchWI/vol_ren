#!/usr/bin/python


import os
import sys
import math

pbsGen= "./pbsGen.py"
executable = "volumerender.py"
picDir = "/home/zwelch/819/ZWELCH_819/renderer/pics/"
extension = "EXR"


def CmdLineFindIndex( tag ):
	for i in range(len(sys.argv)):
		if sys.argv[i] == tag:
			return i
	return -1

def CmdLineFind( tag, defaultvalue ):
	i = CmdLineFindIndex(tag)
	if i > 0:
		if i < len(sys.argv)-1:
			return sys.argv[i+1]
	return defaultvalue



frames = int(CmdLineFind("-frames",200))
ds = float(CmdLineFind("-ds",.025))
jobLabel = str( CmdLineFind( "-job", "MyJob" ) )


angle = 0.0
theta = 360.0 / float(frames)
radius = 25
frame = 0
while angle < 360.0:
	rad_angle = angle * math.pi/180.0
	cosine = math.cos(rad_angle)
	sine = math.sin(rad_angle)
	camera = [ radius * sine, 0, radius*cosine ]
	digits = 1
	padframe = str(frame)
	if frame < 1000:
		padframe = "0" + padframe
	if frame < 100:
		padframe = "0" + padframe
	if frame < 10:
		padframe = "0" + padframe

	command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -extras -ds " + str(ds) + " -camera " + str(camera[0]) + " " + str(camera[1]) + " " + str(camera[2]) + " -fname " + picDir + jobLabel + str(padframe) + "." +extension 
	print command
	frame += 1
	angle += theta
	os.system( command )
