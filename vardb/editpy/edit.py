#Julian Quick
#This function allows the user to edit the selected variable
#By inputting all of the information again
from PyQt4 import QtGui, QtCore
import remove
#from addSignal import addsignal
import getInfo
def edit(num,left,right,self,deleteButton,editButton):
          entries=getInfo.getinfo()
          name, ok = QtGui.QInputDialog.getText(self, 'Edit Signal', 'Enter signal name:',QtGui.QLineEdit.Normal,entries[num][0])
          if ok:
            units, ok = QtGui.QInputDialog.getText(self, 'Edit Signal', 'Enter signal units:',QtGui.QLineEdit.Normal,entries[num][1])
            if ok:
              description, ok = QtGui.QInputDialog.getText(self, 'Edit Signal', 'Enter signal description:',QtGui.QLineEdit.Normal,entries[num][2])
              if ok:
                 remove.rem(num,left,right,self,deleteButton,editButton)
                 addsignal(name,units,description,self,left,right,deleteButton,editButton)
                 #return None

