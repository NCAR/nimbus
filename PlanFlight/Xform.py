#!/usr/bin/python
# Xform.py
# given 3 measured pairs of coordinates (x,y) and (i,j), find transformation such that
# where (x,y) = (long,lat) and (i,j) are cursor coordinates on the screen, find solution
# such that:
#     long = x0 + a11 * i + a12 * j
#     lat  = y0 + a21 * i + a22 * j
import math
x1=-104.
y1 = 38.
x2 = -98.
y2 = 41.
x3 = -104.
y3 = 41.
i1 = 170.
j1 = 694.
i2 = 1091.
j2 = 110.
i3 = 197.
j3 = 103.
a11 = ((j1-j2)*(x1-x3)-(j1-j3)*(x1-x2))/(j1*(i2-i3)+j2*(i3-i1)+j3*(i1-i2))
a22 = ((i1-i2)*(y1-y3)-(i1-i3)*(y1-y2))/(i1*(j2-j3)+i2*(j3-j1)+i3*(j1-j2))
a12 = (x1-x2-a11*(i1-i2))/(j1-j2)
a21 = (y1-y2-a22*(j1-j2))/(i1-i2)
x0 = x1 - a11*i1-a12*j1
y0 = y1 - a21*i1-a22*j1
print format(x0,'.5f'),\
      ', ', format(y0,'.5f'),\
      ', ', format(a11,'.5f'),\
      ', ', format(a12,'.5f'),\
      ', ', format(a21,'.5f'),\
      ', ', format(a22,'.5f')
