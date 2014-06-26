#Julian Quick
#Input:
#      strn: signal character string, 
#      booladd: logical variable denoting whether to add or remove from used signals
#          comments a signal out if above ==== in groundvars, removes otherwise
#      self- used to pass information
#Result:
#      this program edits groundvars to either: uncomment a signal, comment a signal, 
#      append a signal, or remove an appended signal in ground vars
from getGroundInfo import getInfo
from PyQt4 import QtGui, QtCore
import sys
import fileinput
import os
def ads(strn,booladd,self):
   global selfglobal
   selfglobal=self
   from generateTheButtons import generateButtons
   sqlnames,groundlines=getInfo(self)

#--------------------------------------------------------------
#adding to readable groundvar signals
   if booladd==True: 
     pastPoint=False
     wroteLine=False
     print 'adding '+strn+'.'

     # if commented variable exists, uncomment
     for line in fileinput.input(os.path.expandvars(self.gvfilename),inplace=1):
         if line[1:].startswith(strn) and line.startswith('#') and pastPoint==False:
             line=line[1:]
             wroteLine=True
         if pastPoint==True and strn in line:
             fileinput.close()
             return
         if line.startswith('#============='):
             pastPoint=True
         sys.stdout.write(line)
     fileinput.close()

     #if not, create at bottom
     if wroteLine==False:
        groundDB=open(os.path.expandvars(self.gvfilename),'a')
        groundDB.write('\n'+strn+'\n')
        groundDB.close()

#--------------------------------------------------------------
#make signal unreadable in groundvars
   if booladd==False:
      pastPoint=False
      wroteLine=False
      print 'removing '+strn+'.'
      stats='failed'
      for line in fileinput.input(os.path.expandvars(self.gvfilename),inplace=1):

         #If signal is above ====, comment
         if line.startswith(strn) and pastPoint==False:
             line=line.replace(line,'#'+line+'\n')
             wroteLine=True
             stats='successful'
         if line.startswith('#============='):
             pastPoint=True
         #If signal is below =====, delete
         if pastPoint==True and line.strip()==strn and wroteLine==False:
             stats='successful'
         elif not line.strip():
             action=0 #arbitrary action
         else:
            sys.stdout.write(line)
      fileinput.close()
      print 'remove '+stats
   generateButtons(self)
