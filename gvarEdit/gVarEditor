#!/usr/bin/python
#Julian Quick
#This program provides a GUI to edit groundvars based on aircraft database information or 
#commented signals in groundvars. Shows the user two tabs.
#Tab 1 shows signals found in the air SQL, and allows the user to activate them in groundvars
#tab 2 shows all potential signals found in groundvars (comented or not) and allows the user to activate them
#Expects groundvars to have a line with value starting with '#=================', to seperate permanant signals
#from temporary signals. A new signal is either uncommented above this line or appended below this line.
#If dsm_server is already running when this program starts, a warning appears

#WARNING: hercules.raf-ext.ucar.edu real-time MUST be active for program to function
import sys
import os
from getGroundInfo import getInfo
from PyQt4 import QtGui, QtCore
from setup import setup
from generateTheButtons import generateButtons
from dsmCheck import dsmCheck
class GUI(QtGui.QWidget):
    def __init__(self):
        super(GUI, self).__init__()
        self.initUI()

    def initUI(self):
        setup(self)
        dsmCheck(self)
        generateButtons(self)

    #Custom filter for window close event
    def eventFilter(self, qobject, qevent):
        qtype = qevent.type()
        if qtype == QtCore.QEvent.Close:
            self.show()
            return True

        # parents event handler for all other events
        return super(GUI,self).eventFilter(qobject, qevent)
def main():
    app = QtGui.QApplication(sys.argv)
    ex = GUI()
    app.processEvents()
    sys.exit(app.exec_())
if __name__ == '__main__':
    main()

