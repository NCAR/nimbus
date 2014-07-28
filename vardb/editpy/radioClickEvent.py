#Julian Quick
#Logical flow of functions is from bottom to top
import getInfo
from delete import delete
from PyQt4 import QtGui, QtCore
from functools import partial 
from comboBoxPopUp import ComboBox
from setup import fileName

#Labes is used to store info for exec commands in textChange function
#bools determines if text has been changed
#headers contains a list of attribute names
headers=[]
#global bools
#global previousnum
bools=False
rightInfoHub={'headers':{},'textBoxes':{}}
#------------------------------------------------------------------------------
#This function is used by newSignal.py to set the global variable bools true, indicating 
#to save edits if the user switches from the addsignal screen
def boolsTrue():
   global bools
   bools=True
#-----------------------------------------------------------------------------
#removes leading descriptior from static lable, appends to signal list
def signalSet(self,name):
   global rightInfoHub
   try:
      s=rightInfoHub['textBoxes'][name].text()
      s=rightInfoHub['textBoxes'][name].text()
   except AttributeError:
      s=rightInfoHub['textBoxes'][name].currentText()
   return str(s)
   #return info
#-----------------------------------------------------------------------------
#When any right info hub text box is edited, this function is called
#Sets textbox label as text, used to access text later.
def textChange():
   global bools
   bools=True
   if str(rightInfoHub['textBoxes']['is_analog'].currentText())=='false':
      rightInfoHub['textBoxes']['voltageRange'].setDisabled(True)
      rightInfoHub['textBoxes']['voltageRange'].setCurrentIndex(0)
      rightInfoHub['textBoxes']['defaultSampleRate'].setDisabled(True)
      rightInfoHub['textBoxes']['defaultSampleRate'].setCurrentIndex(0)
   else:
      rightInfoHub['textBoxes']['voltageRange'].setDisabled(False)
      rightInfoHub['textBoxes']['defaultSampleRate'].setDisabled(False)
#-----------------------------------------------------------------------------
#Creates upright hand side lables and text boxes
def labler(lablename,signalname,self,num):
   #incoming signal names are local, must be global for use in exec
   global selfglobal
   global rightInfoHub
   selfglobal=self

   #boolean logic
   if lablename in self.booleanList:
      if signalname=='true':
        rightInfoHub['textBoxes'][lablename].setCurrentIndex(0)
      elif signalname=='false':
        rightInfoHub['textBoxes'][lablename].setCurrentIndex(1)
   if lablename in (x[0] for x in self.catelogList) and signalname!='':
      if lablename==self.catelogList[0][0]:
         stdList=getInfo.getStandardNames(fileName())
      else:
         stdList=getInfo.getCategories(fileName())

      rightInfoHub['textBoxes'][lablename].setCurrentIndex(stdList.index(signalname))
      
   rightInfoHub['headers'][lablename].setText(lablename)
   rightInfoHub['headers'][lablename].setMinimumSize(rightInfoHub['headers'][lablename].sizeHint())
   if lablename not in self.booleanList and lablename not in (x[0] for x in self.catelogList) and lablename not in ['voltageRange','defaultSampleRate']:
     rightInfoHub['textBoxes'][lablename].setText(str(signalname))

     #Connvects changes in text box to bools logic
     rightInfoHub['textBoxes'][lablename].textEdited.connect(lambda: textChange())
     textChange()
   #global variable bools set False to indicate no changes have been made yet
   bools=False

#------------------------------------------------
def clearRightInfoHub():
   global rightInfoHub
   dictionary=getInfo.getDictionary(fileName())
   i=0
   while i<len(dictionary):
      try: 
         rightInfoHub['headers'][dictionary[i]].close()
         rightInfoHub['textBoxes'][dictionary[i]].close()
      except:
         pass
      i+=1

#---------------------------------------------------
#Populates upper right box with information
def makeRightInfoHub(self,headers):
   global rightInfoHub
   global bools
   clearRightInfoHub()
   def onActivated(self,sig):
    global rightInfoHub
    global bools
    bools=True

   i=0
   while i<len(headers):
     rightInfoHub['headers'][headers[i]]=QtGui.QLabel(headers[i],self.upright)
     self.upright.verticalLayoutScroll.addWidget(rightInfoHub['headers'][headers[i]],i,1)

     #Check for boolean variable
     if headers[i] in self.booleanList:
        rightInfoHub['textBoxes'][headers[i]]=QtGui.QComboBox(self.upright)

        #warning: preserve assignment order
        rightInfoHub['textBoxes'][headers[i]].addItem("true")
        rightInfoHub['textBoxes'][headers[i]].addItem("false")
        rightInfoHub['textBoxes'][headers[i]].setCurrentIndex(1)
        rightInfoHub['textBoxes'][headers[i]].activated.connect(partial(onActivated,self,headers[i]))
        rightInfoHub['textBoxes'][headers[i]].activated.connect(lambda:textChange())
     #Check if variable is in catalog list
     elif headers[i] in (entry[0] for entry in self.catelogList):
        rightInfoHub['textBoxes'][headers[i]]=QtGui.QComboBox(self.upright)

        if headers[i]==self.catelogList[0][0]:
           stdList=getInfo.getStandardNames(fileName())
        else:
           stdList=getInfo.getCategories(fileName())

        for item in stdList:
          rightInfoHub['textBoxes'][headers[i]].addItem(item)
        rightInfoHub['textBoxes'][headers[i]].currentIndexChanged.connect(lambda:textChange())
     elif headers[i]=='defaultSampleRate':
        rightInfoHub['textBoxes'][headers[i]]=QtGui.QComboBox(self.upright)
        rightInfoHub['textBoxes'][headers[i]].addItem("")
        rightInfoHub['textBoxes'][headers[i]].addItem("500")
        rightInfoHub['textBoxes'][headers[i]].addItem("100")
        rightInfoHub['textBoxes'][headers[i]].addItem("50")
        rightInfoHub['textBoxes'][headers[i]].addItem("10")
        rightInfoHub['textBoxes'][headers[i]].setCurrentIndex(0)
        rightInfoHub['textBoxes'][headers[i]].activated.connect(partial(onActivated,self,headers[i]))
        rightInfoHub['textBoxes'][headers[i]].activated.connect(lambda:textChange())
     elif headers[i]=='voltageRange':
        rightInfoHub['textBoxes'][headers[i]]=QtGui.QComboBox(self.upright)
        rightInfoHub['textBoxes'][headers[i]].addItem("")
        rightInfoHub['textBoxes'][headers[i]].addItem("-10 10")
        rightInfoHub['textBoxes'][headers[i]].addItem("-5 5")
        rightInfoHub['textBoxes'][headers[i]].addItem("-2.5 2.5")
        rightInfoHub['textBoxes'][headers[i]].addItem("0 10")
        rightInfoHub['textBoxes'][headers[i]].setCurrentIndex(0)
        rightInfoHub['textBoxes'][headers[i]].activated.connect(partial(onActivated,self,headers[i]))
        rightInfoHub['textBoxes'][headers[i]].activated.connect(lambda:textChange())
     else:
        rightInfoHub['textBoxes'][headers[i]]=QtGui.QLineEdit(self.upright)
        rightInfoHub['textBoxes'][headers[i]].textChanged.connect(lambda:textChange())
     self.upright.verticalLayoutScroll.addWidget(rightInfoHub['textBoxes'][headers[i]],i,2)
     rightInfoHub['textBoxes'][headers[i]].setFixedWidth(500)
     i+=1
#-------------------------------------------
#This function saves edits which have been made
#input:
#    headers is a list of attributes editted
#    num is the variable's alphanumeric position in vardb
def saveChanges(self,headers,num):
   global bools
   global rightInfoHub
   if bools==True:
      from addSignal import addsignal
      bools=False
      reply=QtGui.QMessageBox.question(self, 'Save Changes?', 'Save these changes to '+rightInfoHub['textBoxes']['name'].text()+'?', QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
      if reply==QtGui.QMessageBox.Yes:
        i=1

        headers=getInfo.getDictionary(fileName())
        #Get new information from text boxes
        signalist=[[]]
        signalist[0]=['name',signalSet(self,'name')]
        while i<len(headers):

             #signalist is of the form [ [attribute name, attribute text] * N]
           signalist.append([headers[i],signalSet(self,headers[i])])
           i+=1
                
             #Uses new information to replace previous information.
             #If entry with desired name exists, new entry is created
        addsignal(signalist,self,-1,{'action':'edit'})
#==============================================================================
#==============================================================================
#This function displays editable information regarding the current selection
#num is signal's alphanumeric position in VDB
#Bool becomes true when text box(es) are edited
def lookingAt(self):
   global previousnum
   global bools
   global reftex
   global headers

   entries=getInfo.getinfo(fileName())
   dictionary=getInfo.getDictionary(fileName())

   #Stiffle error message when no signal is selected
   try:text= str(self.left.scrollAreaWidgetContents.selectedItems()[0].text())
   except:return

   #get current selection's position in vdb
   i=0
   while i<len(entries):
      if entries[i][1]==text:
         num=i
      i+=1

   #Checks for changes to text boxes with bool
   #num =-1 is a flag that a variable is being deleted, and not to save changes made
   if bools==True and num!=-1:
      bools=False
      saveChanges(self,headers,num)
      return

   #If num is set as flag -1, reset it to 0
   #This indicates not to save changes, so bool is set to False
   if num==-1:
      bools=False
      num=0

   #Get attributes
   #headers=entries[num][0]
   headers=getInfo.getDictionary(fileName())

   #CLEAR RIGHT SIDE INFORMATION HUB
   clearRightInfoHub()

   #Create framework for upright side information hub
   makeRightInfoHub(self,headers)

   #Get signal's attribute info
   i=1
   signals=[]
   while i<len(entries[num][0])+1:
     
     signals.append(entries[num][i])
     i+=1
   i=0

   #write attribute info to screen
   while i<len(entries[num][0]):
      if entries[num][0][i] in headers:
        labler(entries[num][0][i],signals[i],self,i)
        headers.remove(entries[num][0][i])
      else:
        labler(dictionary[0],'',self,i)
      i+=1

   #Disconnect buttons
   try: 
      self.deleteButton.clicked.disconnect()
   except Exception: pass
   try: 
      self.saveButton.clicked.disconnect()
   except Exception: pass


   #Connect buttons
   self.deleteButton.clicked.connect(lambda: delete(str(rightInfoHub['textBoxes']['name'].text()),self,num)) 
   self.saveButton.clicked.connect(lambda: saveChanges(self,headers,num))

   clearRightInfoHub()
   previousnum=num
   bools=False
