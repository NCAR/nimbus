#Julian Quick
#This process scans groundvars for signals not in the aircraft database of live signals collected.
#A signal in groundvars but not the aircraft SQL is considered inconsistant because this can lead to data corruption.
#If inconsistant signal(s) found, warns user and offers to comment signals out. Option to hide this warning in session future.
from getGroundInfo import getInfo
from PyQt4 import QtGui, QtCore
import fileinput
import os
import sys
#-----------------------------------
#User ignores this warning

def ignore(self):
   self.dataWarning.hide()

#----------------------------------
#Comment out inconsistant signals

def fixprob(self,badStuff):
      import generateTheButtons
      i=0
      while i<len(badStuff):
         for line in fileinput.input(os.path.expandvars(self.gvfilename),inplace=1):
            if line==badStuff[i]+'\n':
               line='#'+line
            sys.stdout.write(line)
         fileinput.close()
         i+=1
      print 'commented ',badStuff
      self.dataWarning.hide()
      generateTheButtons.generateButtons(self)

#-----------------------------------------------------------
#Initial Process

def qualityCheck(self):
   badStuff=[]
   self.dataComment.close()
   self.dataComment=QtGui.QPushButton('Comment Signal(s)')
   self.dataHBox.addWidget(self.dataComment)
   self.dataComment.clicked.connect(lambda:fixprob(self,badStuff))
   self.dataIgnore.clicked.connect(lambda:ignore(self))
   sqlnames,groundlines=getInfo(self)
   print 'probe'
   groundlines=filter(None,groundlines)
   i=0

   #Look for groundvars signals in aircraft sql database  
   while i<len(groundlines):
        if groundlines[i][0]!='#' and groundlines[i] not in sqlnames and groundlines[i].strip():
                badStuff.append(groundlines[i])
        i+=1

   #Display warnings if inconsistancy found
   if len(badStuff)>1:
     self.dataWarning.show()
     self.dataMessage.setText('Signals '+str(badStuff[:-1])+", and ['"+str(badStuff[-1])+"'] are in groundvars and are to be sent to ground, but cannot be found in aircraft database. Comment these signals?")
   if len(badStuff)==1:
     self.dataWarning.show()
     self.dataMessage.setText("Signal ['"+str(badStuff[0])+"'] is in groundvars and is to be sent to ground, but cannot be found in aircraft database. Comment this signal?")
