#Julian Quick
#This program provides a GUI to edit groundvars based on SQL information or 
#commented signals in groundvars
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
            reply = QtGui.QMessageBox.question(self.tabs, 'Message', "Are you sure to quit?", QtGui.QMessageBox.Yes | QtGui.QMessageBox.No, QtGui.QMessageBox.No)
            if reply==QtGui.QMessageBox.Yes:
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

