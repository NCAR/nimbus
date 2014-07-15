
from getInfo import getDictionary
from radioClickEvent import *
def newSig(self):

    boolsTrue()

    #Deselect qlistwidget
    for entry in range(self.left.scrollAreaWidgetContents.count()):
        item=self.left.scrollAreaWidgetContents.item(entry)
        self.left.scrollAreaWidgetContents.setItemSelected(item,False)

    clearRightInfoHub()
    makeRightInfoHub(self,getDictionary())

    i=0
    for entry in getDictionary():
        labler(entry,'',self,i) 
        i+=1

    self.saveButton.clicked.connect(lambda:saveChanges(self,headers,0))

#def adding(self):
    
