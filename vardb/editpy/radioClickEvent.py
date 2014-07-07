#Julian Quick
#Logical flow of functions is from top to bottom
import getInfo
from delete import delete
from PyQt4 import QtGui, QtCore
from functools import partial 
from comboBoxPopUp import BoolComboBox

#Labes is used to store info for exec commands in textChange function
#bools determines if text has been changed
labes=[]
global bools
global previousnum
reftex =': out of bounds. Please use reference number.'
bools=False
rightInfoHub={'headers':{},'textBoxes':{}}

#removes leading descriptior from static lable, appends to signal list
def signalSet(self,name):
   global rightInfoHub
   return ((str(rightInfoHub['headers'][name].text())).lstrip(str(name)+':')).strip()
   #return info
#==============================================================================
#When any text box is edited, this function is called
def textChange(self,text,labes,num):
   global bools
   global ifs
   global rightInfoHub

   dictionary=getInfo.getDictionary()

   #info about new text is stored in static lable
   if labes[num]=='standard_name':
      try:
          rightInfoHub['headers'][labes[num]].setText(labes[num]+': '+getInfo.getStandardNames()[int(text)])
      except:
          rightInfoHub['headers'][labes[num]].setText(labes[num]+reftex)
   else:
      rightInfoHub['headers'][labes[num]].setText(labes[num]+': '+text)
   bools=True
   
   #check standard names
      

#==============================================================================
#Creates upright hand side lables and text boxes
#upright.lablename is static lable
#upright.edtlablename is text box whch can be edited
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
      else:
        print 'warning: neither true nor false found in ',rightInfoHub['headers'][lablename].text()
   #labes array created to refference different values in exec 
   labes.append(str(lablename))
   
   rightInfoHub['headers'][lablename].setText(lablename+': '+signalname)
   rightInfoHub['headers'][lablename].setMinimumSize(rightInfoHub['headers'][lablename].sizeHint())
   try:
     rightInfoHub['textBoxes'][lablename].setText(signalname)
     #Connvects changes in text box to bools logic
     rightInfoHub['textBoxes'][lablename].textEdited.connect(lambda: textChange(self,rightInfoHub['textBoxes'][lablename].text(),labes,num))
     textChange(self,rightInfoHub['textBoxes'][lablename].text(),labes,num)
   except:
     pass 


   bools=False

def clearRightInfoHub():
   global rightInfoHub
   dictionary=getInfo.getDictionary()
   i=0
   while i<len(dictionary):
      try: 
         rightInfoHub['headers'][dictionary[i]].close()
         rightInfoHub['textBoxes'][dictionary[i]].close()
      except:
         pass
      i+=1

def makeRightInfoHub(self,headers):
   global rightInfoHub
   global bools
   def onActivated(self,sig):
    global headers
    global rightInfoHub
    global bools
    rightInfoHub['headers'][sig].setText(sig+': '+rightInfoHub['textBoxes'][sig].currentText())
    rightInfoHub['headers'][sig].adjustSize() 
    bools=True

   i=0
   while i<len(headers):
     rightInfoHub['headers'][headers[i]]=QtGui.QLabel(headers[i],self.upright)
     self.upright.verticalLayoutScroll.addWidget(rightInfoHub['headers'][headers[i]])
     if headers[i] in self.booleanList:
        rightInfoHub['textBoxes'][headers[i]]=QtGui.QComboBox(self.upright)

        #warning: preserve true and false assignment order
        rightInfoHub['textBoxes'][headers[i]].addItem("true")
        rightInfoHub['textBoxes'][headers[i]].addItem("false")
        rightInfoHub['textBoxes'][headers[i]].activated.connect(partial(onActivated,self,headers[i]))
     else:
        rightInfoHub['textBoxes'][headers[i]]=QtGui.QLineEdit(self.upright)
     self.upright.verticalLayoutScroll.addWidget(rightInfoHub['textBoxes'][headers[i]])
     rightInfoHub['textBoxes'][headers[i]].setFixedWidth(500)
     i+=1

#==============================================================================
#num is signal's row in varDB.txt
#Bool becomes true when text box(es) are edited
#This function configures button settings based on which radio button is selected
#called by makeSingleRadioButton function (mkbut)
def lookingAt(num,self): #displays metadata based on radiobutton selection
   from addSignal import addsignal
   global previousnum
   global bools
   global reftex
  
   entries=getInfo.getinfo()
   dictionary=getInfo.getDictionary()
   headers=entries[num][0]

   #Check for changes to text boxes with bool
   #num =-1 is a flag that a variable is being deleted, and not to save changes made
   if bools==True and num!=-1:
      bools=False
      reply=QtGui.QMessageBox.question(self, 'Save Changes?', 'Do you want to save these changes?', QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
      if reply==QtGui.QMessageBox.Yes:
        i=1

        stdCheck=False
        #Correct standard name
        if ': '+str(rightInfoHub['headers']['standard_name'].text()).lstrip('standard_name: ')==reftex:
           reply=QtGui.QMessageBox.warning(self,'Warining','standard_name has an invalid reference number. Are you sure you wish to save these changes?',QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
           if reply==QtGui.QMessageBox.No:
              stdCheck=True
        else:
            rightInfoHub['headers']['standard_name'].setText(rightInfoHub['textBoxes']['standard_name'].text())
        #Get new information from text boxes
        if stdCheck==False:
           signalist=[[]]
           signalist[0]=[headers[0],signalSet(self,headers[0])]
           while i<len(headers):
             signalist.append([headers[i],signalSet(self,headers[i])])
             i+=1
           #Uses new information to replace previous information
           addsignal(signalist,self,num,{'action':'edit'})
           #lookingAt(num,self)
           return

   #If num is sent as a flag (-1)_, reset it to 0
   #This indicates not to save changes, so bool is set to False
   if num==-1:
      bools=False
      num=0


   #CLEAR RIGHT SIDE INFORMATION HUB
   clearRightInfoHub()

   #Create framework for upright side information hub
   makeRightInfoHub(self,headers)

#Create new lables and boxes for right side based on button selection
   #Get important info
   i=1
   signals=[]
   while i<len(entries[num][0])+1:
     signals.append(entries[num][i])
     i+=1
   i=0

   #write info to screen
   while i<len(signals):
      labler(entries[num][0][i],signals[i],self,i)
      i+=1

   #Disconnect buttons
   try: 
      self.deleteButton.clicked.disconnect()
   except Exception: pass
   try: 
      self.saveButton.clicked.disconnect()
   except Exception: pass


   #Connect radio buttons
   self.deleteButton.clicked.connect(lambda: delete(str(rightInfoHub['headers']['name'].text()).split(': ')[1],self,num)) 
   self.deleteAttributeButton.clicked.connect(lambda: BoolComboBox(num,str(rightInfoHub['headers']['name'].text()).split(': ')[1],self,2)) 
   
   self.saveButton.clicked.connect(lambda: lookingAt(num,self))
   clearRightInfoHub()
   previousnum=num
   bools=False
