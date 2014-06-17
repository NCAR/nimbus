#Julian Quick
#this function returns a list of each signal's properties
import string
def getinfo():
           varDB=open('varDB.txt','r')
           lines = [line.strip() for line in varDB]
           entries=[elem.strip().split('  ') for elem in lines]
           entries=filter(None,entries)
           i=0
           while i<len(entries): #Removes clutter
              entries[i] = [s.lstrip() for s in entries[i]]
              entries[i]=filter(len,entries[i])
              i+=1
           varDB.close()
           return entries
