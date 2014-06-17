#Julian Quick
#This function removes a variable from varDB and radio buttons using it's row number in varDB.txt (num)
import makeSingleRadioButton
from getInfo import getinfo
def rem(num,left,right,self):
      varDB=open('varDB.txt','r')
      lines = [line.strip() for line in varDB]
      varDB.close()
      entries=getinfo()
      varDB2=open('varDB.txt','w')
      for i in reversed(range(left.verticalLayoutScroll.count())): #Remove buttons
         left.verticalLayoutScroll.itemAt(i).widget().setParent(None)
      i=0
      while i < len(lines):
         if i<num:
            varDB2.write(lines[i]+"\n")
            makeSingleRadioButton.mkbut(i,entries[i][0],left,right,self,False)
         if i>num: #Prevent data shift
            varDB2.write(lines[i]+"\n")
            makeSingleRadioButton.mkbut(i-1,entries[i][0],left,right,self,False)
         i+=1
      varDB2.close()
      print "Remove succesful: ",lines[num]

