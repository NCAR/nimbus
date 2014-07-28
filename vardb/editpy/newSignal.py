
from getInfo import getDictionary
from setup import fileName
from radioClickEvent import *
def newSig(self):

    boolsTrue()

    #Deselect qlistwidget
    for entry in range(self.left.scrollAreaWidgetContents.count()):
        item=self.left.scrollAreaWidgetContents.item(entry)
        self.left.scrollAreaWidgetContents.setItemSelected(item,False)

    clearRightInfoHub()
    makeRightInfoHub(self,getDictionary(fileName()))

    i=0
    for entry in getDictionary(fileName()):
        labler(entry,'',self,i) 
        i+=1

    self.saveButton.clicked.connect(lambda:saveChanges(self,headers,-1))

#def adding(self):
    
