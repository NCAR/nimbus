#Julian Quick
#This function allows the user to input signal information to the 
#addsignal function, or to cancel at any time
from PyQt4 import QtGui, QtCore
from addSignal import addsignal
def ads(self):
  name, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Enter signal name:')
  if ok:
    units, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Enter signal units:')
    if ok:
      description, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Enter signal description:')
      if ok:
         addsignal([name,units,description],self,0)

