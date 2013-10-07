# Startup routine for Python from Xanadu
'Run this to load Xanadu data and variables'
import numpy as nm
import array
import struct
import pylab
from datetime import datetime, date, time
numvars = 4
Date = 111117
DLEN = 16801
Var = nm.empty([5, 16801], 'f')
Minutes = 280
VarName = [None]*5
T = nm.empty (4, 'i')
Time = nm.empty (16801, 'f')
Tsec = nm.empty (16801, 'f')
DataFile = open('/home/cooperw/Data/LAMS/DATLF5', 'rb')
VarName[1] = "ATX"
ATX=nm.empty (DLEN, 'f')
VarName[2] = "ATLAMS"
ATLAMS=nm.empty (DLEN, 'f')
VarName[3] = "ATW"
ATW=nm.empty (DLEN, 'f')
VarName[4] = "PCORR"
PCORR=nm.empty (DLEN, 'f')
VarName[0] = 'Tsec'
Var = array.array ('f')
numvars += 1
TM = []
for i in range (0, DLEN):
    B = DataFile.read (16)
    (T[0],) = struct.unpack ('b', B[0:1])
    (T[1],) = struct.unpack ('b', B[4:5])
    (T[2],) = struct.unpack ('b', B[8:9])
    (T[3],) = struct.unpack ('b', B[12:13])
    Tsec[i] = T[0]*3600+T[1]*60+T[2]+T[3]/1000.
    Time[i] = T[0]*10000+T[1]*100+T[2]+T[3]/1000.
    if T[0] > 23:
        T[0] -= 24
    TM.insert(i, time(T[0],T[1],T[2],T[3]/1000))
    Var.insert (numvars*i, Time[i])
    Var.read (DataFile, numvars-1)
    ATX[i] = Var[1+numvars*i]
    ATLAMS[i] = Var[2+numvars*i]
    ATW[i] = Var[3+numvars*i]
    PCORR[i] = Var[4+numvars*i]
ATX = nm.ma.masked_where (ATX == -32767., ATX)
ATLAMS = nm.ma.masked_where (ATLAMS == -32767., ATLAMS)
ATW = nm.ma.masked_where (ATW == -32767., ATW)
ATW -= 1.0
PCORR = nm.ma.masked_where (PCORR == -32767., PCORR)
indx = nm.arange(0,DLEN)
# this strange way of writing I1=0 is for easy change to a specified time
I1 = int(indx[Time==Time[0]])
I2 = int(indx[Time==Time[DLEN-1]])
nticks = I2 - I1 + 1
multiplier = 1
if nticks > 180.:
    nticks /= 60
    multiplier = 60
if nticks > 180.:
    nticks /= 60
    multiplier *= 60
if nticks > 20:
    nticks /= 2
    multiplier *= 2
if nticks > 20:
    nticks /= 2
    multiplier *= 2
if nticks > 20:
    nticks /= 3
    multiplier *= 3
offset_index = int(((int(Tsec[I1]/multiplier)+1)*multiplier-Tsec[I1])%multiplier)
ATM=nm.empty (DLEN, 'f')
II = 5
for i in range(I1+II,I2-II):
    ATM[i] = 0.
    for j in range(i-II,i+II+1):
        ATM[i] += ATLAMS[j]
    ATM[i] /= 2.*II+1
for i in range(I1,I1+II):
    ATM[i] = ATLAMS[i]
for i in range(I2-II,I2):
    ATM[i] = ATLAMS[i]
pylab.figure (4)
pylab.clf ()      
A = pylab.subplot(211)
pylab.plot(Tsec[I1:I2], ATM[I1:I2])
pylab.plot(Tsec[I1:I2], ATW[I1:I2],color='red',lw=2)
pticks = []
for i in range(0,(nticks+1)):
    if I1+offset_index+i*multiplier < DLEN:
        pticks.append(Tsec[I1+offset_index+i*multiplier])
A.xaxis.set_ticks(pticks)
pticklabels = []
for i in range(0,(nticks+1)):
    if I1+offset_index+i*multiplier < DLEN:
        pticklabels.append('')
A.xaxis.set_ticklabels(pticklabels)
yticks = [-20,'',-10,'',0,'',10]
A.yaxis.set_ticklabels(yticks)
for tick in A.yaxis.get_major_ticks():
    tick.label.set_fontsize(14)
pylab.ylabel('Temperature [$^\circ$C]', color='blue',fontsize=15)
#pylab.grid(True)
A2 = pylab.subplot(212)
pylab.plot(Tsec[I1:I2], ATLAMS[I1:I2],color='blue')
pylab.plot(Tsec[I1:I2], ATW[I1:I2],color='red',lw=2)
pticks = []
for i in range(0,(nticks+1)):
    if I1+offset_index+i*multiplier < DLEN:
        pticks.append(Tsec[I1+offset_index+i*multiplier])
A2.xaxis.set_ticks(pticks)
for tick in A2.xaxis.get_major_ticks():
    tick.label.set_fontsize(14) 
pticklabels = []
for i in range(0,(nticks+1)):
    if I1+offset_index+i*multiplier < DLEN:
        pticklabels.append(TM[I1+offset_index+i*multiplier].strftime('%H%M'))
A2.xaxis.set_ticklabels(pticklabels)
yticks = ['','',-10,'',0,'',10,'',20]
A2.yaxis.set_ticklabels(yticks)
for tick in A2.yaxis.get_major_ticks():
    tick.label.set_fontsize(14) 
pylab.xlabel('Time [UTC]',fontsize=15)
pylab.ylabel('Temperature [$^\circ$C]', color='blue',fontsize=15)
pylab.savefig('Fig5')
#pylab.grid(True)
pylab.show()
#
#print 'Loading variables into a Python session'
