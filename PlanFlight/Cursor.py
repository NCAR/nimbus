#!/usr/bin/python
# file Cursor.py
"This is an example copied in order to read the cursor position -- not used now."

import wx

class MyFrame(wx.Frame):

    def __init__(self):
        wx.Frame.__init__(self, None, -1, "My Frame", size=(500, 500))
        panel = wx.Panel(self, -1)
#       self.png = wx.StaticBitmap(self, -1, wx.Bitmap("./Airways.png", wx.BITMAP_TYPE_ANY))
        png = wx.Image('./Airways.png', wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        self.BM = wx.StaticBitmap(panel, -1, png, (10, 40), (png.GetWidth(), png.GetHeight()))
        self.BM.Bind(wx.EVT_MOTION,  self.OnMove)
        self.BM.Bind(wx.EVT_LEFT_DOWN, self.OnBitmapLeftDown)
        wx.StaticText(panel, -1, "Pos:", pos=(10, 5))
        self.posCtrl = wx.TextCtrl(panel, -1, "", pos=(40, 10))

    def OnMove(self, event):
        pos = event.GetPosition()
        self.posCtrl.SetValue("%s, %s" % (pos.x, pos.y))

    def OnBitmapLeftDown(self, event):
        print 'pressed...' 
        b = self.posCtrl.GetValue()
        print b
        a = b.split(',')
	# now convert to long/lat:
        x0 = -105.35324 
        y0 =  41.51483 
        a11 =  0.00671 
        a12 =   0.00031 
        a21 =  0.00004 
        a22 =  -0.00507
        lg = x0 + a11*float(a[0]) + a12*float(a[1])
        lt = y0 + a21*float(a[0]) + a22*float(a[1])
        print 'clicked on ', format(lg, '.2f'), ' ', format(lt, '.2f')
 
app = wx.PySimpleApp()
frame = MyFrame()
frame.Show(True)
app.MainLoop()
