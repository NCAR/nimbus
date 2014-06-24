#Julian Quick
#This function removes a variable from varDB and radio buttons using it's row number in varDB.txt (num)
import makeSingleRadioButton
from getInfo import getinfo
def rem(num,self):
      from generateButtons import generateButtons
      varDB=open('varDB.txt','r')
      lines = [line.strip() for line in varDB]
      varDB.close()
      entries=getinfo()
      varDB2=open('varDB.txt','w')
      for i in reversed(range(self.left.verticalLayoutScroll.count())): #Remove buttons
         self.left.verticalLayoutScroll.itemAt(i).widget().setParent(None)
      i=0
      while i < len(lines):
         if i<num:
            varDB2.write(lines[i]+"\n")
         if i>num: #Prevent data shift
            varDB2.write(lines[i]+"\n")
         i+=1
      varDB2.close()
      generateButtons(self,self.searchText.text(),0)
      print "Remove succesful: ",lines[num]

