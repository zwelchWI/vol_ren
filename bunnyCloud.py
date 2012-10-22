#!/usr/bin/python


import os
import sys
import math

pbsGen= "./pbsGen.py"
executable = "volumerender.py"
execName = "render"
pbsName = "bunny"
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
jobLabel = "zwelch_hw4"


frame = 1

while frame <= 170:

	padframe = str(frame)
	if frame < 1000:
		padframe = "0" + padframe
	if frame < 100:
		padframe = "0" + padframe
	if frame < 10:
		padframe = "0" + padframe
	command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -execName "+execName + " -pbsName "+pbsName + " -extras" + " -fname " + picDir + jobLabel +"/"+jobLabel +"."+ str(padframe) + "." + extension + " -ds " + str(ds) + " -frame " + str(frame)  	
	print command
	frame += 1
	os.system(command)

'''
for gamma in gammas:
	for octave in octaves:
		for fjump in fjumps:
			for freq in freqs:
				padframe = str(frame)
				if frame < 1000:
					padframe = "0" + padframe
				if frame < 100:
					padframe = "0" + padframe
				if frame < 10:
					padframe = "0" + padframe
				command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -execName "+execName + " -pbsName "+pbsName + " -extras" + " -NX 960 -NY 540 -fname " + picDir + jobLabel +"/"+jobLabel +"."+ str(padframe) + "." + extension + " -ds " + str(ds) + " -octaves " + str(octave) + " -fjump " + str(fjump) + " -freq " + str(freq)+ " -pyroGamma " + str(gamma)  	
				print command
				frame += 1
				os.system(command)


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
	os.system( command )'''
