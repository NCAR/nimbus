#Julian Quick
#Logical flow of functions is from top to bottom
import getInfo
from remove import rem
from delete import delete
from PyQt4 import QtGui, QtCore

#Labes is used to store info for exec commands in textChange function
#bools determines if text has been changed
labes=[]
global bools
global previousnum
bools=False

#removes leading descriptior from static lable, appends to signal list
def signalSet(self,name,signalist):
   exec('signalist.append(str(self.right.'+name+'.text()).lstrip("'+str(name)+': "))')
   return signalist
#==============================================================================
#When any text box is edited, this function is called
def textChange(self,text,labes,num):
   global bools

   #info about new text is stored in static lable
   exec('self.right.'+labes[num]+'.setText(labes[num]+": "+text)')
   bools=True

#==============================================================================
#Creates right hand side lables and text boxes
#right.lablename is static lable
#right.edtlablename is text box whch can be edited
def labler(lablename,signalname,self,number):

   #incoming signal names are local, must be global for use in exec
   global selfglobal
   global num
   num=number
   num=number
   selfglobal=self
   #array created to refference different values in exec (patch)
   labes.append(str(lablename))
   exec("self.right."+lablename+'.setText("'+lablename+': '+signalname+'")')
   exec("self.right."+lablename+".setMinimumSize(self.right."+lablename+".sizeHint())")
   exec("self.right."+'edt'+lablename+'.setText(signalname)')
   #Connvects changes in text box to bools logic
   exec('self.right.edt'+lablename+'.textEdited.connect(lambda: textChange(selfglobal,selfglobal.right.edt'+lablename+'.text(),labes,'+str(num)+'))')

#==============================================================================
#num is signal's row in varDB.txt
#Bool becomes true when text box(es) are edited
#This function configures button settings based on which radio button is selected
#called by makeSingleRadioButton function (mkbut)
def lookingAt(num,self): #displays metadata based on radiobutton selection
   from addSignal import addsignal
   global previousnum
   global bools

   #Eventually read headers from varDB
   headers=['name','units','description']
   entries=getInfo.getinfo()

   #Check for changes to text boxes with bool
   #num =-1 is a flag that a variable is being deleted, and not to save changes made
   if bools==True and num!=-1:
      bools=False
      reply=QtGui.QMessageBox.question(self, 'Save Changes?', 'Do you want to save these changes?', QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
      if reply==QtGui.QMessageBox.Yes:
        i=0
        #Get new information from text boxes
        signalist=[]
        while i<len(headers):
          signalist=signalSet(self,headers[i],signalist)
          i+=1
        #Uses new information to replace previous information
        rem(previousnum,self)
        addsignal(signalist,self,num)
        lookingAt(num,self)
        return

   #If num is sent as a flag (-1)_, reset it to 0
   #This indicates not to save changes, so bool is set to False
   if num==-1:
      bools=False
      num=0

#Create new lables and boxes for right side based on button selection
   #Get important info
   i=0
   signals=[]
   while i<len(headers):
     try:signals.append(entries[num][i])
     except Exception:pass
     i+=1
   i=0
   #write info to screen
   while i<len(signals):
      labler(headers[i],signals[i],self,i)
      i+=1
   try: 
      self.deleteButton.clicked.disconnect()
   except Exception: pass
   try: 
      self.saveButton.clicked.disconnect()
   except Exception: pass

   #Update delete button num refference
   self.deleteButton.clicked.connect(lambda: delete(num,self)) 
   
   self.saveButton.clicked.connect(lambda: lookingAt(num,self))
   previousnum=num
