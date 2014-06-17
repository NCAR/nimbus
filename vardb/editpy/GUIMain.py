#Julian Quick
#GUI TO view and edit varDB
#Several external functions are refferenced. See GUI.pdf for flowchart.
import sys
from PyQt4 import QtGui, QtCore
from newSignalInput import ads
from generateButtons import generateButtons
from setup import setup
class GUI(QtGui.QWidget):
    def __init__(self):
        super(GUI, self).__init__()
        self.initUI()
    def initUI(self):      
        self.setGeometry(300, 300, 800, 600)
        self.setWindowTitle('varDB GUI')
        #Create option buttons
        addButton=QtGui.QPushButton("Add Variable")
        addButton.clicked.connect(lambda: ads(self,left,right))
        self.saveButton=QtGui.QPushButton("Save Changes")
        self.deleteButton=QtGui.QPushButton("Delete Selection")
        quitButton=QtGui.QPushButton("Quit")
        quitButton.clicked.connect(QtCore.QCoreApplication.instance().quit)
        #Creates left and right sections, search bar, scroll area
        hbox = QtGui.QHBoxLayout(self)
        left = QtGui.QFrame(self)
        right = QtGui.QFrame(self)
        right=setup(hbox,left,right,self)
        self.searchText.textEdited.connect(lambda: generateButtons(left,right,self,str(self.searchText.text()),0))
        self.setLayout(hbox)
        #Populates scroll area,selects 0th radio button
        generateButtons(left,right,self,'',0)
        #Option buttons layout
        hlox=QtGui.QHBoxLayout()
        hlox.addStretch(1)
        hlox.addWidget(addButton,1)
        hlox.addWidget(self.saveButton,1)
        hlox.addWidget(self.deleteButton,1)
        hlox.addWidget(quitButton,1)
        hlox.addStretch()
        vbox=QtGui.QVBoxLayout()
        vbox.addStretch(1)
 	vbox.addLayout(hlox)
        right.setLayout(vbox)

def main():
    app = QtGui.QApplication(sys.argv)
    ex = GUI()
    ex.show()
    app.processEvents()
    sys.exit(app.exec_())
if __name__ == '__main__':
    main()    
