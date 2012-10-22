#!/usr/bin/python


import os
import sys
import math

pbsGen= "./pbsGen.py"
executable = "volumerender.py"
execName = "renderRobot"
pbsName = "robot"
picDir = "/home/zwelch/819/ZWELCH_819/renderer/pics/"
extension = "exr"

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



ds = float(CmdLineFind("-ds",.05))
jobLabel = "robot"

frame = 86
eye = 0
while frame <= 106:
	padframe = str(frame)
	if frame < 1000:
		padframe = "0" + padframe
	if frame < 100:
		padframe = "0" + padframe
	if frame < 10:
		padframe = "0" + padframe
	command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -execName "+execName + " -pbsName "+pbsName + " -extras" + " -fname " + picDir + jobLabel +"/"+jobLabel +"."+ str(padframe) + "." + extension + " -ds " + str(ds) + " -roboC " +str(eye) 
	print command
	frame += 1
	eye += 0.05
	os.system(command)
