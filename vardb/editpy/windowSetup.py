#This module sets the window and scroll area for the GUI
#Window Dimensions
import initSelf
self.setGeometry(300, 300, 600, 600)
self.setWindowTitle('varDB GUI')
#Create middle division
hbox = QtGui.QHBoxLayout(self)
self.left = QtGui.QFrame(self)
self.left.setFrameShape(QtGui.QFrame.StyledPanel)
self.right = QtGui.QFrame(self)
self.right.setFrameShape(QtGui.QFrame.StyledPanel)
splitter1 = QtGui.QSplitter(QtCore.Qt.Horizontal)
splitter1.addWidget(self.left)
splitter1.addWidget(self.right)
splitter1.setSizes([150,300])
hbox.addWidget(splitter1)
self.setLayout(hbox)
QtGui.QApplication.setStyle(QtGui.QStyleFactory.create('Cleanlooks'))
#Create scroll area
self.left.scrollArea=QtGui.QScrollArea(self.left)
self.left.scrollArea.setWidgetResizable(True)
self.left.scrollAreaWidgetContents=QtGui.QWidget(self.left.scrollArea)
self.left.scrollArea.setWidget(self.left.scrollAreaWidgetContents)
self.left.verticalLayout=QtGui.QVBoxLayout(self.left)
self.left.verticalLayout.addWidget(self.left.scrollArea)
self.left.verticalLayoutScroll=QtGui.QVBoxLayout(self.left.scrollAreaWidgetContents)

