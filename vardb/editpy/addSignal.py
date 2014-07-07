#Julian Quick
#This function inserts a new signal into varDB in alphabetical order 
#input:
#   signals- text to be added to vardb
import csv
from getInfo import getinfo
from generateButtons import generateButtons
from radioClickEvent import lookingAt
from setup import fileName
from lxml import etree
def addsignal(signals,self,num,instructions):
   print 'changing VDB'
   parser = etree.XMLParser(remove_blank_text=True)
   doc=etree.parse('VDB.xml',parser)
   root=doc.getroot()
#+++++++++++
#Edits existing element
   if instructions['action']=='edit':
        for elm in root.iter('variable'):
         if elm.attrib['name'] == signals[0][1]:
               i=1
               while i<len(signals):
                  if elm[i-1].tag==signals[i][0] and elm[i-1].text != signals[i][1]:
                     elm[i-1].text=signals[i][1]
                     print 'changed ', elm.attrib['name'],elm[i-1].tag,' to ',elm[i-1].text
                  i+=1
#++++++++++++

   if instructions['action']=='delete':
      for elm in root.iter('variable'):
          if elm.attrib['name']==signals:
             elm.getparent().remove(elm)
             print 'removed ',signals
            
#+++++++++++
   if instructions['action']=='new signal':
   #Expecting signals to be list of for [ [attribute,name]*n]

       #Check for name input
       if signals[0][0]!='name' : 
           print 'exit code 33 in addsignal.py'
           quit()

       #Insert element
       elms=[]
       for elm in root.iter('variable'):
           elms.append(str(elm.attrib['name']))
       elms.append(signals[0][1])
       elms.sort()
          
       added=False
       i=0
       for elm in root.iter('variable'):
          if str(elm.attrib['name'])!=elms[i] and added==False:
             new = etree.Element('variable',name=signals[0][1])
             l=1
             while l<len(signals):
                subtext = etree.SubElement(new, signals[l][0])
                subtext.text = signals[l][1]
                l+=1
             elm.addprevious(new)
             added=True
             print 'added ',signals[0][1]
          i+=1


   if instructions['action']=='new attribute':
   #Expecting signals to be [name,attribute name,attribute text]
      for elm in root.iter('variable'):
         if str(elm.attrib['name'])==signals[0]:
            subtext=etree.SubElement(elm,signals[1])
            subtext.text=signals[2]
            print 'added ',signals

   if instructions['action']=='remove attribute':
   #Expecting signals to be [varible name, attribute]
      for elm in root.iter('variable'): 
          if elm.attrib['name']==signals[0]:
               for att in elm:
                   if att.tag==signals[1]:
                      print 'removed ',att.tag, ' from ',elm.attrib['name']
                      elm=elm.remove(att)

   doc.write('VDB.xml',pretty_print=True)
   generateButtons(self,self.searchText.text(),num)
   return


