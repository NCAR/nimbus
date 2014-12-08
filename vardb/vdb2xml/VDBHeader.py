#!/usr/bin/python

vdb=open('VDB.xml','w')
vdb.write('<?xml version="1.0" encoding="UTF-8" standalone="no" ?><VarDB xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="VDBSchema.xsd">\n')
def add(name,iter,file):
  vdb.write('  <'+name+'>\n')
  cats=open(file)
  lines=cats.readlines()
  catD={}
  for line in lines:
    line=line.strip('\n')
    if 'None' not in line and line[0] != '#':
      vdb.write('    <'+iter+' name="'+line.split(', ')[1]+'"/>\n')
  vdb.write('  </'+name+'>\n')

add('categories','category','Categories')
add('standard_names','standard_name','StandardNames')
