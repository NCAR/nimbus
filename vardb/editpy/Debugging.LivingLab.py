#GUI TO view and edit varDB
import sys
from PyQt4 import QtGui, QtCore

class Example(QtGui.QWidget):
    
    def __init__(self):
        super(Example, self).__init__()
        
        self.initUI()
 #   entries=[]    
    def initUI(self):      
        print 'self is ',self
        def mkbut(num,name):
              button=QtGui.QRadioButton(name,left)
              left.verticalLayoutScroll.addWidget(button)
              button.clicked.connect(lambda: lookingAt(num))
        def lookingAt(num): #displays metadata based on radiobutton selection
           entries=getinfo()
           named=entries[num][0]
           theunits=entries[num][1]
           description=entries[num][2]
           right.descripts.setText("Description: "+description)
           right.descripts.setMinimumSize(right.descripts.sizeHint())
           right.units.setText("Units: "+theunits)
           right.units.setMinimumSize(right.units.sizeHint())
           try: deleteButton.clicked.disconnect()
           except Exception: pass
           deleteButton.clicked.connect(lambda: delete(num)) #lambda does nothing
           editButton.clicked.connect(lambda: edit(num)) #lambda does nothing
        def delete(num):
           entries=getinfo()
           quit_messge='Are you sure you want to delete '+entries[num][0]
           reply=QtGui.QMessageBox.question(self, 'Warning: altering varDB', quit_messge, QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
           if reply == QtGui.QMessageBox.Yes:
              rem(num)
        def rem(num):
              vardbOrig=open("varDB.txt","r")
              lines = [line.strip() for line in vardbOrig]
              entries=[elem.strip().split('  ') for elem in lines]
              print 'got lines'
              vardbOrig.close()   
              varDB2=open("varDB.txt","w")
              for i in reversed(range(left.verticalLayoutScroll.count())): #Remove buttons
                 left.verticalLayoutScroll.itemAt(i).widget().setParent(None) 
              i=0
              while i < len(lines)-1:
                 if i<num:
                    varDB2.write(lines[i]+"\n")
                    mkbut(i,entries[i][0])
                 if i>num: #Prevent data shift
                    varDB2.write(lines[i]+"\n")
                    mkbut(i-1,entries[i][0])
                 i+=1
              varDB2.close()
        def ads():
	  name, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Enter signal name:')
          if ok:
            units, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Enter signal units:')
            if ok:
              description, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Enter signal description:')
              if ok:
                 addsignal(name,units,description)
        def addsignal(name,units,description):
                 varDB=open('varDB.txt','r')
                 lines = [line.strip() for line in varDB]
                 entries=[elem.strip().split('  ') for elem in lines]
                 varDB.close()
                 for i in reversed(range(left.verticalLayoutScroll.count())): #Remove buttons
                     left.verticalLayoutScroll.itemAt(i).widget().setParent(None) 
                 varDB2=open("varDB.txt","w")
                 i=0
                 logics=True
                 while i<len(entries):
                   if logics==True and entries[i][0][0].upper()>str(name[0]).upper():
                       varDB2.write(name+'  '+units+'  '+description+"\n")
                       mkbut(i,str(name))
                       logics=False
                       i+=1
                       print "making button ", str(name)
                   if logics==True:
                       mkbut(i,entries[i][0])
                       varDB2.write(lines[i]+'\n')
                   if logics==False:
                       mkbut(i,entries[i-1][0])
                       varDB2.write(lines[i]+'\n')
                   if i==len(entries)-1 and logics==True:
                       varDB2.write(name+'  '+units+'  '+description+"\n")
                       mkbut(i,name)
                       varDB2.close()
                       return None
                   i+=1
                 print "addition succesful"+name+units+description 
                 varDB2.close()
                 generateButtons()
        def edit(num):
	  name, ok = QtGui.QInputDialog.getText(self, 'Edit Signal', 'Enter signal name:')
          if ok:
	    units, ok = QtGui.QInputDialog.getText(self, 'Edit Signal', 'Enter signal units:')
            if ok:
              description, ok = QtGui.QInputDialog.getText(self, 'Edit Signal', 'Enter signal description:')
              if ok:
                 rem(num)
                 addsignal(name,units,description)
                 return None
        def getinfo():
           varDB=open('varDB.txt','r')
           lines = [line.strip() for line in varDB]
           entries=[elem.strip().split('  ') for elem in lines]
           i=0
           while i<len(entries): #Removes clutter
              entries[i]=filter(len,entries[i])
              i+=1
           varDB.close()
           return entries
        def generateButtons():
           varDB=open('varDB.txt','r')
           lines = [line.strip() for line in varDB]
           entries=[elem.strip().split('  ') for elem in lines]
           i=0
           while i<len(entries): #Removes clutter
              entries[i]=filter(len,entries[i])
              #create radio buttons
              mkbut(i,entries[i][0])
              i+=1
           varDB.close()  
        #Window Dimensions
        def setup():
           global left
           global right
           global hbox
           self.setGeometry(300, 300, 600, 600)
           self.setWindowTitle('varDB GUI')
           #Create middle division
           hbox = QtGui.QHBoxLayout(self)
           left = QtGui.QFrame(self)
           left.setFrameShape(QtGui.QFrame.StyledPanel)
           right = QtGui.QFrame(self)
           right.setFrameShape(QtGui.QFrame.StyledPanel)
           splitter1 = QtGui.QSplitter(QtCore.Qt.Horizontal)
           splitter1.addWidget(left)
           splitter1.addWidget(right)
 #          splitter1.setSizes([150,300])
           hbox.addWidget(splitter1)
           self.setLayout(hbox)
           QtGui.QApplication.setStyle(QtGui.QStyleFactory.create('Cleanlooks'))
           #Create scroll area
           left.scrollArea=QtGui.QScrollArea(left)
           left.scrollArea.setWidgetResizable(True)
           left.scrollAreaWidgetContents=QtGui.QWidget(left.scrollArea)
           left.scrollArea.setWidget(left.scrollAreaWidgetContents)
           left.verticalLayout=QtGui.QVBoxLayout(left)
           left.verticalLayout.addWidget(left.scrollArea)
           left.verticalLayoutScroll=QtGui.QVBoxLayout(left.scrollAreaWidgetContents)
        setup()
        print 'left isss ' ,left
        #Get info from varDB
        generateButtons()
        #Create Lables
        right.descripts=QtGui.QLabel("Hello Sir",right)
        right.descripts.move(20,20)
        right.units=QtGui.QLabel(right)
        right.units.move(20,60) 
        #Create option buttons
        hlox=QtGui.QHBoxLayout()
        hbox.addStretch(1)
        addButton=QtGui.QPushButton("Add Variable(s)")
        addButton.clicked.connect(lambda: ads())
        editButton=QtGui.QPushButton("Edit Selection")
        deleteButton=QtGui.QPushButton("Delete Selection")
        quitButton=QtGui.QPushButton("Quit")
        quitButton.clicked.connect(QtCore.QCoreApplication.instance().quit)
        hlox.addWidget(addButton)
        hlox.addWidget(editButton)
        hlox.addWidget(deleteButton)
        hlox.addWidget(quitButton)
        vbox=QtGui.QVBoxLayout()
        vbox.addStretch(1)
 	vbox.addLayout(hlox)
        right.setLayout(vbox)
app = QtGui.QApplication(sys.argv)
ex = Example()
ex.show()
app.processEvents()
sys.exit(app.exec_())
