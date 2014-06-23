#Julian Quick
#Input:
#      strn: signal character string, 
#      booladd: logical variable denoting whether to add or remove from used signals
#          comments a signal out if above ==== in groundvars, removes otherwise
#      self- used to pass information
#Result:
#      this program edits groundvars to either: uncomment a signal, comment a signal, 
#      append a signal, or remove an appended signal in ground vars
#NOTE: Currently refferences ground SQL, must update
from getGroundInfo import getInfo
from PyQt4 import QtGui, QtCore
import sys
import fileinput
def ads(strn,booladd,self):
   global selfglobal
   selfglobal=self
   from generateTheButtons import generateButtons
   filename='groundvars'
   sqlnames,groundlines=getInfo()

#--------------------------------------------------------------
   if booladd==True: #adding to readable groundvar signals
     pastPoint=False
     wroteLine=False

     # if commented variable exists, uncomment
     for line in fileinput.input(filename,inplace=1):
         if line[1:].startswith(strn) and line.startswith('#') and pastPoint==False:
             line=line.replace(line,line[1:])
             wroteLine=True
         if line.startswith('#============='):
             pastPoint=True
         sys.stdout.write(line)
     fileinput.close()

     #if not, create at bottom
     if wroteLine==False:
        groundDB=open(filename,'a')
        groundDB.write('\n'+strn)
        groundDB.close()

#--------------------------------------------------------------
   if booladd==False:#make signal unreadable in groundvars
      pastPoint=False
      wroteLine=False
      for line in fileinput.input(filename,inplace=1):

         #If signal is above ====, comment
         if line.startswith(strn) and pastPoint==False:
             line=line.replace(line,'#'+line)
             wroteLine=True
         if line.startswith('#============='):
             pastPoint=True
         #If signal is below =====, delete
         if not((wroteLine==False and line.startswith(strn)) and pastPoint==True):
             sys.stdout.write(line)
      fileinput.close()
   generateButtons(self)
