#Julian Quick
#This function inputs a singnal name, a boolean variable 
#denoting wheter the signal is to be commented or uncommented, and 'self'.
#A signal will only be commented if it is above the ========= line in groundvars
from getGroundInfo import getInfo
from PyQt4 import QtGui, QtCore
import fileinput
import sys
def addit(signame,inDB,self):
    from generateTheButtons import generateButtons
    pastPoint=False
    filename='groundvars'
    notused,groundlines=getInfo()

#------------------------------------------------------
    if inDB==True:#commenting
        for line in fileinput.input(filename,inplace=1):
            if signame[1:] in line and pastPoint==False and line[0]!='#':
                 line=line.replace(line,'#'+line)
            if line.startswith('#============='):
                 pastPoint=True
            sys.stdout.write(line)
        fileinput.close()
#-----------------------------------------------------
    if inDB==False:#uncommenting
        for line in fileinput.input(filename,inplace=1):
            if signame in line and line[0]=='#':
                 line=line.replace(line,line[1:])
            sys.stdout.write(line)
        fileinput.close()
    generateButtons(self)
