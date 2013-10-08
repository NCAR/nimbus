#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Read a NetCDF data file, specified on the command
line or else via user interaction, and read the header.
Create a table with these entries:
    1. variable name
    2. long name as in header file
    3. project name as in file name
The plan is to be able to concatenate and
alphabetize these to construct a complete list of
variables.

Created on Wed Aug  8 15:12:21 2012

@author: cooperw
"""
import sys
import os
import re

os.system ('rm VARSunsorted VARSsorted EVARsorted')
UnsortedFile = open('VARSunsorted', 'w')
""" this is for single-file processing; replaced by set of files
if len (sys.argv) <= 1:
    print "Please supply name of a NetCDF file:$"
    inputFile = raw_input()
else:
    inputFile = sys.argv[1]
os.system ('rm HeaderDump')
scommand = 'ncdump -h /scr/raf/Prod_Data/'+inputFile \
            +' > HeaderDump'
os.system(scommand)
"""
FileList = ["DYCOMS-II/181rf02.nc",\
    "EPIC2001/161new02.nc",\
    "GOTEX/150rf02.nc",\
    "HEFT08/HEFT08rf02.nc",\
    "HEFT10/HEFT10rf01.nc",\
    "HIPPO-1/HIPPO-1rf02.nc",\
    "HIPPO-5/HIPPO-5rf02.nc",\
    "ICEBRIDGE/ICEBRIDGErf02.nc",\
    "ICE-L/ICE-Lrf02.nc",\
    "ICE-T/ICE-Trf02.nc",\
    "IDEAS-4/IDEAS-4rf02.nc",\
    "MAP/803Lrf25.nc",\
    "PACDEX/PACDEXrf02.nc",\
    "PASE/PASErf02.nc",\
    "PLOWS/PLOWSrf02.nc",\
    "PREDICT/PREDICTrf02.nc",\
    "ProgSci/ProgScirf02.nc",\
    "RICO/135rf02.nc",\
    "START08/START08rf02.nc",\
    "TORERO/TOREROrf02.nc",\
    "TREX/TREXrf02.nc",\
    "VOCALS/VOCALSrf02.nc",\
    "WAMO/WAMOrf02.nc"]

for inputFile in FileList:
    os.system ('rm HeaderDump')
    scommand = 'ncdump -h /scr/raf/Prod_Data/'+inputFile \
            +' > HeaderDump'
    os.system(scommand)
    # decode the project name
    pname = re.sub (r'/.*', r'', inputFile)
    # print 'Project name is ', pname
    enable = -1
    try:
        IFile = open('HeaderDump', 'r')
        for line in IFile:
            if re.search ('float ', line) or re.search ('int ', line):
                enable = 1
                vname = re.sub (r'.*float ', r'', line)
                vname = re.sub ('.*int ', r'', vname)
                vname = re.sub (r' ;', r'', vname)
                vname = re.sub (r'\(.*', r'', vname)
                vname = re.sub (r'\n', r'', vname)
			# truncate position-indication suffixes:
                vname = re.sub (r'_RW.*', r'', vname)
                vname = re.sub (r'_RP.*', r'', vname)
                vname = re.sub (r'_LW.*', r'', vname)
                vname = re.sub (r'_LP.*', r'', vname)
                vname = re.sub (r'_RM.*', r'', vname)
                vname = re.sub (r'_LM.*', r'', vname)
                vname = re.sub (r'_IB.*', r'', vname)
                vname = re.sub (r'_IRS', r'', vname)
                vname = re.sub (r'_I2', r'', vname)
                vname = re.sub (r'_G2', r'', vname)
                vname = re.sub (r'_G', r'', vname)
                vname = re.sub (r'_ROI', r'', vname)
                vname = re.sub (r'_OPC', r'', vname)
                vname = re.sub (r'A2DTEMP_.*', r'A2DTEMP', vname)
    
            elif re.search ('long_name', line):
                enable = 2
                lname = re.sub (r'\" ;$', r'', line)
                lname = re.sub (r'.*"', r'', lname)
                lname = re.sub (r'\n', r'', lname)
                UnsortedFile.write(vname+';'+lname+';'+pname+'\n')
    except IOError, e:
        print 'There is no file with the specified name; exiting.\n '\
              + 'ABORT'
        
UnsortedFile.close()
os.system('sort <VARSunsorted >VARSsorted')
VList = open ('VARSsorted', 'r')
EList = open ('EVARsorted', 'w')
LastName = 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
ProjectList = []
nP = 4
lines = 0
for line in VList:
    Name,LongName,Project = line.split(';')
    Project = re.sub (r'\n', r'', Project)
    #print Name,LongName,Project
    if Name != LastName:
        if not re.search ('XXXXXXXXXXXXXXX', LastName):
            if lines > nP:
                oline = LastName + ' (' + LastLongName + ')' + '\n'
                EList.write (oline)
                #print 'G ', oline
            else:
                oline = LastName + ' (' + LastLongName + ')' + '['
                for P in ProjectList:
                    oline += ' ' + P
                oline += ' ]\n'
                EList.write (oline)
                #print 'S ', oline
        LastName = Name
        LastLongName = LongName
        lines = 0
        ProjectList = []
    lines += 1
    if lines <= nP:
        ProjectList.append (Project)
EList.close ()

