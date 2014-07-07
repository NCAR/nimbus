#Julian Quick
#This function comfirms the user desires to remove a variable, and calls the remove function
import getInfo
import remove
from PyQt4 import QtGui, QtCore
def delete(signame,self,num):
   entries=getInfo.getinfo()
   quit_messge='Are you sure you want to delete '+signame
   reply=QtGui.QMessageBox.question(self, 'Warning: altering varDB', quit_messge, QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
   if reply == QtGui.QMessageBox.Yes:
      from addSignal import addsignal
      from radioClickEvent import lookingAt
      from radioClickEvent import clearRightInfoHub
      clearRightInfoHub()
      #lookingAt(-1,self)
      addsignal(signame,self,num,{'action':'delete'})
