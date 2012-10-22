#!/usr/bin/python


import os
import sys
import math

pbsGen= "./pbsGen.py"
executable = "volumerender.py"
execName = "renderBoom"
pbsName = "boom"
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
jobLabel = CmdLineFind("-job","boomFlame")
scale = float(CmdLineFind("-scale",1))

#pbsName = "flame"+str(scale)
frame = 111
radius1 = 0
radius2 = 0
radius3 = 0
radius4 = 0
radius5 = 0
roughness = 0
temp1 = 3000
temp2 = 3000
temp3 = 3000
temp4 = 3000
temp5 = 3000

octave1 = 2
octave2 = 2
octave3 = 2
octave4 = 2
octave5 = 2
fjump = 2
angle1 = 0.0
angle2 = 0.0
angle3 = 0.0
angle4 = 0.0
angle5 = 0.0
theta = 360.0/50.0

x1 = 0
y1 = 1
x2 = 0
y2 = 1
x3 = 0
y3 = 1
x4 = 0
y4 = 1
x5 = 0
y5 = 1



while frame < 210:
	rad_angle = angle1 * math.pi/180.0
	y1 = .5*.3 * math.cos(rad_angle)
	x1 = .5*.3 * math.sin(rad_angle)
	angle1 += theta
	
	
	padframe = str(frame)
	if frame < 1000:
		padframe = "0" + padframe
	if frame < 100:
		padframe = "0" + padframe
	if frame < 10:
		padframe = "0" + padframe
	command = command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -execName "+execName + " -pbsName "+pbsName + " -extras" + " -fname " + picDir + jobLabel +"/"+jobLabel +"."+ str(padframe) + "." + extension + " -ds " + str(ds) +" -radius1 " + str(radius1) + " -roughness1 .75 -trans1 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump1 " + str(fjump) + " -temp1 " + str(temp1) + " -octaves1 " + str(octave1) + " -pos1 " + str(x1) + " " +str(y1) + " 0 "
	
	if frame > 121:
		rad_angle = angle2 * math.pi/180.0
		y2 = .5*.3 * math.cos(rad_angle)
		x2 = .5*.3 * math.sin(rad_angle)
		angle2+=theta
		if radius2 < 1.0*.3:
			radius2 = radius2 + 1/50.0
		if temp2 < 6000:
			temp2 = temp2 + 100
		octave2 += 1/50.0
	command = command + "-radius2 " + str(radius2) + " -roughness2 .75 -trans2 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump2 " + str(fjump) + " -temp2 " + str(temp2) + " -octaves2 " + str(octave2) + " -pos2 " + str(x2) + " " +str(y2) + " 0 "
	
	if frame > 131:
		rad_angle = angle3 * math.pi/180.0
		y3 = .5*.3 * math.cos(rad_angle)
		x3 = .5*.3 * math.sin(rad_angle)
		angle3+=theta
		if radius3 < 1.0*.3:
			radius3 = radius3 + 1/50.0
		if temp3 < 6000:
			temp3 = temp3 + 100
		octave3 += 1/50.0
	command = command + "-radius3 " + str(radius3) + " -roughness3 .75 -trans3 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump3 " + str(fjump) + " -temp3 " + str(temp3) + " -octaves3 " + str(octave3) + " -pos3 " + str(x3) + " " +str(y3) + " 0 "
	
	if frame > 141:
		rad_angle = angle4 * math.pi/180.0
		y4 = .5*.3 * math.cos(rad_angle)
		x4 = .5*.3 * math.sin(rad_angle)
		angle4+=theta
		if radius4 < 1.0*.3:
			radius4 = radius4 + 1/50.0
		if temp4 < 6000:
			temp4 = temp4 + 100
		octave4 += 1/50.0
	command = command + "-radius4 " + str(radius4) + " -roughness4 .75 -trans4 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump4 " + str(fjump) + " -temp4 " + str(temp4) + " -octaves4 " + str(octave4) + " -pos4 " + str(x4) + " " +str(y4) + " 0 "
	
	if frame > 151:
		rad_angle = angle5 * math.pi/180.0
		y5 = .5*.3 * math.cos(rad_angle)
		x5 = .5*.3 * math.sin(rad_angle)
		angle5+=theta
		if radius5 < 1.0*.3:
			radius5= radius5 + 1/50.0
		if temp5 < 6000:
			temp5 = temp5 + 100
		octave5 += 1/50.0
	command = command + "-radius5 " + str(radius5) + " -roughness5 .75 -trans5 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump5 " + str(fjump) + " -temp5 " + str(temp5) + " -octaves5 " + str(octave5) + " -pos5 " + str(x5) + " " +str(y5) + " 0 -scale " + str(scale) 
	
	
	
	
	print command
	if radius1 < 1*.3:
		radius1 = radius1 + 1/50.0
	frame = frame + 1
	roughness = roughness + 1/10.0
	if frame > 161:
		fjump = fjump + 1/50.0
	if temp1 < 6000:
		temp1 = temp1 + 100
	octave1 = octave1 + 1/50.0
#	os.system(command)
x = 0
y = 0
z = 0
frame = 316
while frame < 361:
	rad_angle = angle1 * math.pi/180.0
	y1 = .5 * math.cos(rad_angle)
	x1 = .5 * math.sin(rad_angle)
	angle1 += theta
	x += 3/45.0
	y += .75/45.0
	z += 1.5/45.0
	
	padframe = str(frame)
	if frame < 1000:
		padframe = "0" + padframe
	if frame < 100:
		padframe = "0" + padframe
	if frame < 10:
		padframe = "0" + padframe
	command = pbsGen + " -script " + executable + " -frame " + str(frame) + " -job " +str(jobLabel) + " -execName "+execName + " -pbsName "+pbsName +" -extras" + " -fname " + picDir + jobLabel +"/"+jobLabel+"." + str(padframe) + "." + extension + " -frame "+str(frame) + " -ds " + str(ds) + " -radius1 " + str(radius1) + " -roughness1 .75 -trans1 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump1 " + str(fjump) + " -temp1 " + str(temp1) + " -octaves1 " + str(octave1) + " -pos1 " + str(x+x1) + " " +str(y + y1) +" "+ str(z)
	
		
	rad_angle = angle2 * math.pi/180.0
	y2 = .5 * .3 * math.cos(rad_angle)
	x2 = .5 * .3 * math.sin(rad_angle)
	angle2+=theta
	if temp2 > 3000:
		temp2 = temp2 - 100
	octave2 -= 1/50.0
	command = command + " -radius2 " + str(radius2) + " -roughness2 .75 -trans2 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump2 " + str(fjump) + " -temp2 " + str(temp2) + " -octaves2 " + str(octave2) + " -pos2 " + str(x + x2) + " " +str(y + y2) +" "+ str(z)
	
	rad_angle = angle3 * math.pi/180.0
	y3 = .5 * .3 * math.cos(rad_angle)
	x3 = .5 * .3 * math.sin(rad_angle)
	angle3+=theta
	if temp3 > 3000:
		temp3 = temp3 - 100
	octave3 -= 1/50.0
	command = command + " -radius3 " + str(radius3) + " -roughness3 .75 -trans3 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump3 " + str(fjump) + " -temp3 " + str(temp3) + " -octaves3 " + str(octave3) + " -pos3 " + str(x + x3) + " " +str(y + y3) +" "+ str(z)
	
	rad_angle = angle4 * math.pi/180.0
	y4 = .5 * .3 * math.cos(rad_angle)
	x4 = .5 * .3 * math.sin(rad_angle)
	angle4+=theta
	if temp4 > 3000:
		temp4 = temp4 - 100
	octave4 -= 1/50.0
	command = command + " -radius4 " + str(radius4) + " -roughness4 .75 -trans4 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump4 " + str(fjump) + " -temp4 " + str(temp4) + " -octaves4 " + str(octave4) + " -pos4 " + str(x + x4) + " " +str(y + y4) +" "+ str(z)
	
	rad_angle = angle5 * math.pi/180.0
	y5 = .5 * .3 * math.cos(rad_angle)
	x5 = .5 * .3 * math.sin(rad_angle)
	angle5+=theta
	if temp5 >3000:
		temp5 = temp5 - 100
	octave5 -= 1/50.0
	command = command + " -radius5 " + str(radius5) + " -roughness5 .75 -trans5 " + str(roughness) + " "+ str(roughness) + " "+ str(roughness) + " -fjump5 " + str(fjump) + " -temp5 " + str(temp5) + " -octaves5 " + str(octave5) + " -pos5 " + str(x + x5) + " " +str( y + y5) +" "+ str(z)
	
	
	
	
	
	
	
	print command
	frame = frame + 1
	roughness = roughness + 1/10.0
	fjump = fjump + 1/50.0
	if temp1 > 3000:
		temp1 = temp1 - 100
	octave1 = octave1 - 1/50.0
#	os.system(command)


