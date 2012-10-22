#!/usr/bin/python


import sys
import os



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



scriptToRun = CmdLineFind( "-script", "volumerender.py" )
frame = int( CmdLineFind( "-frame", 0 ) )
nbCores =  int(CmdLineFind( "-nbcores", 1 ))
jobLabel = str( CmdLineFind( "-job", "MyJob" ) )
execName = str( CmdLineFind( "-execName", "FAILZ" ) ) 
pbsName = str( CmdLineFind( "-pbsName", "MyJob" ) )
submitToPBS = int(CmdLineFindIndex("-submittopbs"))

walltime = int( CmdLineFind("-walltime", 30) )
wally = str(walltime)
if walltime < 10:
	wally = "0" + wally

user = os.getenv("USER")

padframe = str(frame)
if frame < 1000:
	padframe = "0" + padframe
if frame < 100:
	padframe = "0" + padframe
if frame < 10:
	padframe = "0" + padframe

padframe = "." + padframe

pbsname = pbsName + padframe
boilerplate = "#PBS -N " + pbsname + "\n"
boilerplate = boilerplate + "#PBS -l nodes=" + str(nbCores) + "\n"
boilerplate = boilerplate + "#PBS -l walltime=" + wally + ":00:00\n#PBS -k oe\n"
boilerplate = boilerplate + "#PBS -M " + user + "@clemson.edu\n"
boilerplate = boilerplate + "#PBS -m a\n"


job = boilerplate + "\n"
job = job + "/home/" + user + "/819/ZWELCH_819/renderer/" + str(scriptToRun) + "  "
job = job + " -frame " + str(frame) +  " -nbpatches " + str(nbCores) + " -joblabel " + jobLabel + " -execName " + execName 

extras = int( CmdLineFindIndex("-extras") )
if extras > 0:
	job = job + " -extras "
	for task in sys.argv[extras+1:]:
		job = job + str(task) + " "
job = job + "\n"

print "Job:\n" + job + "\n\n"


pbsDirectory = "/home/" + user + "/819/ZWELCH_819/renderer/submitfiles/" + jobLabel
if os.path.exists(pbsDirectory) == False:
	os.mkdir(pbsDirectory)

pbsfilename = pbsDirectory + "/" + pbsName + padframe + ".pbs"
jobfile = open( pbsfilename, 'w' )
jobfile.write( job )
jobfile.close()

imageDirectory = "/home/" + user + "/819/ZWELCH_819/renderer/pics/" + jobLabel
if os.path.exists(imageDirectory) == False:
	os.mkdir(imageDirectory)



print "Submitting " + pbsfilename + " to the PBS queue"
os.system("qsub " + pbsfilename)
