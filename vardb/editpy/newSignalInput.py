#Julian Quick
#This function allows the user to input signal information to the 
#addsignal function, or to cancel at any time
from PyQt4 import QtGui, QtCore
from addSignal import addsignal
from getInfo import getDictionary
from comboBoxPopUp import BoolComboBox
import time

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

def ads(self):
  dictionary=getDictionary()
  newInput=[[]]

  input, ok =QtGui.QInputDialog.getText(self, 'Information Aquisition', 'Enter name')
  if ok:
     newInput[0]=([dictionary[0],str(input).strip()])
     i=1
     while i<len(dictionary):
        if dictionary[i] not in self.booleanList:
           if(not dial(dictionary[i],newInput,self)):
              return
        i+=1
     for name in self.booleanList:
        BoolComboBox(newInput[0][1],name,self,1)
     print newInput
     addsignal(newInput,self,-1,{'action':'new signal'})
