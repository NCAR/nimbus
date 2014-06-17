#Julian Quick
#This function inserts a new signal into varDB in alphabetical order 
import csv
from getInfo import getinfo
from generateButtons import generateButtons
def addsignal(signals,self,left,right,num):
                 varDB=open('varDB.txt','r')
                 entries=getinfo()
                 varDB.close()
                 varDB2=open("varDB.txt","w")
                 entries.append(signals)
                 entries.sort()
                 i=0
                 while i<len(entries):
                   j=1
                   writer=entries[i][0]
                   while j<len(entries[i]):
                      #Character seperated values
                      writer+='   '+entries[i][j]
                      j+=1
                   varDB2.write(writer+'\n')
                   i+=1
                 print "addition succesful",signals
                 varDB2.close()
                 generateButtons(left,right,self,self.searchText.text(),num)
