#!/usr/bin/python


import os
import sys
import math

pbsGen= "./pbsGen.py"
executable = "volumerender.py"
execName = "renderSpin"
pbsName = "advect"
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
jobLabel = "finalBunny"

angle = 0.0
theta = 360.0 / float(50)
radius = 20
eggH = 10.0
frame = 1

while frame <= 50:
	rad_angle = angle * math.pi/180.0
	cosine = math.cos(rad_angle)
	sine = math.sin(rad_angle)
	camera = [ radius * sine, 0, radius*cosine ]
	padframe = str(frame)
	if frame < 1000:
		padframe = "0" + padframe
	if frame < 100:
		padframe = "0" + padframe
	if frame < 10:
		padframe = "0" + padframe
	command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -execName "+execName + " -pbsName "+pbsName + " -extras" + " -fname " + picDir + jobLabel +"/"+jobLabel +"."+ str(padframe) + "." + extension + " -ds " + str(ds) + " -time " + str(50 - frame) + " -camera " + str(camera[0]) + " " + str(camera[1]) + " " + str(camera[2])	+ " -eggH " + str(eggH) + " -angle " + str(math.pi*frame/2/50)
	print command
	frame += 1
	eggH -= 10.0/50.0
	angle += theta
#os.system(command)

changeCam = 0
while frame <=60:
	padframe = str(frame)
	if frame < 1000:
		padframe = "0" + padframe
	if frame < 100:
		padframe = "0" + padframe
	if frame < 10:
		padframe = "0" + padframe
	command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -execName "+execName + " -pbsName "+pbsName + " -extras" + " -fname " + picDir + jobLabel +"/"+jobLabel +"."+ str(padframe) + "." + extension + " -ds " + str(ds) + " -camera " + str(0) + " " + str(0) + " " + str(20+changeCam) 
	print command
	frame += 1
	changeCam += 0.1
#os.system(command)

