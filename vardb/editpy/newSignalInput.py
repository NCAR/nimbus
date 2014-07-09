#Julian Quick
#This function is called when the 'add signal' button is pressed, connected in GUIMain
#Function offers user to input every attribute in VARDB dictionary, or to leave input blank

from PyQt4 import QtGui, QtCore
from addSignal import addsignal
from getInfo import getDictionary
from comboBoxPopUp import ComboBox
import time
#############################3##3
#Individual popups asking for an attribute's information. Does not support dropdown menus
def dial(attri,list,self):
  input, ok =QtGui.QInputDialog.getText(self, 'Information Aquisition', 'Enter '+attri+'. If no information, leave blank.')
  if ok:
    input=str(input)
    if input.strip()=='False' or input.strip=='FALSE':
       input='false'
    if input.strip()=='True' or input.strip()=='TRUE':
       input='true'
    if input != '':
       list.append([attri,input.strip()])
    return list
  else:
    return False
###########################################################
#Main new signal input function (what is called by main program)
def ads(self):
  dictionary=getDictionary()
  newInput=[[]]

  input, ok =QtGui.QInputDialog.getText(self, 'Information Aquisition', 'Enter name')
  if ok:
     newInput[0]=([dictionary[0],(str(input).strip()).upper()])

     #If a dropdown menu is created inside a loop, python won't wait for the information to be aquired.
     #Attribute aquisition has been seperated into textboxe input, then input by selection from a drop down menu
     i=1
     while i<len(dictionary):
        if dictionary[i] not in self.booleanList:
           
           #Information is aquired in dial through textbox aquisition
           if(not dial(dictionary[i],newInput,self)):
              return
        i+=1

     #booleanList (initialized in setup.py) is a list of known boolean attributes
     for name in self.booleanList:
     
        #Aquire boolean attribute information through dropsown menu
        ComboBox(newInput[0][1],name,self,1)

     #TO DO: add standard_names input dialogue

     print 'adding ',newInput
     addsignal(newInput,self,-1,{'action':'new signal'})
