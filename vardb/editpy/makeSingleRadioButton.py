#Julian Quick
#This function creates a radio button in the left scroll area, given a name for the button (name), and it's row in varDB.txt (num)
#Chekd is a logical variable. If true, the button will be selected by this program
#import sys
#from PyQt4 import QtGui, QtCore
#from radioClickEvent import lookingAt
def mkbut(num,name,self,chekd):
      global rightInfoHub
      #button=QtGui.QRadioButton(name,self.left)
      #self.left.verticalLayoutScroll.addItem(button)
      self.left.scrollAreaWidgetContents.addItem(name)
      return
      try:
         button.clicked.disconnect()
      except Exception: pass
      button.clicked.connect(lambda: lookingAt(num,self))
      if chekd==True:
         button.click()
