#!/usr/bin/python


import os
import sys
import math


executable = "/home/zwelch/819/ZWELCH_819/renderer/"




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


frame = int( CmdLineFind( "-frame", 0 ) )
nbCores =  int(CmdLineFind( "-nbpatches", 1 ))
jobLabel = str( CmdLineFind( "-job", "MyJob" ) )
walltime = int( CmdLineFind("-walltime", 20) )
execName = str( CmdLineFind( "-execName", "FAILZ" ) ) 

executable = executable + execName + " "

job = executable
extras = int( CmdLineFindIndex("-extras") )
if extras > 0:
	job = job + " -extras "
	for task in sys.argv[extras+1:]:
		job = job + str(task) + " "
print job

os.system(job)


