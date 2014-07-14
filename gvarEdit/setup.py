#Creates window with two tabs each woth a scroll area.
#Tab 1 shows signals found in the real time aircraft database, and allows the user to activate them in groundvars
#tab 2 shows all potential signals found in groundvars (comented or not) and allows the user to activate them
#Headers describe the scroll area contents to the user. The search box allows the user to filter signals.
#'checkbox filters' allow the user toggle viewing signals marked 'yes' or 'no'
from PyQt4 import QtGui, QtCore
from generateTheButtons import generateButtons
from quitComment import comment
def setup(self):
      #groundvars location
      self.gvfilename='/$PROJ_DIR/$PROJECT/$AIRCRAFT/groundvars'

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
     
      #Creates header and search box in tab 1
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
      self.head2 = QtGui.QLabel("   SIGNAL NAME \t \t \t \t \t     INCLUDE IN GROUND SQL", self)

      #Creates header and search box in tab 2
      self.verticalLayout2.addWidget(self.head2)
      self.verticalLayout2.addWidget(self.scrollArea2)
      self.verticalLayout2Scroll=QtGui.QVBoxLayout(self.scrollArea2WidgetContents)
      searchLabel2 = QtGui.QLabel("Search", self)
      self.verticalLayout2.addWidget(searchLabel2)
      self.searchText2 = QtGui.QLineEdit()
      self.searchText2.textChanged.connect(lambda:generateButtons(self))
      searchLabel2.setBuddy(self.searchText2)
      self.verticalLayout2.addWidget(self.searchText2)
      
      #Create checkbox filters and quit button in tab 1
      checkGroup=QtGui.QGroupBox('')
      checkLays=QtGui.QHBoxLayout()      
      checkLabel = QtGui.QLabel("Display signals with property: ", self)
      self.cbyes = QtGui.QCheckBox('Yes', self)
      self.cbno = QtGui.QCheckBox('No', self)
      self.quitButton=QtGui.QPushButton('Quit')
      self.cbyes.click()
      self.cbno.click()
      self.cbyes.clicked.connect(lambda:generateButtons(self))
      self.cbno.clicked.connect(lambda:generateButtons(self))
      self.quitButton.clicked.connect(lambda:self.tabs.close())
      checkLays.addWidget(checkLabel)
      checkLays.addWidget(self.cbyes)
      checkLays.addWidget(self.cbno)
      checkLays.addWidget(self.quitButton)
      checkGroup.setLayout(checkLays)
      self.verticalLayout.addWidget(checkGroup)

      #Create checkbox filters and quit button in tab 2
      checkGroup2=QtGui.QGroupBox('')
      checkLays2=QtGui.QHBoxLayout()      
      checkLabel2 = QtGui.QLabel("Display signals with property: ", self)
      self.cbyes2 = QtGui.QCheckBox('Yes', tab2)
      self.cbno2 = QtGui.QCheckBox('No', tab2)
      self.quitButton2=QtGui.QPushButton('Quit')
      self.cbyes2.click()
      self.cbno2.click()
      self.cbyes2.clicked.connect(lambda:generateButtons(self))
      self.cbno2.clicked.connect(lambda:generateButtons(self))
      self.quitButton2.clicked.connect(lambda:self.tabs.close())
      checkLays2.addWidget(checkLabel2)
      checkLays2.addWidget(self.cbyes2)
      checkLays2.addWidget(self.cbno2)
      checkLays2.addWidget(self.quitButton2)
      checkGroup2.setLayout(checkLays2)
      self.verticalLayout2.addWidget(checkGroup2)

      tab1.setLayout(self.verticalLayout)
      tab2.setLayout(self.verticalLayout2)
      self.tabs.addTab(tab1,"Database")
      self.tabs.addTab(tab2,"Groundvars")
      self.tabs.show()
      self.tabs.resize(800, 700)
      self.tabs.move(200,50)

      #Popup which appears after closing window
      quitLays=QtGui.QVBoxLayout()
      self.quitMessage=QtGui.QLabel('Please comment on modifications',self)
      self.quitreply=QtGui.QLineEdit()
      self.quitOK=QtGui.QPushButton('OK')
      self.quitOK.clicked.connect(lambda:comment(self,self.quitreply.text()))
      quitLays.addWidget(self.quitMessage)
      quitLays.addWidget(self.quitreply)
      quitLays.addWidget(self.quitOK)
      self.setLayout(quitLays)
      self.move(400,400)

      #Popup which appears when data potential corruption is found
      #self.dataDisplayCheck adds the option to not show this message in the future
      #dataHBox and dataComment are objects of self so that dataComment can be deleted and initialized again,
      #clearing it's previous signals
      #See dataQualityCheck.py for process details
      self.dataWarning=QtGui.QWidget()
      self.dataWarning.resize(500,200)
      self.move(400,400)
      self.setWindowTitle('Potential Data Corruption')
      self.dataComment=QtGui.QPushButton('Comment Signal(s)')
      self.dataIgnore=QtGui.QPushButton('Ignore Warning')
      self.dataHBox=QtGui.QHBoxLayout()
      self.dataHBox.addWidget(self.dataComment)
      self.dataHBox.addWidget(self.dataIgnore)
      dataVBox=QtGui.QVBoxLayout()
      self.dataMessage=QtGui.QLabel('No one should be seeing this text. Check setup #software.',self)
      self.dataMessage.setWordWrap(True)
      dataVBox.addWidget(self.dataMessage)
      self.dataDisplayCheck=QtGui.QCheckBox('Do not show this message again')
      self.dataDisplayCheck.setChecked(False)
      dataVBox.addWidget(self.dataDisplayCheck)
      dataVBox.addLayout(self.dataHBox)
      self.dataWarning.setLayout(dataVBox)
