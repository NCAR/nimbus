#Julian QUick

import sys
from PyQt4 import QtGui, QtCore
import getInfo


def rematt(num,signal,att,friend,self,sure):
    from addSignal import addsignal
    if sure==True:
       print 'signal is ',signal
       addsignal([signal,att],friend,num,{'action':'remove attribute'})
    self.hide()

def combo_chosen(self,friend,text,var,sig,adding):
    from addSignal import addsignal
    if adding==True:
       addsignal([sig,var,str(text)],friend,-1,{'action':'new attribute'})
    self.hide()
def BoolComboBox(signal,variable,friend,scenario):
  class ComboBoxBasic(QtGui.QWidget):
    def __init__(self):

        # create GUI
        QtGui.QMainWindow.__init__(self)
        self.setWindowTitle('Information Aquisition')

        # Set the window dimensions
        self.resize(250,50)
        self.setGeometry(500, 300, 200, 60)
 #       self.initGUI()

#    def initGUI(self):
        
        # vertical layout for widgets
        self.vbox = QtGui.QVBoxLayout()
        self.setLayout(self.vbox)
        self.combo = QtGui.QComboBox()
        if scenario==1: #Scenario 1 is from newSignalInput.py
           self.ok=QtGui.QPushButton('OK')
           self.cancel=QtGui.QPushButton('Skip')
           self.combo.addItem('true')
           self.combo.addItem('false')
           self.ok.clicked.connect(lambda: combo_chosen(self,friend,self.combo.currentText(),variable,signal,True))
           self.cancel.clicked.connect(lambda: combo_chosen(self,friend,self.combo.currentText(),variable,signal,False))
           self.labez=QtGui.QLabel('Enter '+variable+' state')
        
        if scenario == 2:#Scenario 2 is from delete attributes button in radioClickEvent.py
           self.labez=QtGui.QLabel('Select attribute to remove from '+variable)
           self.ok=QtGui.QPushButton('Remove')
           self.cancel=QtGui.QPushButton('Cancel')
           self.ok.clicked.connect(lambda: rematt(signal,variable,self.combo.currentText(),friend,self,True))
           self.ok.clicked.connect(lambda: rematt(signal,variable,self.combo.currentText(),friend,self,False))

           #Get attributes
           entries=getInfo.getinfo()
           for entry in entries:
               if entry[1]==variable:
                 for item in entry[0][1:]:
                    self.combo.addItem(item)

        self.hbox=QtGui.QHBoxLayout()
        self.hbox.addWidget(self.ok)
        self.hbox.addWidget(self.cancel)
        
        # Create a combo box and add it to our layout
        self.vbox.addWidget(self.labez)
        self.vbox.addWidget(self.combo)
        self.vbox.addLayout(self.hbox)



        
        

     
  try:friend.gooy.destroy()
  except:pass
  friend.gooy=ComboBoxBasic()
  friend.gooy.show()
  friend.gooy.raise_()
  return True

