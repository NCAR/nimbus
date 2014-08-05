from lxml import etree
stdNames=open('StandardNames','r')
lines=stdNames.readlines()
stdDictionary={}
for line in lines:
  if line[0]!='#':
     line=line.strip('\n')
     stdDictionary[line.split(', ')[0]]=line.split(', ')[1]
doc=etree.parse('VDB.xml')
for elm in doc.getiterator('variable'):
   for att in elm:
      if att.tag=='standard_name':
        att.text=stdDictionary[att.text]
doc.write('VDB.xml',pretty_print=True)

