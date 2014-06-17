#Julian Quick
#This function uses varDB to generate radio buttons
#num is which radio button to leave selected
#Filtering is search term
from PyQt4 import QtGui, QtCore
from makeSingleRadioButton import mkbut
from getInfo import getinfo
def generateButtons(left,right,self,filtering,num):
           #removes buttons from left
           for i in reversed(range(left.verticalLayoutScroll.count())): 
               left.verticalLayoutScroll.itemAt(i).widget().setParent(None)
           entries=getinfo()
           varDB=open('varDB.txt','r')
           i=0
           while i<len(entries): 
              #displayed buttons must start with filter phrase
              if str(entries[i][0]).startswith(filtering) or filtering=='':
                 if num!=i:
                    mkbut(i,entries[i][0],left,right,self,False)
                 if num==i:
                    #This button will be "machine clicked"
                    mkbut(i,entries[i][0],left,right,self,True)
              i+=1
           varDB.close()
