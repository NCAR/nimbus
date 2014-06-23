#Julian Quick
#Generates "yes/no" radio buttons in tabs 1 and 2
#Tab 1 shows signals found in the air SQL, and allows the user to activate them in groundvars
#tab2 shows all potential signals found in groundvars (comented or not) and allws the user to activate them
#self is input to this function, allowing refference to the scroll layout, search bars, and checkboxes
from PyQt4 import QtGui, QtCore
from getGroundInfo import getInfo
from tab1Changes import ads
from tab2Changes import addit
from removeLayout import deleteLayout
def generateButtons(self):

    #Clear grids
    try:
       deleteLayout(self.grid1)
       deleteLayout(self.grid2)
    except Exception: pass

    #initialize grids
    self.grid1 = QtGui.QGridLayout()
    self.grid2 = QtGui.QGridLayout()

    #Filts are search terms
    filt1=str(self.searchText.text())
    filt2=str(self.searchText2.text())

    #global variable for exec executions
    global selfglobal
    selfglobal=self
    #save scroll positions
    yOffSet1 = self.scrollArea.verticalScrollBar().value()
    yOffSet2 = self.scrollArea2.verticalScrollBar().value()

    filename='groundvars'
    sqlnames,groundlines=getInfo()

    #Clear scroll areas
    for i in range(self.verticalLayoutScroll.count()): self.verticalLayoutScroll.itemAt(i).widget().close()
    for i in range(self.verticalLayout2Scroll.count()): self.verticalLayout2Scroll.itemAt(i).widget().close()


#-----------------------TAB1-------------------------------------
#Generates radio button set in tab 1

    i=0
    while i<len(sqlnames):
      if str(sqlnames[i]).startswith(filt1):
       if (sqlnames[i] in groundlines and self.cbyes.isChecked()) or (sqlnames[i] not in groundlines and self.cbno.isChecked()):
         groupbox1=QtGui.QGroupBox('')
         HBLays=QtGui.QHBoxLayout()
         no=QtGui.QRadioButton("NO")
         exec('no.released.connect(lambda:ads("'+str(sqlnames[i])+'",False,selfglobal))')
         yes=QtGui.QRadioButton("YES")
         exec('yes.released.connect(lambda:ads("'+str(sqlnames[i])+'",True,selfglobal))')
         nameLabel=QtGui.QLabel(sqlnames[i])
         if '#'+sqlnames[i] in groundlines:
            nameLabel=QtGui.QLabel('#'+sqlnames[i])
         self.grid1.addWidget(nameLabel,i,1)
         self.grid1.addWidget(yes,i,2)
         self.grid1.addWidget(no,i,3)
         if sqlnames[i] in groundlines:
            yes.setChecked(True)
            no.setChecked(False)
         else:
            yes.setChecked(False)
            no.setChecked(True)
      i+=1
    groupbox1.setLayout(self.grid1)
    self.verticalLayoutScroll.addWidget(groupbox1)


#------------------TAB2-----------------------------------
#Get useful entries is groundvars

    groundvars=open(filename,'r')
    lines=groundvars.readlines()
    groundvars.close()
    lines = [x for x in lines if not x.startswith('\n')]
    lines = [x for x in lines if x!='#\n']
    lines=map(lambda s: s.strip(),lines)
    i=0   
    #Generate radio button set in tab 2
    linecounts=0
    print sqlnames
    linecount=1
    while i<len(lines):
      
      #end of potential signals
      if str(lines[i][1:]).startswith('======='):
         break

      #Comments for project managers
      if str(lines[i][0])=='#' and lines[i][1:] not in '\t'.join(sqlnames):
          commentLabel=QtGui.QLabel(lines[i])
          self.grid2.addWidget(commentLabel,i,1)#,i,3)
   
      #Signal detection
      #-search box, signal is listed in air SQL (ignores characters at and after _)
      if (str(lines[i]).startswith(filt2) or (str(lines[i][1:]).startswith(filt2) and str(lines[i][0])=='#')) and str(lines[i][1:]).split('_',1)[0] in '\t'.join(sqlnames):
       
       #Checkbox filtration
       if lines[i]!='#' and (str(lines[i][0])=='#' and self.cbno2.isChecked()) or (self.cbyes2.isChecked() and str(lines[i][0])!='#'):

          #radio button row initialization
          groupbox2=QtGui.QGroupBox('')
          no=QtGui.QRadioButton("NO")
          exec('no.released.connect(lambda:addit("'+str(lines[i])+'",True,selfglobal))')
          yes=QtGui.QRadioButton("YES")
          exec('yes.released.connect(lambda:addit("'+str(lines[i])+'",False,selfglobal))')
          nameLabel=QtGui.QLabel(lines[i])
          self.grid2.addWidget(nameLabel,i,1)
          self.grid2.addWidget(yes,i,2)
          self.grid2.addWidget(no,i,3)
          if str(lines[i][0])!='#':
             yes.setChecked(True)
             no.setChecked(False)
          else:
             no.setChecked(True)
             yes.setChecked(False)
      i+=1 
    groupbox2.setLayout(self.grid2)
    self.verticalLayout2Scroll.addWidget(groupbox2)
  
    #Reset scroll bars to original positions
    self.scrollArea.ensureVisible(0,yOffSet1,1,1)
    self.scrollArea2.ensureVisible(0,yOffSet2,1,1)
