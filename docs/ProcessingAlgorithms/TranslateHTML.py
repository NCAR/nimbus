#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Translate LyX output of xhtml to make these changes:
     1. Change figure references to be relative to sites/default/files/
     2. Translate some HTML codes not recognized in Drupal7 to hex:
         &af; -> &#8289
         &epsi -> &#949
         &cir; -> &#176 or &#759
         """
import sys
import re
import urllib
import copy
if len (sys.argv) <= 1:
    print "Please supply name of file to translate:$"
    inputFile = raw_input()
else:
    inputFile = sys.argv[1]
try:
    IFile = open(inputFile, 'r')
    for line in IFile:
        line = line.replace('\n', '')
        #if r"<img.*src=\'" in line:
        if re.search(r"<img.*src=", line):
            line = re.sub (r'src=\'.*[/_]([A-Za-z0-9\.]*)\' alt', \
                   r"src='sites/default/files/\1' alt", line)
        if re.search (r'\&af;', line):
            line = re.sub (r'\&af;', r'&#8289', line)
        if re.search (r'\&epsi;', line):
            line = re.sub (r'\&epsi', r'&#949', line)
        if re.search (r'\&cir;', line):
            line = re.sub (r'\&cir', r'&deg', line)
        if re.search (r'\&sime;', line):
            line = re.sub (r'\&sime', r'&#8776', line)
        if re.search (r'\&ap;', line):
            line = re.sub (r'\&ap', r'&#8776', line)
        if re.search (r'\&rhov;', line):
            line = re.sub (r'\&rhov', r'&rho', line)
        print line

        
except IOError as e:
    print 'There is no file with the specified name; exiting.\n '\
          + 'ABORT'
    

