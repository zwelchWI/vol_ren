#!/usr/bin/python

import os
import sys
import datetime


output = sys.argv[len(sys.argv)-1]
nbframes = len(sys.argv)-2
print "Number of frames " + str(nbframes)
if nbframes > 1:
	print  str(datetime.datetime.now()) + " : " + sys.argv[1]
	
	command = "cp " + sys.argv[1] + " " + output
	#print command
	os.system(command)
	for image in sys.argv[2:nbframes+1]:
		command = "composite -blend 100x100% " + output + " " + image + " " + output
		print  str(datetime.datetime.now()) + " : " + image
		os.system(command)

shortlogo = ":o)"
username = os.getenv("USER");
currenttime =  str(datetime.datetime.now())
label = str(username) + "   " + currenttime
print "User: " + str(username) + " at time " + currenttime
command = "convert -fill white -draw 'gravity SouthWest text  10,10 \"" + label + "\"' " + output + " " + output
#print command
os.system(command)
command = "convert -fill white -draw 'gravity SouthEast text  10,10 \"" + output + "\"' " + output + " " + output
#print command
os.system(command)
command = "convert -fill white -draw 'gravity South text  0,10 \"" + shortlogo + "\"' " + output + " " + output
#print command
os.system(command)
print "File " + output + " created by " + str(username) + " at " + currenttime