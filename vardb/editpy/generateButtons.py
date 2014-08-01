#Julian Quick
#This function uses varDB to generate radio buttons
#input:
#    num is which radio button to leave selected,using getinfo() as reference list
#    Filtering is search term
from PyQt4 import QtGui, QtCore
from getInfo import getinfo
from setup import fileName
def generateButtons(self,filtering,num):
           global rightInfoHub
           #removes buttons from left
           self.left.scrollAreaWidgetContents.clear()
           entries=getinfo(fileName())
           i=0
           while i<len(entries): 

              #displayed buttons must start with filter phrase
              if filtering.upper() in str(entries[i][1]).upper() or filtering=='':
                    self.left.scrollAreaWidgetContents.addItem(entries[i][1])
              i+=1
           if num>=0:
              self.left.scrollAreaWidgetContents.setItemSelected(self.left.scrollAreaWidgetContents.item(num),True)
              try:self.left.scrollAreaWidgetContents.scrollToItem(self.left.scrollAreaWidgetContents.selectedItems()[0])
              except:pass
