#Julian Quick
#num is signal's row in varDB.txt
#This function configures button settings based on which radio button is selected
#called by makeSingleRadioButton function (mkbut)
import getInfo
import delete
import edit
from PyQt4 import QtGui, QtCore
global lblnm
lblnm=[]
def onChanged(text,right,signalname,lablename):
   print "onchanged!"
   signm=signalname
   exec('right.'+lablename+'.setText(lablename+": "'+text+')')
   exec("right."+lablename+'.adjustSize()')
def labler(lablename,signalname,right,num):#Creates lable
   global signm
   global labes
   global rite #These variable connect with error messages wthout these reassignments
   rite=right
   lblnm.append(str(lablename))
   print "heiy",lblnm,num
   labes=str(lblnm[num])
   signm=signalname
   exec("right."+lablename+'.setText("'+lablename+': '+signalname+'")')
   exec("right."+lablename+".setMinimumSize(right."+lablename+".sizeHint())")
   exec("right."+'edt'+lablename+'.setText(signalname)')
   print "************"
   print 'rite, lablename, labes, signm,lblnm,num',rite, lablename, labes, signm,lblnm,num
   print "************"
   exec("right."+'edt'+lablename+'.textChanged.connect(lambda: onChanged(str(rite.edt'+labes+'.text()),rite,signm,'+str(lblnm[num])+'))')
def lookingAt(num,left,right,deleteButton,editButton,self): #displays metadata based on radiobutton selection
   headers=['name','units','decription']
   entries=getInfo.getinfo()
   signals=[]
   i=0
   while i<len(headers):
     signals.append(entries[num][i])
     i+=1
   i=0
   while i<len(signals):
      labler(headers[i],signals[i],right,i)
      i+=1
   try: 
      editButton.clicked.disconnect()
      deleteButton.clicked.disconnect()
   except Exception: pass
   deleteButton.clicked.connect(lambda: delete.delete(num,left,right,self,deleteButton,editButton)) #lambda does nothing
   editButton.clicked.connect(lambda: edit.edit(num,left,right,self,deleteButton,editButton))


