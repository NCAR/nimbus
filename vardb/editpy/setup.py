#Julian QUick
#Sets up GUI Window, scroll area, and data lables
from PyQt4 import QtGui, QtCore
def setup(hbox,self):

      #Eventually Headers should be read from varDB
      headers=['name','units','description']

      #Create middle division
      splitter1 = QtGui.QSplitter(QtCore.Qt.Horizontal)
      splitter1.addWidget(self.left)
      splitter1.addWidget(self.right)
      splitter1.setStretchFactor(1,1)
      hbox.addWidget(splitter1)
      self.left.setFrameShape(QtGui.QFrame.StyledPanel)
      self.right.setFrameShape(QtGui.QFrame.StyledPanel)
      QtGui.QApplication.setStyle(QtGui.QStyleFactory.create('Cleanlooks'))

      #Create scroll area
      self.left.scrollArea=QtGui.QScrollArea(self.left)
      self.left.scrollArea.setWidgetResizable(True)
      self.left.scrollAreaWidgetContents=QtGui.QWidget(self.left.scrollArea)
      self.left.scrollArea.setWidget(self.left.scrollAreaWidgetContents)
      self.left.verticalLayout=QtGui.QVBoxLayout(self.left)
      self.left.verticalLayout.addWidget(self.left.scrollArea)
      self.left.verticalLayoutScroll=QtGui.QVBoxLayout(self.left.scrollAreaWidgetContents)

      #Create search bar
      searchLabel = QtGui.QLabel(" Search", self)
      self.left.verticalLayout.addWidget(searchLabel)
      self.searchText = QtGui.QLineEdit()
      searchLabel.setBuddy(self.searchText)
      self.left.verticalLayout.addWidget(self.searchText)

      #create right information hub
      i=0
      while i<len(headers):
          exec('self.right.'+headers[i]+'=QtGui.QLabel("'+headers[i]+'",self.right)')
          exec('self.right.'+headers[i]+'.move(20,'+str(70*i)+')')
          exec('self.right.'+'edt'+headers[i]+'=QtGui.QLineEdit(self.right)')
          exec('self.right.'+'edt'+headers[i]+'.move(60,'+str(70*i+30)+')')
          exec('self.right.'+'edt'+headers[i]+'.setFixedWidth(330)')
          i+=1
      return self
