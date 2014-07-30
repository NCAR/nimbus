from lxml import etree
from getInfo import getDictionary
import os

derive='derive'
mod='modulus_range'
deps='dependencies'
lags='static_lag'
spike='spike_slope'
#--------------------Derived dictionary-------------------------------
drname=open(os.path.expandvars('$PROJ_DIR/Configuration/raf/DerivedNames'),'r')
lines=drname.readlines()
DRdi={}
for line in lines:
   line=line.strip('\t')
   line=line.strip('\n')
   if line[0]!='#':
      try:
        DRdi[line.split('\t')[0]]=line.split('\t')[1]
      except IndexError:
        DRdi[line.split(' ')[0]]=line.split(' ')[1]
        
#--------------------Modvars dictionary-------------------------------
modname=open(os.path.expandvars('$PROJ_DIR/Configuration/raf/ModVars'),'r')
lines=modname.readlines()
modi={}
for line in lines:
   line=line.strip('\n')
   if line[0]!='#':
      modi[line.split('\t')[0]]=line.split('\t')[1]+' '+line.split('\t')[2]
        
doc=etree.parse('VDB.xml')
VDBDict=getDictionary('VDB.xml')

#-----------------------Local Files---------------------------------------
def makeDi(fileName):
   f=open(fileName,'r')
   lines=f.readlines()
   di={}
   for line in lines:
      line=line.strip('\n')
      if line!='':
        if line[0]!='#':
           try:
              di[line.split('\t')[0]]=line.split('\t')[1]
           except IndexError:
              try:
                 di[line.split('  ')[0]]=line.split('  ')[1]
              except IndexError:
                 try:
                   di[line.split(' ')[0]]=line.split(' ')[1]
                 except IndexError:
                   di[line.split(' ')[0]]=''
   return di 

depdi=makeDi('DependTable')
lagdi=makeDi('Lags')
spikedi=makeDi('Despike')

#----modvars
for elm in doc.getiterator('variable'):
  for key in modi:
    if elm.attrib['name']==key.strip():
       subtext = etree.SubElement(elm, mod)
       subtext.text = modi[key]
       for att in elm:
          if VDBDict.index(att.tag)>VDBDict.index(mod):
             att.addprevious(subtext)

#---- derived
  for key in DRdi:
    if elm.attrib['name']==key.strip():
       subtext = etree.SubElement(elm, derive)
       subtext.text = DRdi[key]
       for att in elm:
          if VDBDict.index(att.tag)>VDBDict.index(derive):
             att.addprevious(subtext)

#---- depndTable
  for key in depdi:
    if elm.attrib['name']==key.strip():
       subtext = etree.SubElement(elm, deps)
       subtext.text = depdi[key]
       for att in elm:
          if VDBDict.index(att.tag)>VDBDict.index(deps):
             att.addprevious(subtext)

#---- lags
  for key in lagdi:
    if elm.attrib['name']==key.strip():
       subtext = etree.SubElement(elm, lags)
       subtext.text = lagdi[key]
       for att in elm:
          if VDBDict.index(att.tag)>VDBDict.index(lags):
             att.addprevious(subtext)

#---- despike
  for key in spikedi:
    if elm.attrib['name']==key.strip():
       subtext = etree.SubElement(elm, spike)
       subtext.text = spikedi[key]
       for att in elm:
          if VDBDict.index(att.tag)>VDBDict.index(spike):
             att.addprevious(subtext)

doc.write('VDB.xml',pretty_print=True)


