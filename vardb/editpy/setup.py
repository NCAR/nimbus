#Julian QUick
#Sets up GUI Window, scroll area, and data lables
from PyQt4 import QtGui, QtCore
def setup(hbox,left,right,self):
      #Eventually Headers should be read from varDB
      headers=['name','units','description']
      #Create middle division
      splitter1 = QtGui.QSplitter(QtCore.Qt.Horizontal)
      splitter1.addWidget(left)
      splitter1.addWidget(right)
      splitter1.setStretchFactor(1,1)
      hbox.addWidget(splitter1)
      left.setFrameShape(QtGui.QFrame.StyledPanel)
      right.setFrameShape(QtGui.QFrame.StyledPanel)
      QtGui.QApplication.setStyle(QtGui.QStyleFactory.create('Cleanlooks'))
      #Create scroll area
      left.scrollArea=QtGui.QScrollArea(left)
      left.scrollArea.setWidgetResizable(True)
      left.scrollAreaWidgetContents=QtGui.QWidget(left.scrollArea)
      left.scrollArea.setWidget(left.scrollAreaWidgetContents)
      left.verticalLayout=QtGui.QVBoxLayout(left)
      left.verticalLayout.addWidget(left.scrollArea)
      left.verticalLayoutScroll=QtGui.QVBoxLayout(left.scrollAreaWidgetContents)
      #Create search bar
      searchLabel = QtGui.QLabel(" Search", self)
      left.verticalLayout.addWidget(searchLabel)
      self.searchText = QtGui.QLineEdit()
      searchLabel.setBuddy(self.searchText)
      left.verticalLayout.addWidget(self.searchText)
      #create right information hub
      i=0
      while i<len(headers):
          exec('right.'+headers[i]+'=QtGui.QLabel("'+headers[i]+'",right)')
          exec('right.'+headers[i]+'.move(20,'+str(70*i)+')')
          exec('right.'+'edt'+headers[i]+'=QtGui.QLineEdit(right)')
          exec('right.'+'edt'+headers[i]+'.move(60,'+str(70*i+30)+')')
          exec('right.'+'edt'+headers[i]+'.setFixedWidth(330)')
          i+=1
      return right
