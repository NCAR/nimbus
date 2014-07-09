#Julian Quick
#This function uses varDB to generate radio buttons
#input:
#    num is which radio button to leave selected,using getinfo() as reference list
#    Filtering is search term
from PyQt4 import QtGui, QtCore
from getInfo import getinfo
def generateButtons(self,filtering,num):
           global rightInfoHub
           #removes buttons from left
           self.left.scrollAreaWidgetContents.clear()
           entries=getinfo()
           i=0
           while i<len(entries): 

              #displayed buttons must start with filter phrase
              if str(entries[i][1]).startswith(filtering) or filtering=='':
                    self.left.scrollAreaWidgetContents.addItem(entries[i][1])
                    if i==num:
                      self.left.scrollAreaWidgetContents.setItemSelected(self.left.scrollAreaWidgetContents.item(num),True)
              i+=1
