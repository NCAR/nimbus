from PyQt4 import QtGui, QtCore
import subprocess
import re
#Check for dsm_server process
def dsmCheck(self):
    dsmruns=False
    s = subprocess.Popen(["ps", "axw"],stdout=subprocess.PIPE)
    for x in s.stdout:
        if re.search('dsm_server', x):
             dsmruns=True 
    if dsmruns==True:
       reply = QtGui.QMessageBox.warning(self.tabs,'Warning', "dsm_server is already running, and these changes can corrupt data transfer.", QtGui.QMessageBox.Cancel | QtGui.QMessageBox.Ignore)
       if reply==QtGui.QMessageBox.Cancel:
          quit()
