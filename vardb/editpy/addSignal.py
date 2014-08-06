#Julian Quick
#This is the master VDB edit function. This is the only write access point to VDB (getInfo reads VDB)
#Input:
#   signals-New information to transcribe. Is of different forms depending on desired action
#   self-holds all Gui information
#   num- used to call generate buttons after edditing is finished. -1 is a flag
#        for no need to generate the buttons again
#   instructions- dictionary with instructions of what action to take. Determines
#                 how VDB will be editted, and what signals is expected to look like.      
#example attribute: is_analog
import csv
from getInfo import *
from generateButtons import generateButtons
from radioClickEvent import lookingAt
from setup import fileName
from lxml import etree

logFile='/tmp/vardbEditor.log'

def addsignal(signals,self,num,instructions):
   parser = etree.XMLParser(remove_blank_text=True)
   doc=etree.parse(fileName(),parser)
   root=doc.getroot()
   log=open(logFile,'a')
#+++++++++++
   if instructions['action']=='edit':
   #Expecting signals to be of form [ [attribute,new valie] * n ]
   #Edits existing attributes

        added=False
        newAtt=[]
        j=0
        for elm in root.iter('variable'):
           if elm.attrib['name'] == signals[0][1].upper():

               #variable is already in vardb
               added=True
        
               #edit exiting entries
               i=0
               while i<len(elm):
                  for sig in signals:
                     if elm[i].tag == sig[0] and elm[i].text != sig[1]:
                        elm[i].text=sig[1]
                        print 'changed ', elm.attrib['name'],": ",elm[i].tag,' to '+elm[i].text+'.'
                        print>>log, 'changed ', elm.attrib['name'],": ",elm[i].tag,' to '+elm[i].text+'.'
                  i+=1

               #Check for new entries
               for sig in signals:
                  inThere=False
                  wasRemoved=False
                  i=0
                  while i<len(elm):
                     if elm[i].tag==sig[0]:
                        inThere=True
                     i+=1
                  if not inThere and sig[0] != 'name' and sig[1]!='':
                     newAtt.append(etree.SubElement(elm,sig[0]))
                     newAtt[j].text=sig[1]
                     for att in elm:
                        if getDictionary(fileName()).index(att.tag)>getDictionary(fileName()).index(sig[0]):
                           att.addnext(newAtt[j])
                           break
                     j+=1
                     print 'Added ',sig[0],' to ',elm.attrib['name'],' set as ',sig[1]
                     print>>log, 'Added ',sig[0],' to ',elm.attrib['name'],' set as ',sig[1]

               #check for blank entries, remove them
               for att in elm:
                   if str(att.text)=='':#and str(att.tag) not in [s[0] for s in self.catelogList]:
                      print 'removed ',att.tag,' from ',elm.attrib['name']
                      print>>log, 'removed ',att.tag,' from ',elm.attrib['name']
                      elm.remove(att)
        if added==False:
            instructions['action']='new signal'
            print str(signals[0][1]).upper(),' not found in VDB. Creating new entry.'
            print>>log, str(signals[0][1]).upper(),' not found in VDB. Creating new entry.'
#++++++++++++
   if instructions['action']=='delete':
      for elm in root.iter('variable'):
          if elm.attrib['name']==signals:
             elm.getparent().remove(elm)
             print 'removed ',signals
             print>>log, 'removed ',signals
            
#+++++++++++
   if instructions['action']=='new signal':
   #Expecting signals to be list of for [ [attribute,value]*n]
   #Inserts a new signal into varDB in alphabetical order 

       #Check for name input
       if signals[0][0]!='name' : 
           print 'exit code 1 in addsignal.py: no name was input'
           print>>log, 'exit code 1 in addsignal.py: no name was input'
           quit()

       #Insert element
       elms=[]
       for elm in root.iter('variable'):
           elms.append(str(elm.attrib['name']))
       elms.append(str(signals[0][1]).upper())
       elms.sort()
          
       added=False
       i=0
       for elm in root.iter('variable'):
          if str(elm.attrib['name'])!=elms[i] and added==False:
             new = etree.Element('variable',name=signals[0][1].upper())
             l=1
             while l<len(signals):
                if signals[l][1]!='':
                   subtext = etree.SubElement(new, signals[l][0])
                   subtext.text = signals[l][1]
                l+=1
             elm.addprevious(new)
             added=True
             num=i
             print>>log, 'added ',signals[0][1]
             print 'added ',signals[0][1]
          i+=1
       if added==False:
          new = etree.Element('variable',name=signals[0][1].upper())
          l=1
          while l<len(signals):
             subtext = etree.SubElement(new, signals[l][0])
             subtext.text = signals[l][1]
             l+=1
          elm.addnext(new)
          added=True
          num=i
          print 'appended ',signals[0][1]
          print>>log, 'appended ',signals[0][1]
           
#=============
   doc.write('VDB.xml',pretty_print=True)
   generateButtons(self,str(self.searchText.text()),num)
   return


