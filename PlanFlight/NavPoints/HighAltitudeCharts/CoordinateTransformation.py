# -*- coding: utf-8 -*-
"""
Spyder Editor

This temporary script file is located here:
/home/cooperw/.spyder2/.temp.py
"""

import wx
import matplotlib
import numpy
import pylab
# This program calculates the transformation from cursor coordinates to
# lat/lon for the enroute high-altitude charts. The charts themselves have
# been processed by 'gimp' to reduce shear and 'parallax' as present in the
# original maps, so that the maps are closer to square in lat-lon, but some
# distortion is still present. The maps were obtained from the FAA site;
# search for 'IFR enroute charts' because the actual URL changes when the
# maps are updated. The versions used are due for updating on May 31 2012.
# See the notes on this transformation as coded here. To use this routine,
# enable the display in PlanFlight.py, in 'OnMove', that changes the normal
# lon-lat display to show cursor coordinates. Then display the map as it
# will be shown, and determine the cursor coordinates for a set of 10 or
# so points placed over the range of the map (e.g., the intersections of
# the lat-lon lines on the map) and record those in an ascii file. Change
# the next program line to reference that file, using the format of the 
# existing ones as a template. Then run the program, check the listing to
# ensure that the points in the file are reproduced correctly by the
# transformation that results, then encode the fit coefficients into the
# code with an appropriate branch on the selected map.
MouseFile = open ('./cal.AL', 'r')
x=[];y=[];g=[];t=[]
for line in MouseFile:
    line = line.replace('\n','')
    a = line.split(',')
    print 'a=',a
    x.append (float(a[1]))
    y.append (float(a[2]))
    t.append (float(a[3]))
    g.append (float(a[4]))
sumx=0.
sumy=0.
sumxy=0.
sumx2=0.
sumy2=0.
sumg=0.
sumt=0.
sumgx=0.
sumgy=0.
sumtx=0.
sumty=0.
n=0
for i in range(len(x)):
    sumx += x[i]
    sumy += y[i]
    sumxy += x[i]*y[i]
    sumx2 += x[i]*x[i]
    sumy2 += y[i]*y[i]
    sumg += g[i]
    sumt += t[i]
    sumgx += g[i]*x[i]
    sumgy += g[i]*y[i]
    sumtx += t[i]*x[i]
    sumty += t[i]*y[i]
    n += 1.
sumx /= n
sumy /= n
sumxy /= n
sumx2 /= n
sumy2 /= n
sumg /= n
sumt /= n
sumgx /= n
sumgy /= n
sumtx /= n
sumty /= n
# see 5/14 notes for equations
sigmax2=sumx2-sumx*sumx
sigmay2=sumy2-sumy*sumy
sigmaxy2=sumxy-sumx*sumy
sigmaty2=sumty-sumt*sumy
sigmatx2=sumtx-sumt*sumx
sigmagx2=sumgx-sumg*sumx
sigmagy2=sumgy-sumg*sumy
a12 = (sigmagy2*sigmax2 - sigmagx2*sigmaxy2) \
    / (sigmax2*sigmay2 - sigmaxy2*sigmaxy2)
a11 = (sigmagx2 - a12 * sigmaxy2) / sigmax2
gzero = sumg-a11*sumx-a12*sumy
a22 = (sigmaty2*sigmax2 - sigmatx2*sigmaxy2) \
    / (sigmax2*sigmay2 - sigmaxy2*sigmaxy2)
a21 = ((sigmatx2 - a22 * sigmaxy2) / sigmax2)    
tzero = sumt-a21*sumx-a22*sumy
print a11,a12,a21,a22,gzero,tzero
for i in range(len(x)):
    lon =gzero + a11 * x[i] + a12 * y[i]
    lat = tzero + a21 * x[i] + a22 * y[i]
    print 'counts %.0f %.0f lat/lon = %.2f %.2f transformed %.2f %.2f' % \
          (x[i], y[i], t[i], g[i], lat, lon)