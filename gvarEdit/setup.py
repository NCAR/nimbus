from PyQt4 import QtGui, QtCore
from generateTheButtons import generateButtons
from quitComment import comment
def setup(self):
      #Create tabs
      self.tabs=QtGui.QTabWidget()
      tab1=QtGui.QWidget()	
      tab2=QtGui.QWidget()
      self.tabs.installEventFilter(self)

      #Create scroll area in tab 1
      self.scrollArea=QtGui.QScrollArea(self)
      self.scrollArea.setWidgetResizable(True)
      self.scrollAreaWidgetContents=QtGui.QWidget(self.scrollArea)
      self.scrollArea.setWidget(self.scrollAreaWidgetContents)
      self.verticalLayout=QtGui.QVBoxLayout(self) 
     
      self.head = QtGui.QLabel("   SIGNAL NAME \t \t \t INCLUDE IN GROUND SQL", self)
      self.verticalLayout.addWidget(self.head)
      self.verticalLayout.addWidget(self.scrollArea)
      self.verticalLayoutScroll=QtGui.QVBoxLayout(self.scrollAreaWidgetContents)
      searchLabel = QtGui.QLabel("Search", self)
      self.verticalLayout.addWidget(searchLabel)
      self.searchText = QtGui.QLineEdit()
      self.searchText.textChanged.connect(lambda:generateButtons(self))
      searchLabel.setBuddy(self.searchText)
      self.verticalLayout.addWidget(self.searchText)

      #Create scroll area in tab 2
      self.scrollArea2=QtGui.QScrollArea(self)
      self.scrollArea2.setWidgetResizable(True)
      self.scrollArea2WidgetContents=QtGui.QWidget(self.scrollArea2)
      self.scrollArea2.setWidget(self.scrollArea2WidgetContents)
      self.verticalLayout2=QtGui.QVBoxLayout(tab2) #May wish to update verticalLayout as well
      self.head2 = QtGui.QLabel("   SIGNAL NAME \t \t \t \t \t INCLUDE IN GROUND SQL", self)

      self.verticalLayout2.addWidget(self.head2)
      self.verticalLayout2.addWidget(self.scrollArea2)
      self.verticalLayout2Scroll=QtGui.QVBoxLayout(self.scrollArea2WidgetContents)
      searchLabel2 = QtGui.QLabel("Search", self)
      self.verticalLayout2.addWidget(searchLabel2)
      self.searchText2 = QtGui.QLineEdit()
      self.searchText2.textChanged.connect(lambda:generateButtons(self))
      searchLabel2.setBuddy(self.searchText2)
      self.verticalLayout2.addWidget(self.searchText2)
      
      #Create checkbox filters in tab 1
      checkGroup=QtGui.QGroupBox('')
      checkLays=QtGui.QHBoxLayout()      
      checkLabel = QtGui.QLabel("Display signals with property: ", self)
      self.cbyes = QtGui.QCheckBox('Yes', self)
      self.cbno = QtGui.QCheckBox('No', self)
      self.cbyes.click()
      self.cbno.click()
      self.cbyes.clicked.connect(lambda:generateButtons(self))
      self.cbno.clicked.connect(lambda:generateButtons(self))
      checkLays.addWidget(checkLabel)
      checkLays.addWidget(self.cbyes)
      checkLays.addWidget(self.cbno)
      checkGroup.setLayout(checkLays)
      self.verticalLayout.addWidget(checkGroup)

      #Create checkbox filters in tab 2
      checkGroup2=QtGui.QGroupBox('')
      checkLays2=QtGui.QHBoxLayout()      
      checkLabel2 = QtGui.QLabel("Display signals with property: ", self)
      self.cbyes2 = QtGui.QCheckBox('Yes', tab2)
      self.cbno2 = QtGui.QCheckBox('No', tab2)
      self.cbyes2.click()
      self.cbno2.click()
      self.cbyes2.clicked.connect(lambda:generateButtons(self))
      self.cbno2.clicked.connect(lambda:generateButtons(self))
      checkLays2.addWidget(checkLabel2)
      checkLays2.addWidget(self.cbyes2)
      checkLays2.addWidget(self.cbno2)
      checkGroup2.setLayout(checkLays2)
      self.verticalLayout2.addWidget(checkGroup2)

      tab1.setLayout(self.verticalLayout)
      tab2.setLayout(self.verticalLayout2)
      self.tabs.addTab(tab1,"Add from SQL")
      self.tabs.addTab(tab2,"Add from DB comments")
      self.tabs.show()
      self.tabs.resize(800, 700)
      self.tabs.move(200,50)

      quitLays=QtGui.QVBoxLayout()
      self.quitMessage=QtGui.QLabel('Please comment on modifications',self)
      self.quitreply=QtGui.QLineEdit()
      self.quitOK=QtGui.QPushButton('OK')
      self.quitOK.clicked.connect(lambda:comment(self.quitreply.text()))
      quitLays.addWidget(self.quitMessage)
      quitLays.addWidget(self.quitreply)
      quitLays.addWidget(self.quitOK)
      self.setLayout(quitLays)
      self.move(400,400)

