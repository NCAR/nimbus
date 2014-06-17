#Julian Quick
#This function comfirms the user desires to remove a variable, and calls the remove function
import getInfo
import remove
from PyQt4 import QtGui, QtCore
def delete(num,left,right,self):
   entries=getInfo.getinfo()
   quit_messge='Are you sure you want to delete '+entries[num][0]
   reply=QtGui.QMessageBox.question(self, 'Warning: altering varDB', quit_messge, QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
   if reply == QtGui.QMessageBox.Yes:
      from radioClickEvent import lookingAt
     # self.searchText.setText('')
      lookingAt(-1,left,right,self)
      remove.rem(num,left,right,self)

