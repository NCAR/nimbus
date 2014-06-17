#This module sets the window and scroll area for the GUI
#Window Dimensions
import initSelf
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
splitter1.setSizes([150,300])
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

