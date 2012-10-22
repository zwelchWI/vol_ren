#!/usr/bin/python


import os
import sys
import math

pbsGen= "./pbsGen.py"
executable = "volumerender.py"
execName = "renderFsim"
pbsName = "fsim"
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



ds = float(CmdLineFind("-ds",.01))
jobLabel = "fluidSim"
dt = float(CmdLineFind("-dt",.01))

frame = 1

while frame <= 200:

	padframe = str(frame)
	if frame < 1000:
		padframe = "0" + padframe
	if frame < 100:
		padframe = "0" + padframe
	if frame < 10:
		padframe = "0" + padframe
	command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -execName "+execName + " -pbsName "+pbsName + " -extras" + " -fname " + picDir + jobLabel +"/"+jobLabel +"."+ str(padframe) + "." + extension + " -ds " + str(ds) + " -frame " + str(frame) + " -NX 960 -NY 540 -dt " + str(dt)  	
	print command
	frame += 1
	os.system(command)
