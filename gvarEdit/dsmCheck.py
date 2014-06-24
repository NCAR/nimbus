import commands
from PyQt4 import QtGui, QtCore
#Check for dsm_server process
def dsmCheck(self):
    if 'dsm_server' in commands.getoutput('ps -A'):
       reply = QtGui.QMessageBox.warning(self.tabs,'Warning', "dsm_server is already running, and these changes can corrupt data transfer.", QtGui.QMessageBox.Cancel | QtGui.QMessageBox.Ignore)
