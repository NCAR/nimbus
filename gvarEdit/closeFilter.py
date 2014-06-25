from PyQt4 import QtGui, QtCore
def eventFilter(self, qobject, qevent):
    qtype = qevent.type()
    if qtype == QtCore.QEvent.Close:
        reply = QtGui.QMessageBox.question(self.tabs, 'Message', "Are you sure to quit?", QtGui.QMessageBox.Yes | QtGui.QMessageBox.No, QtGui.QMessageBox.No)
        if reply==QtGui.QMessageBox.Yes:
            self.show()
        return True

    # parents event handler for all other events
    return super(GUI,self).eventFilter(qobject, qevent)
