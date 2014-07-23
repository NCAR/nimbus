#Julian Quick
#GUI TO view and edit varDB
#Several external functions are refferenced. See GUI.pdf for flowchart.
import sys
from PyQt4 import QtGui, QtCore
from newSignal import newSig
from generateButtons import generateButtons
from setup import setup

class GUI(QtGui.QWidget):
    def __init__(self):
        super(GUI, self).__init__()
        self.initUI()
    def initUI(self):      
        self.setGeometry(500, 300, 800, 600)
        self.setWindowTitle('varDB GUI')

        #Create option buttons
        addButton=QtGui.QPushButton("Add Variable")
        addButton.pressed.connect(lambda: newSig(self))
        self.saveButton=QtGui.QPushButton("Save Changes")
        self.deleteButton=QtGui.QPushButton("Delete Selected variable")
        quitButton=QtGui.QPushButton("Quit")
        quitButton.clicked.connect(QtCore.QCoreApplication.instance().quit)

        #Creates left and right sections, search bar, scroll area
        hbox = QtGui.QHBoxLayout(self)
        self.left = QtGui.QFrame(self)
        self.upright = QtGui.QFrame(self)
        self.downright = QtGui.QFrame(self)
        self=setup(hbox,self,"VDB.xml")
        self.searchText.textEdited.connect(lambda: generateButtons(self,str(self.searchText.text()),0))
        self.setLayout(hbox)

        #Populates scroll area
        generateButtons(self,'',-1)

        #Option buttons layout
        hlox=QtGui.QHBoxLayout()
        hlox2=QtGui.QHBoxLayout()
        hlox.addStretch(1)
        hlox.addWidget(addButton,1)
        hlox.addWidget(self.saveButton,1)
        hlox.addWidget(self.deleteButton,1)
        hlox.addWidget(quitButton,1)
        hlox.addStretch()
        vbox=QtGui.QVBoxLayout()
        vbox.addStretch(1)
 	vbox.addLayout(hlox)
 	vbox.addLayout(hlox2)
        self.downright.setLayout(vbox)

app = QtGui.QApplication(sys.argv)
ex = GUI()
ex.show()
app.processEvents()
exit(app.exec_())
