#!/usr/bin/python


import os
import sys
import math


frame = 1

gammas  =[0.5,.75,1,2,3]
octaves =[1,1.5,2,2.5,3,4,5,6]
fjumps  =[1,1.5,2,2.5,3]
freqs   =[0.5,0.75,1,1.5,2]

print "index for Pyro Wedge"
print "frame gamma octaves fjump freq"

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
				command = padframe + "  " 
				command = command + str(gamma)
				for x in range(len(str(gamma)),6):
					command = command + " "
				command = command + str(octave)
				for x in range(len(str(octave)),len("octaves ")):
					command = command + " "
				command = command + str(fjump)
				for x in range(len(str(fjump)),len("fjump ")):
					command = command + " "							   
				command = command + str(freq)
							   
				print command
				if frame%50 == 0 and frame != 1000:
					print "frame gamma octaves fjump freq"
				frame += 1




'''



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
