#Julian Quick
#this function returns a list of each signal's properties
from lxml import etree
from setup import fileName
def getRef(name):
   doc=etree.parse(fileName())
   stdnames=[]
   for elm in doc.getiterator(name):
      i=0
      while i<len(elm):
         stdnames.append(elm[i].attrib['name'])
         i+=1
   return stdnames
def getCategories():
   doc=etree.parse(fileName())
   stdnames=[]
   for elm in doc.getiterator('Categories'):
      i=0
      while i<len(elm):
         stdnames.append(elm[i].attrib['name'])
         i+=1
   return stdnames
def getStandardNames():
   doc=etree.parse(fileName())
   stdnames=[]
   for elm in doc.getiterator('standardNames'):
      i=0
      while i<len(elm):
         stdnames.append(elm[i].attrib['name'])
         i+=1
   return stdnames
def getinfo():
   doc=etree.parse(fileName())
   infoList=[[[]]for elm in doc.getiterator('variable')]
   j=0
   for elm in doc.getiterator('variable'):
      infoList[j][0].append('name')
      infoList[j].append(elm.attrib['name'])
      i=0
      while i<len(elm):
         infoList[j][0].append(elm[i].tag)
         infoList[j].append(str(elm[i].text))
         i+=1
      j+=1
   return infoList

def getDictionary():
   doc=etree.parse(fileName())
   dictionary=[]
   dictionary.append('name')
   for elm in doc.getiterator('Dictionary'):
      i=0
      while i<len(elm):
         dictionary.append(elm[i].attrib['name'])
         i+=1
   return dictionary
