#!/usr/bin/python
# file: PlanFlight.py
"PlanFlight Controller, main program. All the UI is in this module."
'2012 Copyright University Corporation for Atmospheric Research'
# Other modules are:
#   Specs: specifications like flight speed, airport location, wind, etc.
#   ModuleConstructor: builds modules and adds them to the track
#   KML: translates the coordinates to kml for, e.g., in GoogleEarth`
#   Print: used for printing

#   Target formatting: (not followed with full consistency)
#     space surrounding math operations +, -, *, /, except in cases where
#         the more compact format without spaces seems to clarify what
#         the formula is (e.g., for (2.*PI, etc -- 2.*PI is more a 
#         single entity).  Also, usually no space for formulas that 
#	  calculate indices or spatial offsets in displays, etc.
#     space after commas separating sequences of arguments to functions
#     no space for commas in tuples -- seems clearer that way
#     space before ( for functions
#     no space separating lists, dictionaries, etc, from indices: M[i]
#     target line length < 73
#     no tabs in code lines, but tabs used freely in comment lines
import wx
import os
import sys
import pickle
import string
from Frame import frame 	# this makes 'frame' accessible in other
import Specs			# must follow Frame import
from Specs import *
import pdb
import matplotlib # http://matplotlib.sourceforge.net/
matplotlib.use('WXAgg')
import pylab
import numpy
import KML
from KML import *
from Nearest import Nearest
import ModuleConstructor
#from ModuleConstructor import *
from Printer import Printer
				# modules

# the following are made global for easy dismissal in callback functions
frame2 = None
frame3 = None
frame4 = None
frame5 = None
frame6 = None
frame7 = None
frame8 = None
frame9 = None
frame10 = None
frame11 = None
frame12 = None
frame13 = None

# the following are overridden by the contents of 'Standard.nav' 
# if it is present:
NavItems = {'RAF': (-105.138988, 39.913041, 0., 0.),
            'SLN': (-97.621361, 38.925139, 0., 0.)}

# Entry of lat/lon can be in two formats:
# Decimal degrees or integer degrees and decimal minutes.
# 'DegMin' functions refer to this choice
DefaultDegMin = True			# default to int deg / dec. min

# ids for event handlers (many not used; list could be pruned):
ID_ABOUT = 101
ID_OPEN  = 102
ID_NEW   = 103
ID_SAVE  = 104
ID_PRNT  = 105
ID_EXIT  = 110
ID_SHOW  = 111
ID_EDIT  = 112
ID_GET   = 113
ID_IC    = 120
ID_TR    = 121
ID_OB    = 122
ID_VS    = 123
ID_LM    = 124
ID_RT    = 125
ID_SP    = 126
ID_DA    = 127
ID_MD    = 130
ID_SV    = 131
ID_SA    = 132
ID_ST    = 501
ID_SM    = 502
ID_CM    = 503
ID_AM    = 504
ID_SCT   = 505
ID_SE    = 506
ID_SZ    = 507
ID_LG    = 508
ID_LT    = 509
ID_Z     = 510
ID_OR    = 511
ID_LGS   = 512
ID_OTH   = 513
ID_C2    = 514
ID_GE    = 515
ID_OT    = 516
ID_SC    = 517
ID_RO    = 518
ID_RC    = 519
ID_PT    = 520
ID_FL    = 521
ID_IS    = 522
ID_DX    = 523
ID_CY    = 524
ID_RM    = 525
ID_RX    = 526
ID_MP    = 527
ID_VD    = 528
ID_CK    = 529
ID_NV    = 530
ID_CA    = 531
ID_LN    = 532
ID_LF    = 533
ID_S3    = 534
ID_AN    = 535
ID_AB    = 536
ID_LT2   = 537
ID_LG2   = 538
ID_ID    = 539
ID_FD    = 540
ID_CK2   = 541
ID_HP    = 542
ID_PT8   = 543
ID_PT2   = 544
ID_FL2   = 545
ID_PT9   = 546
ID_HINT1 = 547
ID_DF    = 548
ID_SR    = 549
ID_SND   = 550
ID_PT11  = 551
ID_CP    = 552
ID_DP    = 553
ID_TAS   = 554
ID_APPEND= 555
ID_RF    = 556
ID_XX    = 558
IDB      = 1000

# The routine has these classes defining frames:
#    MainWindow     (frame):  	The main user-interface window
#    SpecsFrame    (frame2): 	For display and editing of Specs
#    TrackFrame    (frame3): 	Displays the track definition
#    WaypointFrame (frame4): 	Listing of waypoints
#    ReorderFrame  (frame5): 	Supports reordering of modules
#    MapFrame      (frame6): 	Displays map and reads cursor
#    NavFrame      (frame7): 	The navigation points    
#    HelpFrame     (frame8): 	Window for displaying Help information
#    WizardFrame   (frame9): 	Window for displaying context hints
#    SRFrame       (frame10): 	Chart that shows specific range
#    SNDFrame      (frame11):   Display the sounding
#    DLGFrame      (frame12):   Info for Performance displays
#    PlotFrame     (frame13):   Frame for display of track plot
#
#    Most of the code is in MainWindow

class NavFrame (wx.Frame):
    def __init__(self):
        'Displays a frame with navigation points for use when defining'\
         + ' coordinates.'
        wx.Frame.__init__(self, None, -1, 'Navigation Points', \
                          size = (450,500))
        scrollWin = wx.PyScrolledWindow (self, -1)
#	use a panel to be able to tab between entries
#       panel = wx.Panel (scrollWin, -1)
#	(didn't work to put everything on the panel instead
#			 of on the scrollWin... so tabs don't work)
        wx.Button (scrollWin, ID_LN, 'Load Nearby', (5,5), (95,32))
        wx.Button (scrollWin, ID_CA, 'Clear All', (100,5), (75,32))
        wx.Button (scrollWin, ID_RF, "Refresh", (190,5), (75,32))
        self.DegMin = wx.CheckBox (scrollWin, ID_CK2, 'deg min format',\
                                   (290,5), size = (135,40))
        self.DegMin.SetToolTip(wx.ToolTip \
           ('enter lat/lon as int degrees and decimal minutes?\n'\
            + '(If unchecked, lat/lon expected in decimal degrees)'))
        self.DegMin.SetValue (DefaultDegMin)
        self.Bind (wx.EVT_CHECKBOX, self.onCheckBoxChange, id=ID_CK2) 
        ButtonAddNav = wx.Button (scrollWin, ID_AB, 'Add', (5,40),\
                                  size=(45,32))
        ButtonAddNav.SetBackgroundColour ('Light Grey')
        self.Bind (wx.EVT_BUTTON, self.onAdd, id=ID_AB)
        self.Bind (wx.EVT_BUTTON, self.OnRefresh, id=ID_RF)
        self.InputID = wx.TextCtrl (scrollWin, ID_ID, 'BJC', (50,40), \
                                    size=(60,32))
        fmt = self.DegMin.GetValue ()
        if fmt:
            degree, minute = ConvertToDegMin \
                             (Specs.TakeoffLocation ()[1])
            slt = format (degree, '.0f') + '  ' + format (minute, '.1f')
            degree, minute = ConvertToDegMin \
                             (Specs.TakeoffLocation ()[0])
            slg = format (degree, '.0f') + '  ' + format (minute, '.1f')
        else:
            slt = format (Specs.TakeoffLocation ()[1], '.3f')
            slg = format (Specs.TakeoffLocation ()[0], '.3f')
        self.InputLT = wx.TextCtrl (scrollWin, ID_LT2, slt, (110,40), \
                                    size = (90,32))
        self.InputLG = wx.TextCtrl (scrollWin, ID_LG2, slg, (200,40), \
                                    size = (90,32))
        ButtonFindNav = wx.Button (scrollWin, ID_FD, 'Find', (290,40),\
                                   size = (50,32))
        ButtonFindNav.SetBackgroundColour ('Light Grey')
        self.Bind (wx.EVT_BUTTON, self.onFind, id=ID_FD)
# now create line for each point in NavItems{}
        Keys = NavItems.keys ()
        Keys.sort ()
        self.Keys = Keys	#attach it for use in checkbox callback
        nb = 0
#				Allow for a series of nav buttons
        Buttons = []
        Lats = []
        Lons = []
        DelButtons = []
        for Ky in Keys:
            ll = NavItems[Ky]
            nb += 1
            yy = 40 * (nb + 1)
            Buttons.append (wx.Button (scrollWin, IDB + 4 * nb, Ky, \
                                      (5, yy), size = (70,32)))
            Buttons[nb-1].SetBackgroundColour ('Light Grey')
            if fmt:
                degree, minute = ConvertToDegMin (ll[1])
                slt = format (degree, '.0f') + '  ' \
                    + format (minute, '.1f')
                degree, minute = ConvertToDegMin (ll[0])
                slg = format (degree, '.0f') + '  ' \
                    + format (minute, '.1f')
            else:
                slt = format (ll[1], '.3f')
                slg = format (ll[0], '.3f')
            Lats.append (wx.TextCtrl (scrollWin, IDB+4*nb+1, slt, \
                                     (75, yy), (90,32)))
            Lons.append (wx.TextCtrl (scrollWin, IDB+4*nb+2, slg, \
                                     (165, yy), (90,32)))
            DelButtons.append (wx.Button (scrollWin, IDB+4*nb+3, \
                               'Remove', (255,yy), size=(70,32)))
            DelButtons[nb-1].SetBackgroundColour ('Light Grey')
            self.Bind (wx.EVT_BUTTON, self.onRemove, id = (IDB+4*nb+3))
            self.Bind (wx.EVT_BUTTON, self.onTransfer, id = (IDB+4*nb))
            lbl = ('%.0f/%.1f') % (ll[2], ll[3])
            wx.StaticText (scrollWin, -1, lbl, pos=(330,yy))
        self.Buttons = Buttons
        self.Lats = Lats
        self.Lons = Lons
        self.DelButtons = DelButtons
        listing = ' '
        stTxt = wx.StaticText (scrollWin, -1, listing, pos = (20,20))
        w, h = stTxt.GetSize ()
        h = nb * 40 + 80
        scrollWin.SetScrollbars (0, 12, 0, h/12+2)
        scrollWin.SetScrollRate (1,12)
        self.Bind (wx.EVT_BUTTON, self.onClearAll, id = ID_CA)
        self.Bind (wx.EVT_BUTTON, self.onLoadNearby, id = ID_LN)
        self.Bind (wx.EVT_CONTEXT_MENU, self.OnHint)
    #end __init__ def

#   event handlers for NavFrame:
    def onClearAll (self, event): 
        "Reset the list of nav points to have just RAF"
        global NavItems, frame7
        NavItems = {'RAF': (-105.138988, 39.913041, 0., 0.)}
        frame7.Close ()
        frame7 = NavFrame ()
        frame7.Show ()

    def onRemove (self, event):
        "Remove the nav point from NavItems and from NavPoints window."
        global frame7, NavItems
        i = (event.GetId () - IDB - 3) / 4 - 1
        ID = self.Buttons [i].GetLabel ()
        del NavItems[ID]
        frame7.Close ()
        frame7 = NavFrame ()
        frame7.Show ()

    def onTransfer (self, event):
        "Transfer a lat/lon on NavPoints window to Module Specs window."
        global frame
        i = (event.GetId () - IDB) / 4 - 1
        ID = self.Buttons[i].GetLabel ()
        lg,lt,a,d = NavItems [ID]
#       now transfer to Module-definition window:
        if frame.DegMin.GetValue ():	# deg-min section
            degrees, minutes = ConvertToDegMin (lg)
            slg = format (degrees, 'd') + '  ' + format (minutes, '.1f')
            degrees, minutes = ConvertToDegMin (lt)
            slt = format (degrees, 'd') + '  ' + format (minutes, '.1f')
        else:
            slg = format (lg, '.3f')
            slt = format (lt, '.3f')
        frame.InputLG.SetValue (slg)
        frame.InputLT.SetValue (slt)
        
    def OnRefresh (self, event):
        'Refresh the display of bearing/distance to reference point.'
                    # retrieve the lat/lon displayed in the MainWindow
        global frame7, NavItems        
        lg,lt = frame.GetLatLonValues ()
        Dmin, Best, BestAz, BestLG, BestLT = Nearest (lt, lg)	
        Keys = NavItems.keys ()
        for ky in Keys:
            NavItems[ky] = (NavItems[ky][0], NavItems[ky][1], -1., -1.)
            for i in range (len(Best)):
                if ky == Best[i]:
                    NavItems[ky] = (NavItems[ky][0], NavItems[ky][1],\
                                    BestAz[i], Dmin[i])
                #end of test for match with ky
            #end of i loop
        #end of ky loop
        # now refresh window
        frame7.Close ()
        frame7 = NavFrame ()
        frame7.Show ()
        
        
    def onLoadNearby (self, event):
        'Runs Nearest and reads its output, then makes new '\
         +'entries in Navigation-Points window.'
        global NavItems, frame, frame7
#			get lat/lon from Module Specs window:
#       frame.DegMin.SetValue (not frame.DegMin.GetValue ())
        lg,lt = frame.GetLatLonValues ()
#       frame.DegMin.SetValue (not frame.DegMin.GetValue ())
        Dmin, Best, BestAz, BestLG, BestLT = Nearest (lt, lg)	
#       for line in f:
        for i in range (len (Best)):
            ID = Best[i]
            NavItems[ID] = (BestLG[i], BestLT[i], BestAz[i], Dmin[i])
        frame7.Close ()
        frame7 = NavFrame ()
        frame7.Show ()
        
    def onAdd (self, event):
        "Add nav point to navigation-window display."
        global DefaultDegMin, frame7
        self.DegMin.SetValue (not self.DegMin.GetValue ())
        lg,lt = self.GetLatLonValues (0)
        self.DegMin.SetValue (not self.DegMin.GetValue ())
        NavItems[self.InputID.GetValue ()] = (lg,lt,0.,0.)
        DefaultDegMin = self.DegMin.GetValue ()
        frame7.Close ()
        frame7 = NavFrame ()
        frame7.Show ()
            
    def onFind (self, event):
        "Find item matching ID box in database, load lat/lon."
        ID = self.InputID.GetValue ()
        VORfile = open ("./NavPoints/VOR.txt")
        for line in VORfile:
            if ID in line:		# found right line; get lat/lon
                ssplit = line.split (',')
                if ssplit[0] != ID: continue
                lg = float (ssplit[1])
                lt = float (ssplit[2])
                dlg = wx.MessageDialog (self,\
                'Found ' + ID + ' at lat/long of ' + format (lt, '.3f') \
                + ',' + format (lg, '.3f') + '\n'\
                'Full name is ' + ssplit[3] + '\n'\
                +'Click OK to use it, CANCEL to keep searching',\
                'Confirm Point', wx.OK|wx.CANCEL|wx.ICON_QUESTION)
                result = dlg.ShowModal ()
                dlg.Destroy ()
                if result == wx.ID_OK:
                    if self.DegMin.GetValue ():
                        degrees, minutes = ConvertToDegMin (lg)
                        slg = format (degrees, '.0f') + '  ' \
                            + format (minutes, '.1f')
                        degrees, minutes = ConvertToDegMin (lt)
                        slt = format (degrees, '.0f') + '  ' \
                            + format (minutes, '.1f')
                    else:                           # decimal deg section
                        slg = format (lg, '.3f')
                        slt = format (lt, '.3f')
                    self.InputLT.SetValue (slt)
                    self.InputLG.SetValue (slg)
                    VORfile.close ()
                    return ()
                # end of result==OK section
            # end of ID-in-line section
        # end of loop over VORfile lines; now do same for WPTS.txt
        VORfile.close ()
        WPTfile = open ("./NavPoints/WPTS.txt")
        for line in WPTfile:
            if ID in line:		# found right line; get lat/lon
                ssplit = line.split (',')
                if ssplit[0] != ID: continue
                lg = float (ssplit[1])
                lt = float (ssplit[2])
                dlg = wx.MessageDialog (self,\
                'Found ' + ID + ' at lat/long of ' + format (lt, '.3f')\
                + ',' + format (lg, '.3f') + '\n'\
                + 'Full name is ' + ssplit[3] + '\n'\
                + 'Click OK to use it, CANCEL to keep searching',\
                'Confirm Point', wx.OK|wx.CANCEL|wx.ICON_QUESTION)
                result = dlg.ShowModal ()
                dlg.Destroy ()
                if result == wx.ID_OK:
                    if self.DegMin.GetValue ():
                        degrees, minutes = ConvertToDegMin (lg)
                        slg = format (degrees, '.0f') + '  '\
                            +format (minutes, '.1f')
                        degrees, minutes = ConvertToDegMin (lt)
                        slt = format (degrees, '.0f') + '  '\
                            +format (minutes, '.1f')
                    else:                           # decimal deg section
                        slg = format (lg, '.3f')
                        slt = format (lt, '.3f')
                    self.InputLT.SetValue (slt)
                    self.InputLG.SetValue (slg)
                    WPTfile.close ()
                    return ()
                # end of result==OK section
            # end of ID-in-line section
        # end of loop over WPTfile lines
        WPTfile.close ()
# 		# only reach this point if no match:
        dlg = wx.MessageDialog (self,\
        'No match found for ' + ID, 'Search Error', \
        wx.OK|wx.ICON_INFORMATION)
        result = dlg.ShowModal ()
        dlg.Destroy ()

    def OnHint (self, event):
        "Display a window with context hints."
        global frame9
        CallingID = event.GetId ()
        if CallingID < 0:
            frame.SetStatusText ('Hints unavailable for this item', 2)
            return ()
        f = open ('./Hint.txt','rb')
        frame.SetStatusText ('See suggestions in the \'Hints\' window', 2)
        hint = 'Suggestions:\n\n\n'
        enable = False
        for line in f:
            if CallingID == ID_LN:
                if '**Load Nearby:**' in line:
                    enable = True
            if CallingID == ID_CA:
                if '**Clear All:**' in line:
                    enable = True
            if CallingID == ID_AB:
                if '**Add Button:**' in line:
                    enable = True
            if CallingID == ID_FD:
                if '**Find Button:**' in line:
                    enable = True
            if CallingID >= IDB:
                if '**Left Button and Remove Button:**' in line:
                    enable = True
            if '-----' in line: enable = False
            if enable: hint += line
        if frame9 != None and frame9: frame9.Close ()
        frame9 = WizardFrame (hint)
        frame9.Show ()

    def onCheckBoxChange (self, event):
        "When checkbox changes, change the displayed coordinates also."
        global frame
        fmt =  self.DegMin.GetValue ()
        lg,lt = self.GetLatLonValues (0)
        if fmt:
            degrees, minutes = ConvertToDegMin (lg)
            slg = format (degrees, '.0f') + '  ' \
                + format (minutes, '.1f')
            degrees, minutes = ConvertToDegMin (lt)
            slt = format (degrees, '.0f') + '  ' \
                + format (minutes, '.1f')
        else:				# decimal deg section
            slg = format (lg, '.3f')
            slt = format (lt, '.3f')
        self.InputLT.SetValue (slt)
        self.InputLG.SetValue (slg)
        nb = 0
        for Ky in self.Keys:
            nb += 1
            lg,lt = self.GetLatLonValues (nb)
            if fmt:
                degrees, minutes = ConvertToDegMin (lg)
                slg = format (degrees, '.0f') + '  ' \
                    + format (minutes, '.1f')
                degrees, minutes = ConvertToDegMin (lt)
                slt = format (degrees, '.0f') + '  ' \
                    + format (minutes, '.1f')
            else:				# decimal deg section
                slg = format (lg, '.3f')
                slt = format (lt, '.3f')
            self.Lats[nb-1].SetValue (slt)
            self.Lons[nb-1].SetValue (slg)

    def GetLatLonValues (self, nb):
        "Read lat/lon from window, convert if necessary from deg-min,'\
         + ' and return the current values in decimal-degree format."
#		if necessary, convert deg-min format to decimal-deg
        if nb == 0:
            if not self.DegMin.GetValue ():	# deg-min section
                slg = self.InputLG.GetValue ()
                slt = self.InputLT.GetValue ()
                slg = slg.replace (',', ' ')
                slt = slt.replace (',', ' ')
                ssplit = slg.split ()
                if len (ssplit) < 2: ssplit.append ('0.')
                lg = int (float (ssplit[0]))
                if lg < 0: lg -= float (ssplit[1]) / 60.
                else: lg += float (ssplit[1]) / 60.
                ssplit = slt.split ()
                if len (ssplit) < 2: ssplit.append ('0.')
                lt = int (float (ssplit[0]))
                if lt < 0: lt -= float (ssplit[1]) / 60.
                else: lt += float (ssplit[1]) / 60.
            else:
                lg = float (self.InputLG.GetValue ())
                lt = float (self.InputLT.GetValue ())
            return (lg,lt)
        else:
            if not self.DegMin.GetValue ():	# deg-min section
                slg = self.Lons[nb-1].GetValue ()
                slt = self.Lats[nb-1].GetValue ()
                ssplit = slg.split ()
                if len (ssplit) < 2: ssplit.append ('0.')
                lg = int (float (ssplit[0]))
                if lg < 0: lg -= float (ssplit[1]) / 60.
                else: lg += float (ssplit[1]) / 60.
                ssplit = slt.split ()
                if len (ssplit) < 2: ssplit.append ('0.')
                lt = int (float (ssplit[0]))
                if lt < 0: lt -= float (ssplit[1]) / 60.
                else: lt += float (ssplit[1]) / 60.
            else:
                lg = float (self.Lons[nb-1].GetValue ())
                lt = float (self.Lats[nb-1].GetValue ())
            return (lg, lt)

class WaypointFrame (wx.Frame):
    def __init__(self, listing):
        'Displays, with print and save buttons, the waypoints.'
        wx.Frame.__init__(self, None, -1, 'Waypoints', size = (550,400))
        scrollWin = wx.PyScrolledWindow (self, -1)
        wx.Button (scrollWin, ID_PT, 'Print', (140,8), (60,32))
        wx.Button (scrollWin, ID_FL, 'Save', (205,8), (55,32))
        wx.Button (scrollWin, ID_VD, 'List VOR/DME', (265,8), (115,32))
        stTxt = wx.StaticText (scrollWin, -1, listing, pos = (20,20))
        w, h = stTxt.GetSize ()
        scrollWin.SetScrollbars (0, 12, 0, h/12+2)
        scrollWin.SetScrollRate (1, 12)
        self.printer = Printer (self)
        self.listing = listing
        self.Bind (wx.EVT_BUTTON, frame.OnPrint, id = ID_PT)
        self.Bind (wx.EVT_BUTTON, frame.onFile, id = ID_FL)
        self.Bind (wx.EVT_BUTTON, self.onVORDME, id = ID_VD)
    #end __init__ def

# WaypointFrame event handlers:
    def onVORDME (self, event):
        "Produce listing with VOR/DME and nav-pt-relative coordinates."
        global frame4
#	first make a list of coordinates needed
        KeyList = ModuleConstructor.Track.keys ()
        KeyList.sort ()
        alast = {}
        clist = []
        KML.WaypointNumber = 0
        for ky in KeyList:
            if ('Module' in ky and 'Number' not in ky):
#               kk = int (ky.replace ('Module', ''))
                m = ModuleConstructor.Track[ky]
                KeyListManeuvers = m.keys ()
                KeyListManeuvers.sort ()
                for kym in KeyListManeuvers:
                    if 'Manvr' in kym:
#                       kk = int (kym.replace ('Manvr', ''))
                        mn = m[kym]
                                  #  skip turns
                        if 'Turn' not in mn['Type']:
                            al = mn['Anchor']
                            if (al != alast):
                                clist.append ((al[0],al[1]))
#                               convert to deg and decimal min:
                                KML.WaypointNumber += 1
                                alast = al
                            an = mn['EndPoint']
                            if (an != alast):
                                clist.append ((an[0],an[1]))
                                KML.WaypointNumber += 1
                                alast = an
                            # end of search for waypoints in maneuver
                        # end of branch for maneuver not Turn
                    # end of Maneuver procesing
                # end of loop over Maneuver keys
            # end of module processing
        # end of loop over Track keys
        listing = 'Waypoints:\n'
        lno = 0
        for al in clist:
            lno += 1
            listing  += ' WP ' + format (lno) + ' ' 
            d,m = ConvertToDegMin (al[1])
            listing += format(d, 'd') + ' ' + format (m, '.1f')
            d,m = ConvertToDegMin (al[0]) 
            listing += ', ' + format(d, 'd') + ' ' + format (m, '.1f')
            listing += ' is '
            Dmin, Best, BestAz, BestLG, BestLT = Nearest (al[1], al[0])
            listing += Best[0] + ':' + format (BestAz[0], '.0f') + '/' \
                      + format (Dmin[0], '.1f') + '; '
            listing += Best[1] + ':' + format (BestAz[1], '.0f') + '/' \
                      + format (Dmin[1], '.1f') + '; '
            listing += Best[4] + ':' + format (BestAz[4], '.0f') + '/' \
                      + format (Dmin[4], '.1f') + '; '
            listing += Best[5] + ':' + format (BestAz[5], '.0f') + '/' \
                      + format (Dmin[5], '.1f') + '\n'
        if frame4 != None and frame4: frame4.Close ()
        frame4 = WaypointFrame (listing)
        frame4.Show ()
    #end def VORDME 
# end WaypointFrame class

# the HelpFrame holds the display of the instructions file Help.txt
class HelpFrame (wx.Frame):
    def __init__(self, listing):
        'Displays information and instructions for the program.'
        wx.Frame.__init__(self, None, -1, 'Instructions', \
                          size = (550,500))
        scrollWin = wx.PyScrolledWindow (self, -1)
        wx.Button (scrollWin, ID_PT8, 'Print', (140,8), (60,32))
        stTxt = wx.StaticText (scrollWin, -1, listing, pos = (20,20))
        w, h = stTxt.GetSize ()
        scrollWin.SetScrollbars (0, 12, 0, h/12+2)
        scrollWin.SetScrollRate (1, 12)
        self.printer = Printer (self)
        self.listing = listing
        self.Bind (wx.EVT_BUTTON, frame.OnPrint, id = ID_PT8)
    #end __init__ def
    # no event handlers; OnPrint is in MainWindow
#end HelpFrame class

# the SNDFrame lists the contents of the sounding file
class SNDFrame (wx.Frame):
    def __init__(self, listing):
        'Displays a listing of the sounding in use.'
        wx.Frame.__init__(self, None, -1, 'Sounding', \
                          size = (600,500))
        scrollWin = wx.PyScrolledWindow (self, -1)
        wx.Button (scrollWin, ID_PT8, 'Print', (240,8), (60,32))
        stTxt = wx.StaticText (scrollWin, -1, listing, pos = (20,20))
        fontxx = wx.Font (10, wx.TELETYPE, wx.NORMAL,wx.NORMAL)
        stTxt.SetFont (fontxx)
        w, h = stTxt.GetSize ()
        scrollWin.SetScrollbars (0, 12, 0, h/12+2)
        scrollWin.SetScrollRate (1, 12)
        self.printer = Printer (self)
        self.listing = listing
        self.Bind (wx.EVT_BUTTON, frame.OnPrint, id = ID_PT11)
    #end __init__ def
    # no event handlers; OnPrint is in MainWindow
#end HelpFrame class

# the WizardFrame holds short descriptions for display as Context
class WizardFrame (wx.Frame):
    def __init__(self, hint):
        'Displays context help for the selection.'
        wx.Frame.__init__(self, None, -1, 'Hints', \
                          pos = (5,300), size = (550,300))
        scrollWin = wx.PyScrolledWindow (self, -1)
        wx.Button (scrollWin, ID_PT9, 'Print', (140,8), (60,32))
        stTxt = wx.StaticText (scrollWin, -1, hint, pos = (20,20))
        w, h = stTxt.GetSize ()
        scrollWin.SetScrollbars (0, 12, 0, h/12+2)
        scrollWin.SetScrollRate (1, 12)
        self.printer = Printer (self)
        self.hint = hint
        self.Bind (wx.EVT_BUTTON, frame.OnPrint, id = ID_PT9)
    #end __init__ def
    # no event handlers; OnPrint is in MainWindow
#end WizardFrame class

class ReorderFrame (wx.Frame):
    def __init__(self):
        'Displays a list of modules with entries to reorder them.'
        wx.Frame.__init__ (self, None, -1, 'Reorder Modules', \
                           size = (280,400))
        panel = wx.Panel (self, -1)
        KeyList = ModuleConstructor.Track.keys ()
        KeyList.sort ()
        newOrder = []
        label = []
        last = 0
        for ky in KeyList:
            if 'Module' in ky and 'Number' not in ky:
                m = ModuleConstructor.Track[ky]
                kk = int (ky.replace ('Module', ''))
                label.append (wx.StaticText (panel, -1, \
                             '   ' + format (kk, 'd') + ': '\
                                   + m['Type'], \
                             size = (130,32)))
                newOrder.append (wx.TextCtrl (panel, -1, str (kk), \
                                size = (20,32)))
                last = kk
            # end of Module processing
        # end of key loop
        self.Order = newOrder
        B1 = wx.Button (panel, ID_RM, 'Reorder Modules', size = (135,32))
        B2 = wx.Button (panel, ID_RX, 'Return w/o Saving', \
                        size = (135,32))
        self.Bind (wx.EVT_BUTTON, self.SaveConfig, id = ID_RM)
        self.Bind (wx.EVT_BUTTON, self.DismissConfig, id = ID_RX)
        mlist = range (0, 2 * last)
        for kk in range (0, last):
            mlist[2*kk] = label[kk]
            mlist[2*kk+1] = self.Order[kk]
        mlist.append (B1)
        mlist.append (B2)
#       print mlist
        sizer = wx.FlexGridSizer (cols = 2, hgap = 6, vgap = 6)
        sizer.AddMany (mlist)
        panel.SetSizer (sizer)

    def DismissConfig (self, event):
        "Close the Reorder frame."
        self.Destroy ()

    def SaveConfig (self, event): 
        "Save the Track in the redefined order."
        import copy
        newOrder = self.Order
        newTrack = copy.deepcopy (ModuleConstructor.Track)
        for k in range (0, len (newOrder)):
            newOrder[k] = int (self.Order[k].GetValue ())
            ky = 'Module'+format (k+1, '03d')
            newky = 'Module'+format (newOrder[k], '03d')
            md = ModuleConstructor.Track[ky]
            del newTrack[newky]
            newTrack[newky] = md
#           print ky," becomes ", newky
        ModuleConstructor.Track = newTrack
        Recalculate ()
        self.Destroy ()

class MapFrame (wx.Frame):
    def __init__(self):
        'Display maps with cursor that can read coordinates from map '\
         + 'for module definition.'
        wx.Frame.__init__(self, None, -1, 'High Altitude Chart',\
                          size = (450,450))
        panel = wx.Panel (self, -1)
        png = wx.Image ('./Airways.png', \
                        wx.BITMAP_TYPE_ANY).ConvertToBitmap ()
        self.BM = wx.StaticBitmap (panel, -1, png, (10,50), \
                                  (png.GetWidth (), png.GetHeight ()))
        self.BM.Bind (wx.EVT_MOTION,  self.OnMove)
        self.BM.Bind (wx.EVT_LEFT_DOWN, self.OnBitmapLeftDown)
        wx.StaticText (panel, -1, "Position: ", pos = (16,5))
        self.posCtrl = wx.TextCtrl (panel, -1, "", size = (130,32), \
                                   pos = (70,5))
        wx.StaticText (panel, -1, \
        "Click point to transfer coordinates \nto long/lat for module",\
        pos = (200,2))

    def OnMove (self, event):
        'Provides continuous display of the long/lat coordinates of '\
         + 'the cursor.'
        pos =event.GetPosition ()
        a = (pos.x,pos.y)
#       use the next line to display the integer cursor coordinates
#       self.posCtrl.SetValue ("%s, %s" % (pos.x, pos.y))	
# now convert to long/lat:
# these transformation coordinates are determined
# with the help of Xform.py
        x0 = -105.35324
        y0 =  41.51483
        a11 =  0.00671
        a12 =   0.00031
        a21 =  0.00004
        a22 =  -0.00507
        lg = x0 + a11 * float (a[0]) + a12 * float (a[1])
        lt = y0 + a21 * float (a[0]) + a22 * float (a[1])
        dp = format (lg, '.2f') + ',' + format (lt, '.2f')
        self.posCtrl.SetValue (dp)

    def OnBitmapLeftDown (self, event):
        'On mouse click, transfer the lon/lat coordinates to the '\
         + 'module-definition window'
        global frame
#       b = self.posCtrl.GetValue ()
        b = event.GetPosition ()
        a = (b.x,b.y)
# now convert to long/lat:
        x0 = -105.35324
        y0 =  41.51483
        a11 =  0.00671
        a12 =   0.00031
        a21 =  0.00004
        a22 =  -0.00507
        lg = x0 + a11 * float (a[0]) + a12 * float (a[1])
        lt = y0 + a21 * float (a[0]) + a22 * float (a[1])
        MainWindow.SetStatusText (frame, 'clicked on ' \
                                 + format (lg, '.2f') + ', ' \
                                 + format (lt, '.2f'), 2)
#       now transfer to Module-definition window:
        if frame.DegMin.GetValue ():	# deg-min section
            degrees, minutes = ConvertToDegMin (lg)
            slg = format (degrees, 'd') + '  ' + format (minutes, '.1f')
            degrees, minutes = ConvertToDegMin (lt)
            slt = format (degrees, 'd') + '  ' + format (minutes, '.1f')
        else:
            slg = format (lg, '.3f')
            slt = format (lt, '.3f')
        frame.InputLG.SetValue (slg)
        frame.InputLT.SetValue (slt)
    #end __init__ def
#end MapFrame class    

class PlotFrame (wx.Frame):
    def __init__(self):
        'Display the current track in a lat-lon plot'
        wx.Frame.__init__(self, None, -1, 'Planned Track',\
                          size = (750,650))
        panel = wx.Panel (self, -1)
        png = wx.Image ('./Plan.png', \
                        wx.BITMAP_TYPE_ANY).ConvertToBitmap ()
        self.BM = wx.StaticBitmap (panel, -1, png, (10,50), \
                                  (png.GetWidth (), png.GetHeight ()))
        self.BM.Bind (wx.EVT_MOTION,  self.OnMove)
        self.BM.Bind (wx.EVT_LEFT_DOWN, self.OnBitmapLeftDown)
        wx.StaticText (panel, -1, "Position: ", pos = (16,5))
        self.posCtrl = wx.TextCtrl (panel, -1, "", size = (130,32), \
                                   pos = (70,5))
        wx.StaticText (panel, -1, \
        "Click point to transfer coordinates \nto long/lat for module",\
        pos = (200,15))
#       pdb.set_trace ()

    def OnMove (self, event):
        'Provides continuous display of the long/lat coordinates of '\
         + 'the cursor.'
        pos =event.GetPosition ()
        a = (pos.x,pos.y)
#       use the next line to display the integer cursor coordinates
#       self.posCtrl.SetValue ("%s, %s" % (pos.x, pos.y))	
# now convert to long/lat:
        lt = frame.yzero - (pos.y - 540.) * frame.ymag
        lg = frame.xzero + (pos.x - 100. ) * frame.xmag
# these transformation coordinates are determined
# with the help of Xform.py
#       x0 = -105.35324
#       y0 =  41.51483
#       a11 =  0.00671
#       a12 =   0.00031
#       a21 =  0.00004
#       a22 =  -0.00507
#       lg = x0 + a11 * float (a[0]) + a12 * float (a[1])
#       lt = y0 + a21 * float (a[0]) + a22 * float (a[1])
        dp = format (lt, '.2f') + ',' + format (lg, '.2f')
        self.posCtrl.SetValue (dp)

    def OnBitmapLeftDown (self, event):
        'On mouse click, transfer the lon/lat coordinates to the '\
         + 'module-definition window'
        global frame
#       b = self.posCtrl.GetValue ()
        pos = event.GetPosition ()
        a = (pos.x,pos.y)
# now convert to long/lat:
        lt = frame.yzero - (pos.y - 540.) * frame.ymag
        lg = frame.xzero + (pos.x - 100. ) * frame.xmag
        MainWindow.SetStatusText (frame, 'clicked on ' \
                                 + format (lg, '.2f') + ', ' \
                                 + format (lt, '.2f'), 2)
#       now transfer to Module-definition window:
        if frame.DegMin.GetValue ():	# deg-min section
            degrees, minutes = ConvertToDegMin (lg)
            slg = format (degrees, 'd') + '  ' + format (minutes, '.1f')
            degrees, minutes = ConvertToDegMin (lt)
            slt = format (degrees, 'd') + '  ' + format (minutes, '.1f')
        else:
            slg = format (lg, '.3f')
            slt = format (lt, '.3f')
        frame.InputLG.SetValue (slg)
        frame.InputLT.SetValue (slt)
    #end __init__ def
#end PlotFrame class    

class SRFrame (wx.Frame):
    def __init__(self):
        'Display a chart showing specific range vs GW, altitude, Mach '\
         + 'number.'
        wx.Frame.__init__(self, None, -1, 'N Mi per lb fuel',\
                          size = (900,900))
        panel = wx.Panel (self, -1)
        png = wx.Image ('./SR.png', \
                        wx.BITMAP_TYPE_ANY).ConvertToBitmap ()
        self.BM = wx.StaticBitmap (panel, -1, png, (5,5), \
                                  (png.GetWidth (), png.GetHeight ()))

    #end __init__ def
#end SRFrame class    
class CPFrame (wx.Frame):
    def __init__(self):
        'Display a plot of the GV climb profile '
        wx.Frame.__init__(self, None, -1, 'Climb Profile',\
                          size = (900,600))
        panel = wx.Panel (self, -1)
        png = wx.Image ('./ClimbProfile.png', \
                        wx.BITMAP_TYPE_ANY).ConvertToBitmap ()
        self.BM = wx.StaticBitmap (panel, -1, png, (5,5), \
                                  (png.GetWidth (), png.GetHeight ()))

    #end __init__ def
#end SRFrame class    

# Next frame enables display and editing of the 'Specs', which include
# the fuel use rate, flight speed, turn rate, airport location, wind, ...
# The window becomes visible via the 'Edit' item under the 'Specs' item.
# The structure enables use of arrays that include altitude dependence
# and perhaps time dependence, but these are temporarily implemented as
# constants. The structure is such that access to these values is 
# expected to be through functions, not the individual constants stored 
# here; i.e., use Specs.TakeoffLocation () rather than the individual 
# constants for Specs.Specs['Airport']'. Although these are the same 
# now, then may change as items like ['Wind'] are changed to functions 
# of altitude. 
class SpecsFrame (wx.Frame):
    def __init__(self):
        'Displays the specifications for track construction, with entry'\
         + ' items that support changing the specification.'
        wx.Frame.__init__ (self, None, -1, 'Specifications', \
                           size=(300,400))
        panel = wx.Panel (self, -1)
        label1 = wx.StaticText (panel, -1, "Airport Location:", \
                                size=(150,32))
        ap = Specs.TakeoffLocation ()
        self.lon = wx.TextCtrl (panel, -1, str (ap[0]), size = (75,32))
        self.lat = wx.TextCtrl (panel, -1, str (ap[1]), size = (75,32))
        self.alt = wx.TextCtrl (panel, -1, str (ap[2]), size = (75,32))
        label2 = wx.StaticText (panel, -1, "Flight Speed, kts:", \
                                size=(150,32))
        self.FlightSpeed = wx.TextCtrl (panel, -1, \
                                        str (Specs.FlightSpeed (ap[2])),\
                                        size = (75,32))
        label2a = wx.StaticText (panel, -1, "Max Turn Rate, deg/s:", \
                                 size=(150,32))
        self.TurnRate = wx.TextCtrl (panel, -1, \
                                     str (Specs.MaxTurnRate (ap[2])), \
                                     size = (75,32))
        label3 = wx.StaticText (panel, -1, "Wind Direction:", \
                                size=(150,32))
        self.WindDir = wx.TextCtrl (panel, -1, \
                                    str (Specs.Wind (ap[2],0.)[0]), \
                                    size = (75,32))
        label4 = wx.StaticText (panel, -1, "Wind Speed:", size=(150,32))
        self.WindSpd = wx.TextCtrl (panel, -1, \
                                    str (Specs.Wind (ap[2],0.)[1]), \
                                    size = (75,32))
        label5 = wx.StaticText (panel, -1, "Fuel Burn:", size=(150,32))
        self.fuelBurn = wx.TextCtrl (panel, -1, \
                                     str (Specs.FuelBurnRate (ap[2])), \
                                     size = (75,32))
        label6 = wx.StaticText (panel, -1, "Fuel Load:", size=(150,32))
        self.fuelLimit = wx.TextCtrl (panel, -1, \
                                      str (Specs.FuelUseLimit ()), \
                                      size = (75,32))
        label7 = wx.StaticText (panel, -1, "Diversion Factor:", \
                                size=(150,32))
        self.Extra = wx.TextCtrl (panel, -1, \
                                         str (Specs.Extra ()), \
                                         size = (75,32))
        B1 = wx.Button (panel, ID_CM, 'Save Configuration', \
                        size=(135,32))
        B2 = wx.Button (panel, ID_C2, 'Return w/o Saving', size=(135,32))
        self.Bind (wx.EVT_BUTTON, self.SaveConfig, id=ID_CM)
        self.Bind (wx.EVT_BUTTON, self.DismissConfig, id=ID_C2)
        sizer = wx.FlexGridSizer (cols=2, hgap=6, vgap=6)
        sizer.AddMany ([label1, self.lon, self.lat, self.alt, label2, \
                       self.FlightSpeed, label2a, self.TurnRate, \
                       label3, self.WindDir, label4, \
                       self.WindSpd, label5, self.fuelBurn, label6, \
                       self.fuelLimit, label7, self.Extra,\
                       B1, B2])
        panel.SetSizer (sizer)

    def DismissConfig (self, event):
        'Dismiss the \'Specifications\' window.'
        self.Destroy ()

# Note: 'Save Configuration' saves for this run only; defaults can only
#       be saved for the next use by changing the constants in the 
#       program.  The Specs are saved with the track, though, and can 
#       optionally be restored if the track is re-used.
    def SaveConfig (self, event): 
        'Save the configuration, for this run only; defaults unchanged.'
        Specs.Specs['Airport'] = (float (self.lat.GetValue ()),\
                                  float (self.lon.GetValue ()),\
                                  float (self.alt.GetValue ()))
        Specs.Specs['FlightSpeed'] = float (self.FlightSpeed.GetValue ())
        Specs.Specs['MaxTurn'] = float (self.TurnRate.GetValue ())
        Specs.Specs['FuelBurn'] = float (self.fuelBurn.GetValue ())
        Specs.Specs['FuelLimit'] = float (self.fuelLimit.GetValue ())
        Specs.Specs['Extra'] = self.Extra.GetValue ()
        wd = float (self.WindDir.GetValue ())
        ws = float (self.WindSpd.GetValue ())
        Specs.Specs['Wind'] = (wd,ws)
        self.Destroy ()

class DLGFrame (wx.Frame):
    def __init__(self):
        'Asks for the gross weight and altitude, for Specific Range plot'
        wx.Frame.__init__ (self, None, -1, 'Specific Range Parameters', \
                           size=(300,120))
        panel = wx.Panel (self, -1)
        label1 = wx.StaticText (panel, -1, "Gross Weight (lbs):", \
                                size=(150,32))
        self.GrossWeight = wx.TextCtrl (panel, -1, \
                           format (Specs.GrossWeight (), '.0f'),\
                           size = (100,32))
        label2 = wx.StaticText (panel, -1, "Altitude (ft):", \
                                size=(150,32))
        self.Altitude = wx.TextCtrl (panel, -1, \
                                        str (ModuleConstructor.AC[2]),\
                                        size = (150,32))
        B1 = wx.Button (panel, ID_CM, 'Set', \
                        size=(35,32))
        sizer = wx.FlexGridSizer (cols=2, hgap=6, vgap=6)
        sizer.AddMany ([label1, self.GrossWeight, label2, \
                       self.Altitude, B1])
        panel.SetSizer (sizer)


# here is the main window for the UI. It is split into two panels, the
# left one having buttons to be used in dequence to construct the track
# and the right one containing specifications for the individual modules.
class MainWindow (wx.Frame):
    def __init__(self, parent, id, title):
        'Defines the primary window for user interaction.'
        wx.Frame.__init__(self, parent, wx.ID_ANY, title, \
                          size = (500,250),\
                          style = wx.DEFAULT_FRAME_STYLE)
        splitter = wx.SplitterWindow (self, -1, pos=(0,100),\
                                      style=wx.SP_3D)
#       self.control = wx.TextCtrl (self, 1, style = wx.TE_MULTILINE)
        panel1 = wx.Panel (splitter, -1)
        panel2 = wx.Panel (splitter, -1)
        panel1.SetBackgroundColour ('Light Grey')
        panel2.SetBackgroundColour ('Light Blue')
# Status bar at the bottom of the window
        StatusBar = self.CreateStatusBar (3)
        StatusBar.SetStatusWidths ([2,180,-1])
        self.SetStatusText ('Right-click items for hints re usage', 2)
        splitter.SplitVertically (panel1, panel2,180)
        splitter.SizeWindows ()
#       box = wx.BoxSizer (wx.VERTICAL)
#       box.Add (wx.Button (panel, -1, 'Start new track'), 1)
#       box.Add (wx.Button (panel, -1, 'Add module to track'), 1)
#       box.Add (wx.Button (panel, -1, 'Show current track'), 1)
#       panel.SetSizer (box)
#       box = wx.BoxSizer (wx.VERTICAL)
#       box.Add (splitter)
#       box.Add (StatusBar)
#       self.SetSizer (box)
        module_list = Specs.ModuleType

# initial defaults for module parameters:
        s = str (100.)		# size, n mi
# anchor point for module: (longitude, latitude, altitude)
        o = str (0.)		# orientation for module, deg true
#       dr = False		# drifting with wind? (True or False)
        legs = str (2)		# number of legs in pattern, if relevant
        othr = str (50.)	# customizable depending on type
# dimensions for panel layout:
        dely = 32		# line size
        Col1 = 180		# width of first column
        Col2 = 5		# start of 2nd column
        yloc = 0		# variable to increment by dely for rows
        self.cb = wx.ComboBox (panel1, ID_SM, \
         '      Select Module', (0,yloc), (Col1,dely), module_list)
        self.module_list = module_list
        yloc += dely
        B1 = wx.Button (panel1, ID_AM, 'Add module to track', \
                        (0,yloc), (Col1,dely))
        B1.SetBackgroundColour ('Pink')
        yloc += dely
        wx.Button (panel1, ID_SCT, 'Show current track ', (0,yloc), \
                   (Col1,dely))
        yloc += dely
        B3 = wx.Button (panel1, ID_SE, 'Backup and make kml', \
                        (0,yloc), (Col1,dely))
        B3.SetBackgroundColour ('Pink')
        yloc += dely
        wx.Button (panel1, ID_GE, 'Show in Google Earth', \
                   (0,yloc), (Col1,dely))
        yloc = 0
        fontBold = wx.Font (12, wx.SWISS, wx.NORMAL,wx.BOLD)
        moduleHead = wx.StaticText (panel2, -1, "Module Specs:", \
                                    (Col2,yloc), size=(100,dely))
        moduleHead.SetFont (fontBold)
        self.DegMin = wx.CheckBox (panel2, ID_CK, 'deg min format', \
                                   (Col2+170,yloc-5), size = (120,dely))
        self.DegMin.SetToolTip(wx.ToolTip \
           ('enter lat/lon as int degrees and decimal minutes?\n'\
            + '(If unchecked, lat/lon expected in decimal degrees)'))
        self.DegMin.SetValue (True)
        yloc += dely
        wx.StaticText (panel2, -1, "#", (Col2+5,yloc), \
                                 size=(75,dely))
        self.sc = wx.SpinCtrl (panel2, -1, '', (Col2+15,yloc), \
                               size=(50,dely))
        self.sc.SetRange (1, 999)
        self.sc.SetValue (1)
        wx.StaticText (panel2, -1, "Size, n mi:", \
                                (Col2+70,yloc),size=(75,dely))
        self.InputS = wx.TextCtrl (panel2, ID_SZ, s, (Col2+145,yloc), \
                                   size = (100,dely))
        yloc += dely
        wx.StaticText (panel2, -1, \
                     "Anchor (lat, lon, alt) [deg,min, deg,min, ft]:", \
                     (Col2+20,yloc+6),size=(280,dely))
        yloc += dely
        degree, minute = ConvertToDegMin (Specs.TakeoffLocation ()[1])
        slt = format (degree, '.0f') + '  ' + format (minute, '.1f')
        self.InputLT = wx.TextCtrl (panel2, ID_LT, slt, \
                                    (Col2+15,yloc), size = (90,dely))
        degree, minute = ConvertToDegMin (Specs.TakeoffLocation ()[0])
        slg = format (degree, '.0f') + '  ' + format (minute, '.1f')
        self.InputLG = wx.TextCtrl (panel2, ID_LG, slg, \
                                    (Col2+110,yloc), size = (90,dely))
        self.InputZ = wx.TextCtrl (panel2, ID_Z, \
                                   str (Specs.TakeoffLocation ()[2]), \
                                   (Col2+205,yloc), size = (90,dely))
        yloc += dely
        wx.StaticText (panel2, -1, "Angle:", (Col2,yloc),\
                                size=(60,dely))
        self.InputOR = wx.TextCtrl (panel2, ID_OR, o, (65,yloc), \
                                    size = (50,dely))
        wx.StaticText (panel2, -1, "Legs:", (Col2+130,yloc),\
                                size=(60,dely))
        self.InputLEGS = wx.TextCtrl (panel2, ID_LGS, legs, \
                                      (Col2+180,yloc), size = (50,dely))
        yloc += dely
        ButtonOther = wx.Button (panel2, ID_OT, 'Other', \
                                 (Col2,5*dely),size=(60,dely))
        ButtonOther.SetBackgroundColour ('Light Grey')
        ButtonOther.SetToolTip(wx.ToolTip \
           ('click this button for help regarding these entries.\n'\
            + '(The \'Other\' entry depends on the module type)'))
        self.InputOTHER = wx.TextCtrl (panel2, ID_OTH, othr, \
                                      (Col2+65,yloc), size = (100,dely))
        self.Drifting = wx.CheckBox (panel2, ID_DF, 'Drifting', \
                                     (Col2+170,yloc), size = (100,dely))
        self.Drifting.SetToolTip(wx.ToolTip \
           ('Check this to have the pattern drift with the wind.\n'\
            + '(Wind direction and speed are specified by either \n'\
            + 'a sounding or fixed wind direction and speed.)'))
        self.Bind (wx.EVT_BUTTON, self.OnStartTrack, id=ID_ST)
        self.Bind (wx.EVT_COMBOBOX, self.OnSelectModule, id=ID_SM)
        self.Bind (wx.EVT_CONTEXT_MENU, self.onHint)
        self.Bind (wx.EVT_BUTTON, self.OnAddModule, id=ID_AM)
        self.Bind (wx.EVT_BUTTON, self.OnShowTrack, id=ID_SCT)
        self.Bind (wx.EVT_BUTTON, self.OnMakeOut, id=ID_SE)
        self.Bind (wx.EVT_BUTTON, self.OnGoogleEarth, id=ID_GE)
        self.Bind (wx.EVT_BUTTON, self.OnOther, id=ID_OT)
#       self.Bind (wx.EVT_SPIN, self.OnSpin, id=ID_SC)
        self.Bind (wx.EVT_SPINCTRL, self.OnSpin, self.sc)
        self.Bind (wx.EVT_CHECKBOX, self.onCheckBoxChange, id=ID_CK) 
#       self.Centre ()
# menus:
        filemenu = wx.Menu ()
        specsmenu = wx.Menu ()
        editmenu = wx.Menu ()
        helpmenu = wx.Menu ()
        mapmenu = wx.Menu ()
        navmenu = wx.Menu ()
        GVmenu = wx.Menu ()
        filemenu.Append (ID_NEW,  '&New\tCtrl+N', \
                         'Start a new flight plan')
        filemenu.Append (ID_OPEN,  '&Open\tCtrl+O', \
                         'Select an existing flight plan')
        filemenu.Append (ID_APPEND,  '&Append\tCtrl+A', \
                         'Append an existing flight plan')
        filemenu.Append (ID_SAVE,  "&Save\tCtrl+S", \
                        "Save the flight plan")
        filemenu.Append (ID_PRNT,  "&List\tCtrl+L", \
                        "Make coordinate listing")
        filemenu.Append (ID_SHOW, "&View\tCtrl+V", \
                        "Display the current track")
        filemenu.AppendSeparator ()
        filemenu.Append (ID_ABOUT,  "About", \
                         "Information about this program")
        filemenu.Append (ID_EXIT, "E&xit\tCtrl+Q", "Quit")
        specsmenu.Append (ID_EDIT, '&Edit\tCtrl+E', \
                          'edit specs (aircraft, airport, wind)')
        specsmenu.Append (ID_GET, '&Get Sounding\tCtrl+W', \
                          'download latest DNR sounding')
        specsmenu.Append (ID_SND, '&See Sounding', \
                          'See sounding in use')
        editmenu.Append (ID_IS, '&Insert\tCtrl+I', 'insert a module')
        editmenu.Append (ID_DX, '&Delete\tCtrl+D', 'delete a module')
        editmenu.Append (ID_CY, '&Copy', 'copy a module')
        editmenu.Append (ID_RO, '&Reorder', 'reorder modules')
        editmenu.Append (ID_RC, 'Re&calculate', 'calculate track')
        mapmenu.Append  (ID_MP, '&Airways\tCtrl+M', 'High Altitude Chart')
        navmenu.Append  (ID_NV, 'See Nav &Points\tCtrl+P', \
                                'Reference Points')
        navmenu.Append  (ID_LF, '&Open', 'Open a specified file')
        navmenu.Append  (ID_S3, '&Save', 'Save to a specified file')
        GVmenu.Append  (ID_TAS, 'TA&S', 'Show TAS for conditions')
        GVmenu.Append  (ID_SR, '&Specific Range', 'n mi per lb fuel')
        GVmenu.Append  (ID_CP, 'Climb&Profile', 'Show climb altitude vs time')
        GVmenu.Append  (ID_DP, '&DescentProfile', 'Show max descent vs time')
        helpmenu.Append (ID_HP, 'User\'s Manual', 'Instructions')

# Put menus on the menubar:
        menubar = wx.MenuBar ()
        menubar.Append (filemenu, '&File')
        menubar.Append (specsmenu, '&Specs')
        menubar.Append (editmenu, '&Edit Module')
        menubar.Append (mapmenu, '&Maps')
        menubar.Append (navmenu, '&NavPts')
        menubar.Append (GVmenu, '&Performance')
        menubar.Append (helpmenu, '&Help')
        self.SetMenuBar (menubar)
# bind to event handlers
        wx.EVT_MENU (self, ID_ABOUT, self.OnAbout)
        wx.EVT_MENU (self, ID_EXIT, self.OnExit)
        wx.EVT_MENU (self, ID_OPEN, self.OnOpen)
        wx.EVT_MENU (self, ID_APPEND, self.OnAppend)
        wx.EVT_MENU (self, ID_NEW, self.OnStartTrack)
        wx.EVT_MENU (self, ID_SAVE, self.OnSaveTrack)
        wx.EVT_MENU (self, ID_PRNT, self.OnList)
        wx.EVT_MENU (self, ID_SHOW, self.OnShow)
        wx.EVT_MENU (self, ID_EDIT, self.OnEdit)
        wx.EVT_MENU (self, ID_GET, self.OnGet)
        wx.EVT_MENU (self, ID_IC, self.OnIC)
        wx.EVT_MENU (self, ID_TR, self.OnTR)
        wx.EVT_MENU (self, ID_OB, self.OnOB)
        wx.EVT_MENU (self, ID_VS, self.OnVS)
        wx.EVT_MENU (self, ID_LM, self.OnLM)
        wx.EVT_MENU (self, ID_RT, self.OnRT)
        wx.EVT_MENU (self, ID_SP, self.SetParameters)
        wx.EVT_MENU (self, ID_DA, self.OnDA)
        wx.EVT_MENU (self, ID_MD, self.OnMD)
        wx.EVT_MENU (self, ID_IS, self.OnIS)
        wx.EVT_MENU (self, ID_CY, self.OnIS)	# see note with OnIS
        wx.EVT_MENU (self, ID_DX, self.OnDX)
        wx.EVT_MENU (self, ID_SA, self.OnSA)
        wx.EVT_MENU (self, ID_RO, self.OnReorder)
        wx.EVT_MENU (self, ID_RC, self.OnRecalculate)
        wx.EVT_MENU (self, ID_MP, self.OnMap)
        wx.EVT_MENU (self, ID_SR, self.OnSR)
        wx.EVT_MENU (self, ID_CP, self.OnCP)
        wx.EVT_MENU (self, ID_DP, self.OnDP)
        wx.EVT_MENU (self, ID_NV, self.OnNav)
        wx.EVT_MENU (self, ID_LF, self.onLoadNav)
        wx.EVT_MENU (self, ID_S3, self.onSave3)
        wx.EVT_MENU (self, ID_HP, self.onHelp)
        wx.EVT_MENU (self, ID_SND, self.OnSND)
        wx.EVT_MENU (self, ID_TAS, self.OnTAS)
# temporary; delete soon
        self.Bind (wx.EVT_MENU, self.choosecolor, id=300)
        self.Bind (wx.EVT_MENU, self.openfile, id=301)
        self.Bind (wx.EVT_MENU, self.pagesetup, id=302)
        self.Bind (wx.EVT_MENU, self.choosefont, id=303)
        self.Bind (wx.EVT_MENU, self.lonentry, id=306)
        self.SetMaxSize ((500, 270))
        self.SetMinSize ((500, 250))
        self.Show (True)


# menu-event handlers:
    def onFile (self, event): 
        'Saves the track listing to a file.'
        if event.GetId () == ID_FL2:
            dlg = wx.FileDialog (self, "Save as:", os.getcwd (), \
                                 "TRACK.trk", "*.trk", wx.SAVE)
            listing = frame3.listing
        else:		# ID_FL
            dlg = wx.FileDialog (self, "Save as:", os.getcwd (), \
                                 "WPTS.wpts", "*.wpts", wx.SAVE)
            listing = frame4.listing
        if dlg.ShowModal () == wx.ID_OK:
            path = dlg.GetPath ()
            mypath = os.path.basename (path)
            self.SetStatusText ("File saved to %s" % mypath, 1)
#           print "path = ",path
#           print "file is ", mypath
            WaypointFile = open (path, 'wb')
            WaypointFile.write (listing)
            WaypointFile.close ()
        dlg.Destroy ()
    def onLoadNav (self, event): 
        'Load navigation points from a selected file.'
        global frame7, NavItems
        dlg = wx.FileDialog (self, "Choose a file", os.getcwd (), \
                             "", "*.nav", wx.OPEN)
        if dlg.ShowModal () == wx.ID_OK:
            path = dlg.GetPath ()
            mypath = os.path.basename (path)
            self.SetStatusText ("You selected: %s" % mypath, 1)
#           print "path = ",path
#           print "file is ", mypath
            f = open (path, 'rb')
            NavItems = pickle.load (f)
            f.close ()
            frame7.Close ()
            frame7 = NavFrame ()
            frame7.Show ()
        dlg.Destroy ()

    def onSave3(self, event):
        "Save the navigation points in NavItems."
        dlg = wx.FileDialog (self, "Provide a file name:", \
                             os.getcwd (), "", "*.nav", wx.SAVE)
        if dlg.ShowModal () == wx.ID_OK:
            path = dlg.GetPath ()
            mypath = os.path.basename (path)
            self.SetStatusText ("Selected path: %s" % mypath, 1)
            f = open (path, 'wb')
            pickle.dump (NavItems, f)
            f.close ()
        dlg.Destroy ()

    def onHelp (self, event):
        "Display a window with instructions."
        global frame8
        f = open ('./Help.txt','rb')
        listing = 'Instructions:\n\n\n'
        for line in f:
            listing += line
        f.close ()
        if frame8 != None and frame8: frame8.Close ()
        frame8 = HelpFrame (listing)
        frame8.Show ()

    def onHint (self, event):
        "Display a window with context hints."
        global frame9
        CallingID = event.GetId ()
        if CallingID < 0: 
            frame.SetStatusText ('Hints unavailable for this item', 2)
            return ()
        f = open ('./Hint.txt','rb')
        frame.SetStatusText ('See suggestions in the \'Hints\' window', 2)
        hint = 'Suggestions:\n\n\n'
        enable = False
        for line in f:
            if CallingID == ID_SM: 
                if '**Select Module:**' in line: 
                    enable = True
            if CallingID == ID_AM: 
                if '**Add module to track:**' in line: 
                    enable = True
            if CallingID == ID_SCT: 
                if '**Show current track:**' in line: 
                    enable = True
            if CallingID == ID_SE: 
                if '**Backup and make kml:**' in line: 
                    enable = True
            if CallingID == ID_GE: 
                if '**Show in Google Earth:**' in line: 
                    enable = True
            if CallingID == ID_CK: 
                if '**deg min format:**' in line: 
                    enable = True
            if CallingID == ID_SZ: 
                if '**Size, n mi:**' in line: 
                    enable = True
            if CallingID == ID_LG or CallingID == ID_LT \
               or CallingID == ID_Z:
                if '**Anchor (lat, lon, alt) [deg,min, deg,min, ft]:**'\
                  in line:
                    enable = True
            if CallingID == ID_OR: 
                if '**Angle:**' in line: 
                    enable = True
            if CallingID == ID_LGS: 
                if '**Legs:**' in line: 
                    enable = True
            if CallingID == ID_OTH: 
                if '**Other:**' in line: 
                    enable = True
            if CallingID == ID_DF: 
                if '**Drifting:**' in line: 
                    enable = True
            if CallingID == ID_OT: 
                if '**Other Button:**' in line: 
                    enable = True
            if '-----' in line: enable = False
            if enable: hint += line
        if frame9 != None and frame9: frame9.Close ()
        frame9 = WizardFrame (hint)
        frame9.Show ()

    def OnStartTrack (self, event): 
        "Abandon present track and start over with a new track."
        if (ModuleConstructor.Track != {}):
            dlg = wx.MessageDialog (self,\
                'There is an existing track.\nClick OK to erase it',\
                'Confirm Erase', wx.OK|wx.CANCEL|wx.ICON_QUESTION)
            result = dlg.ShowModal ()
            dlg.Destroy ()
            if result == wx.ID_OK:
                ModuleConstructor.Track = {}
                ModuleConstructor.ManeuverNumber = 0
                ModuleConstructor.ModuleNumber = 0
                ModuleConstructor.AC = Specs.TakeoffLocation ()
                ModuleConstructor.Time = 0
                ModuleConstructor.Fuel = Specs.FuelUseLimit ()
                self.sc.SetValue (1)
                self.InputS.SetValue (str (100.))
                self.InputOR.SetValue (str (0.))
                self.InputLEGS.SetValue (str (1))
                self.InputOTHER.SetValue (str (2000.))
                if self.DegMin.GetValue ():	# deg-min section
                    degree, minute = ConvertToDegMin \
                                         (Specs.TakeoffLocation ()[1])
                    slt = format (degree, '.0f') + '  ' \
                        + format (minute, '.1f')
                    self.InputLT.SetValue (slt)
                    degree, minute = ConvertToDegMin \
                                           (Specs.TakeoffLocation ()[0])
                    slg = format (degree, '.0f') + '  ' \
                        + format (minute, '.1f')
                    self.InputLG.SetValue (slg)
                else:
                    self.InputLG.SetValue \
                                     (str (Specs.TakeoffLocation ()[0]))
                    self.InputLT.SetValue (str (Specs.TakeoffLocation \
                                                                ()[1]))
                self.InputZ.SetValue (str (Specs.TakeoffLocation ()[2]))
                self.Drifting.SetValue (False)
                self.DegMin.SetValue (True)

    def OnSelectModule (self, event): 
        "Choose the type of the next module to add to the track."
        ix = event.GetSelection ()
        module_name = self.module_list [ix]
        self.SetStatusText ('Module name: %s' % module_name, 1)
        ModuleConstructor.WorkingModule = {}
        ModuleConstructor.WorkingModuleName = module_name
# reset 'Module Specs' panel items as appropriate for selected module:
        if (module_name == 'InitialClimb'):
            self.InputS.SetValue ('None')
            self.InputOR.SetValue ('None')
            self.InputLEGS.SetValue ('None')
            self.InputZ.SetValue (str (35000.))
			# 'Other' is rate of climb, ft/min
            self.InputOTHER.SetValue ('2000.')
        elif (module_name == 'Enroute'):
            self.InputS.SetValue ('None')
            self.InputOTHER.SetValue ('0.')	#climb/desc rate; 0=>any
            self.InputOR.SetValue ('None')
            self.InputLEGS.SetValue ('None')
        elif (module_name == 'DelayInArea'):
            self.InputS.SetValue ('100.')
            self.InputOR.SetValue ('None')
            self.InputLEGS.SetValue ('None')
			# 'Other' is time to delay, hrs
            self.InputOTHER.SetValue ('1.0')
        elif (module_name == 'OutAndBack'):
            self.InputS.SetValue ('100.')
            self.InputOR.SetValue ('0.0')
            self.InputLEGS.SetValue ('None')
            self.InputOTHER.SetValue ('None')
        elif (module_name == 'VerticalSection'):
            self.InputS.SetValue ('100.')
            self.InputOR.SetValue ('0.0')
            self.InputLEGS.SetValue ('2')
			# 'Other' is altitude change between legs
            self.InputOTHER.SetValue ('-1000.')
        elif (module_name == 'Sawtooth'):
            self.InputS.SetValue ('100.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('None')
            self.InputOTHER.SetValue ('2000.')
        elif (module_name == 'StairStep'):
            self.InputS.SetValue ('150.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('3')
            self.InputOTHER.SetValue ('500.')
        elif (module_name == 'SpiralProfile'):
            self.InputS.SetValue ('None')
            self.InputOR.SetValue ('None')
            self.InputLEGS.SetValue ('None')
			# 'Other' is rate of climb or descent
            self.InputOTHER.SetValue ('1000.')
        elif (module_name == 'Circle'):
            self.InputS.SetValue ('100.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('None')
			# 'Other' is rate of turn, deg/s
			# (This is calculated, not input)
            self.InputOTHER.SetValue ('1')	# turn to the right
        elif (module_name == 'Racetrack'):
            self.InputS.SetValue ('100.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('2')
			# 'Other' is offset between legs, n mi
                        # (+ve for right turns)
            self.InputOTHER.SetValue ('50.')
        elif (module_name == 'Lawnmower'):
            self.InputS.SetValue ('100.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('2')
			# 'Other' is offset between legs, n mi
                        # (+ve for first turn to right)
            self.InputOTHER.SetValue ('50.')
        elif (module_name == 'SquareSpiral'):
            self.InputS.SetValue ('300.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('33')
            self.InputOTHER.SetValue ('90')
        elif (module_name == 'Figure4'):
            self.InputS.SetValue ('200.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('None')
            self.InputOTHER.SetValue ('None')
        elif (module_name == 'Figure8'):
            self.InputS.SetValue ('50.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('4')
#			# altitude increment between legs
            self.InputOTHER.SetValue ('0.')	
        elif (module_name == 'Butterfly'):
            self.InputS.SetValue ('200.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('1')
#			# altitude increment between patterns
            self.InputOTHER.SetValue ('0.')	
        elif (module_name == 'ThreeBlade'):
            self.InputS.SetValue ('200.')
            self.InputOR.SetValue ('0.')
            self.InputLEGS.SetValue ('None')
#			# altitude increment between patterns
            self.InputOTHER.SetValue ('None')	
        elif (module_name == 'ReturnToBase'):
            self.InputS.SetValue ('None')
            self.InputOR.SetValue ('None')
            self.InputLEGS.SetValue ('None')
            self.InputOTHER.SetValue ('None')	
            self.Drifting.SetValue (False)

    def OnAddModule (self, event): 
        "Add the currently defined module to the track definition."
        global frame
#       need to set: name, dimension, anchor, orientation, drift, 
#       legs, other
# deal with 'None' entries, then reset after use
        s = self.InputS.GetValue ()
        if (s == 'None' or s == 'NA'): s = 'NA'
        else: s = float (s)
        o = self.InputOR.GetValue ()
        if (o == 'None' or o == 'NA'): o = 'NA'
        else: o = float (o)
        l = self.InputLEGS.GetValue ()
        if (l == 'None' or l == 'NA'): l = 'NA'
        elif 'x' not in l: l = int (l)	# allows for 4x5 format SqSpiral
        other = self.InputOTHER.GetValue ()
        if (other == 'None' or other == 'NA'): other = 'NA'
        else: other = float (other)
# in case this is not the last module but is one being edited,
# restore Time, Fuel, and AC to the values at the end of the
# last module
        if (ModuleConstructor.ModuleNumber == 0):
            ModuleConstructor.Time = 0.
            ModuleConstructor.Fuel = Specs.FuelUseLimit ()
            ModuleConstructor.ManeuverNumber = 0
            ModuleConstructor.FlightDirection = 0
            time = 0.
        else:
#			get the previous module to find end time, etc.
            ky = "Module" + format (ModuleConstructor.ModuleNumber, \
                                    '03d')
            md = ModuleConstructor.Track[ky]
            KeyListManeuvers = md.keys ()
            KeyListManeuvers.sort ()
            for kym in KeyListManeuvers:
                if (kym.find ('Manvr') != -1): 
                    mm = md[kym]
                    time = mm['Time']
                    fuel = mm['Fuel']
                    fd   = mm['FlightDirection']
                    mvr = int (kym.replace ('Manvr',''))
            ModuleConstructor.Time = time
            ModuleConstructor.Fuel = fuel
            ModuleConstructor.ManeuverNumber = mvr
            ModuleConstructor.FlightDirection = fd
        lg,lt = self.GetLatLonValues ()
        md = ModuleConstructor.Module \
                            (ModuleConstructor.WorkingModuleName, s,\
                             (lg, lt, float (self.InputZ.GetValue ())),\
                             o, self.Drifting.GetValue (), l, other,\
                             frame)
        md['Time'] = time
# (frame is included here so Module can reset items on the frame if 
#  that is useful. At present, this is done if some characteristic
#  like the circle radius is not possible at the specified flight
#  level.)
#       print md
#       add to Track with key = module number:
        ky = "Module" + format (ModuleConstructor.ModuleNumber, '03d')
        ModuleConstructor.Track[ky] = md
#       print "At end of AddModule; here is Track:"
#       print ModuleConstructor.Track
        ModuleConstructor.WorkingModule = md
#       increment the displayed module number
        self.sc.SetValue (ModuleConstructor.ModuleNumber + 1)
#		If the new module number represents and existing module,
#		load its values into the combobox and text-entry windows:
        self.OnSpin (self)
#		Now take into account that, if module is being
#		edited, there may be subsequent modules that are
#		affected. Must loop through all subsequent
#		modules and reconstruct them to get Time and Fuel
#		to be correct.
        Recalculate ()

    def GetLatLonValues (self):
        'Read lat/lon from window, convert if necessary from deg-min, '\
         + 'and return the current values in decimal-degree format'
#		if necessary, convert deg-min format to decimal-deg
        if self.DegMin.GetValue ():	# deg-min section
            slg = self.InputLG.GetValue ()
            slt = self.InputLT.GetValue ()
            slg = slg.replace (',',' ')
            slt = slt.replace (',',' ')
            ssplit = slg.split ()
            if len (ssplit) < 2: ssplit.append ('0.')
            lg = int (float (ssplit[0]))
            if lg < 0: lg -= float (ssplit[1]) / 60.
            else: lg += float (ssplit[1]) / 60.
            ssplit = slt.split ()
            if len (ssplit) < 2: ssplit.append ('0.')
            lt = int (float (ssplit[0]))
            if lt < 0: lt -= float (ssplit[1]) / 60.
            else: lt += float (ssplit[1]) / 60.
        else:				# decimal-degree section
            lg = float (self.InputLG.GetValue ())
            lt = float (self.InputLT.GetValue ())
        return (lg,lt)

    def OnShowTrack (self, event):
        "Display modules contained in the current track."
        global frame3
        if 'Comment' in ModuleConstructor.Track.keys ():
            print "Comment in current track:\n", \
                  ModuleConstructor.Track['Comment']
        else:
            print 'No comment in current track.'
        message = 'Track has these modules:\n'
#       print "\nOnShow has this track:",ModuleConstructor.Track
        # print them in order
        KeyList = ModuleConstructor.Track.keys ()
        KeyList.sort ()
        for ky in KeyList:
            if ('Module' in ky and 'Number' not in ky):
                m = ModuleConstructor.Track[ky]
                degLat, minLat = ConvertToDegMin (m['Anchor'][1])
                degLon, minLon = ConvertToDegMin (m['Anchor'][0])
                message += '\n' + ky + ': ' + m['Type'] + '\n'
                message += 'Anchor = '
                message += format (degLat, '.0f') + '  ' \
                         + format (minLat, '.1f') + ', '
                message += format (degLon, '.0f') + '  ' \
                         + format (minLon, '.1f') + ', '
                message += format (m['Anchor'][2], '.0f')
                message += '\nDimension = '
                if m['Dimension'] == 'None' or m['Dimension'] == 'NA': 
                    message += m['Dimension']
                else: message += format (m['Dimension'], '.0f')
                message += '  Orientation = '
                if m['Orientation'] == 'None' \
                   or m['Orientation'] == 'NA': 
                    message += m['Orientation']
                else: message += format (m['Orientation'], '.0f')
                message += '\nLegs = '
                if (m['Legs'] == 'None' or m['Legs'] == 'NA'): 
                    message += m['Legs']
                else: message += format (m['Legs'], 'd')
                message += '  Drift = '
                message += format (m['Drift'])
                message += '  Other = '
                if m['Other'] == 'None' or m['Other'] == 'NA': 
                    message += m['Other']
                else: message += format (m['Other'], '.0f')
                time = 0.
                KeyListManeuvers = m.keys ()
                KeyListManeuvers.sort ()
                for kym in KeyListManeuvers:
                    if (kym.find ('Manvr') != -1): 
                        mm = m[kym]
                        time = mm['Time']
                        fuel = mm['Fuel']
                message += '\n  Time = ' + format (time, '.1f')
                message += '  Fuel = ' + format (fuel, '.0f') + '\n'
        if frame3 != None and frame3: frame3.Close ()
        frame3 = TrackFrame (message)
        frame3.Show ()

    def OnGoogleEarth (self, event):
        "Start Google Earth and show .kml file"
			# generate .kml file, in case it is needed
        self.OnMakeOut (event)
			# need appropriate replacement for Windows:
        os.system ("google-earth $PWD/Plan.kml&")

    def OnMakeOut (self, event): 
        'Make .kml output file and save backup copy of Track'
      			# extract each module from track, then extract
			# each maneuver from module. Do both in order
        KMLFile, KMLACFile = KMLHeader ("Plan.kml")
#       save Specs (including format code) as part of track:
        ModuleConstructor.Track['Specs'] = Specs.Specs
# add counters and other information needed to restore the track:
        ModuleConstructor.Track['Fuel'] = ModuleConstructor.Fuel
        ModuleConstructor.Track['Time'] = ModuleConstructor.Time
        ModuleConstructor.Track['AC'] = ModuleConstructor.AC
        ModuleConstructor.Track['FlightDirection'] \
                               = ModuleConstructor.FlightDirection
        ModuleConstructor.Track['ModuleNumber'] \
                               = ModuleConstructor.ModuleNumber
        ModuleConstructor.Track['ManeuverNumber'] \
                               = ModuleConstructor.ManeuverNumber
# save a backup of the track. This is always named Backup.track.
# use the File menu, Save item, to save under a different name.
        TrackFile = open ('./Backup.track','wb')
        pickle.dump (ModuleConstructor.Track, TrackFile)
#       TrackFile.write (repr (ModuleConstructor.Track))
        TrackFile.close ()
        KeyList = ModuleConstructor.Track.keys ()
        KeyList.sort ()
        alast = {}
        for ky in KeyList:
            if ('Module' in ky and 'Number' not in ky):
                m = ModuleConstructor.Track[ky]
                KeyListManeuvers = m.keys ()
                KeyListManeuvers.sort ()
                for kym in KeyListManeuvers:
                    if ('Manvr' in kym):
                        mn = m[kym]
                        al = mn['Anchor']
                        if (al != alast):
                            PlotWaypoint (KMLFile, KMLACFile, al)
                            alast = al
                        an = mn['EndPoint']
                        if (an != alast):
                            PlotWaypoint (KMLFile, KMLACFile, an)
                            alast = an
                #end of loop over keys in module
		# Special module-dependent code:
                if m['Type'] == 'Circle':
				# generate points around a circle.
				# find circle center:
                    al = m['Anchor']
                    oo = m['Orientation']
                    ot = m['Other']
                    ss = m['Dimension'] / 2.	# this is circle radius
                    dr = m['Drift']
                    oo += 90. * ot
                    c0 = al[0] + ss * math.sin (oo * Cradeg) \
                         / (60. * math.cos (al[1] * Cradeg))
                    c1 = al[1] + ss * math.cos (oo * Cradeg) / 60.
                    oo += 180.
                    circPts = []
				# get time between points for drift:
                    nsteps = 30
                    dt = 2. * ss * PI / (nsteps * Specs.FlightSpeed (AC[2]))
                    oo -= 360. / nsteps
                    cc = (c0,c1,al[2])
                    for i in range (0,nsteps+1):
                        oo += 360. / nsteps
                        d0 = cc[0] + ss * math.sin (oo * Cradeg) \
                             / (60. * math.cos (cc[1] * Cradeg))
                        d1 = cc[1] + ss * math.cos (oo * Cradeg) / 60.
                        circPts.append((d0,d1,al[2]))
                        if dr: 
                            cc = ModuleConstructor.DriftOffset (dt, cc)
                    #end of circle points
                    PlotPoints (KMLFile, KMLACFile, circPts)
                #end of branch for Circle processing
                if m['Type'] == 'DelayInArea':
				# generate points around a circle.
				# find circle center:
                    cc = m['Anchor']
                    ss = m['Dimension']		# this is circle radius
                    dr = m['Drift']
                    circPts = []
				# get time between points for drift:
                    nsteps = 30
                    dt = 2. * ss * PI / (nsteps * Specs.FlightSpeed (AC[2]))
                    oo = -360. / nsteps
                    for i in range (0,nsteps+1):
                        oo += 360. / nsteps
                        d0 = cc[0] + ss * math.sin (oo * Cradeg) \
                             / (60. * math.cos (cc[1] * Cradeg))
                        d1 = cc[1] + ss * math.cos (oo * Cradeg) / 60.
                        circPts.append((d0,d1,al[2]))
                        if dr: 
                            cc = ModuleConstructor.DriftOffset (dt, cc)
                    #end of circle points
                    PlotPoints (KMLFile, KMLACFile, circPts)
                #end of branch for DelayInArea processing
                if m['Type'] == 'SpiralProfile':
				# generate points around a circle.
				# find circle center:
                    al = m['Anchor']
                    oo = m['FlightDirection']
                    ot = m['Other']
				# estimate the radius of the spiral
				# assume flown at 1/2 max turn rate
                    
                    ss = 2. * Specs.FlightSpeed (al[2]) * 360. \
                         / (Specs.MaxTurnRate (al[2]) * 2.*PI * 3600.)
                    dr = m['Drift']
				# assume spiral to the left
				# (usually most comfortable)
                    oo -= 90. 
                    c0 = al[0] + ss * math.sin (oo * Cradeg) \
                         / (60. * math.cos (al[1] * Cradeg))
                    c1 = al[1] + ss * math.cos (oo * Cradeg) / 60.
                    oo += 180.
                    circPts = []
				# get time between points for drift:
                    nsteps = 30
                    dt = 2. * ss * PI / (nsteps * Specs.FlightSpeed (AC[2]))
                    oo -= 360. / nsteps
                    cc = (c0,c1,al[2])
                    for i in range (0,nsteps+1):
                        oo += 360. / nsteps
                        d0 = cc[0] + ss * math.sin (oo * Cradeg) \
                             / (60. * math.cos (cc[1] * Cradeg))
                        d1 = cc[1] + ss * math.cos (oo * Cradeg) / 60.
                        circPts.append((d0,d1,al[2]))
                        if dr: 
                            cc = ModuleConstructor.DriftOffset (dt, cc)
                    #end of circle points
                    PlotPoints (KMLFile, KMLACFile, circPts)
                #end of branch for SpiralProfile processing
                if m['Type'] == 'Figure8':
				# generate points around a circle.
				# find circle center:
                    al = m['Anchor']
                    oo = m['Orientation']
                    ot = m['Other']
                    ss = m['Dimension'] / 4.	# radius of the Fig-8
                    dr = m['Drift']
				# assume first circle to the left
                    oo -= 90. 
                    c0 = al[0] + ss * math.sin (oo * Cradeg) \
                         / (60. * math.cos (al[1] * Cradeg))
                    c1 = al[1] + ss * math.cos (oo * Cradeg) / 60.
                    oo += 180.
                    circPts = []
				# get time between points for drift:
                    nsteps = 30
                    dt = 2. * ss * PI / (nsteps * Specs.FlightSpeed (AC[2]))
                    oo -= 360. / nsteps
                    cc = (c0,c1,al[2])
                    for i in range (0,nsteps+1):
                        oo += 360. / nsteps
                        d0 = cc[0] + ss * math.sin (oo * Cradeg) \
                             / (60. * math.cos (cc[1] * Cradeg))
                        d1 = cc[1] + ss * math.cos (oo * Cradeg) / 60.
                        circPts.append((d0,d1,al[2]))
                        if dr: 
                            cc = ModuleConstructor.DriftOffset (dt, cc)
                    #end of circle-left points; now circle-right
                    oo = m['Orientation']
                    oo += 90. 
                    c0 = al[0] + ss * math.sin (oo * Cradeg) \
                         / (60. * math.cos (al[1] * Cradeg))
                    c1 = al[1] + ss * math.cos (oo * Cradeg) / 60.
                    oo -= 180.
				# get time between points for drift:
                    oo -= 360. / nsteps
                    cc = (c0,c1,al[2])
                    for i in range (0,nsteps+1):
                        oo += 360. / nsteps
                        d0 = cc[0] + ss * math.sin (oo * Cradeg) \
                             / (60. * math.cos (cc[1] * Cradeg))
                        d1 = cc[1] + ss * math.cos (oo * Cradeg) / 60.
                        circPts.append((d0,d1,al[2]))
                        if dr: 
                            cc = ModuleConstructor.DriftOffset (dt, cc)
                    #end of circle-left points
                    PlotPoints (KMLFile, KMLACFile, circPts)
                #end of branch for Figure8 processing
            #end of branch to process modules
        #end of loop over keys in track
        KMLclose (KMLFile, KMLACFile)

    def onCheckBoxChange (self, event):
        'When checkbox is changed, change the displayed coordinate '\
         + 'format'
        global frame
# deg-min format was just changed, so go back to other setting 
# in order to read values:
        fmt =  self.DegMin.GetValue ()
        self.DegMin.SetValue (not self.DegMin.GetValue ())
        lg, lt = self.GetLatLonValues ()
        self.DegMin.SetValue (not self.DegMin.GetValue ())
        if fmt:
            degrees, minutes = ConvertToDegMin (lg)
            slg = format (degrees, '.0f') + '  ' \
                + format (minutes, '.1f')
            degrees, minutes = ConvertToDegMin (lt)
            slt = format (degrees, '.0f') \
                + '  ' + format (minutes, '.1f')
        else:				# decimal deg section
            slg = format (lg, '.3f')
            slt = format (lt, '.3f')
        frame.InputLT.SetValue (slt)
        frame.InputLG.SetValue (slg)
        if fmt:
            frame.SetStatusText ('Using deg-min format for lat/lon', 2)
        else:
            frame.SetStatusText ('Using decimal-degree format for '\
                                 + 'lat/lon', 2)
  

    def OnSpin (self, event):
        "Controls the display of modules."
# 		set the display to correspond to the selected module
#		check for module key in defined track
        modNumber = self.sc.GetValue ()
        ModuleConstructor.ModuleNumber = modNumber - 1
        Test = 'Module' + format (modNumber, '03d')
        if Test not in ModuleConstructor.Track.keys ():
            frame.SetStatusText \
                ('module # ' + format (modNumber, 'd') \
                  + ' is not defined yet', 2)
        else:
            m = ModuleConstructor.Track[Test]
            t = m['Type']
#		find index module list
            indx = self.module_list.index (t)
            self.cb.SetValue (self.module_list[indx])
            ModuleConstructor.WorkingModuleName = self.module_list[indx]
# deal with 'None' entries, then reset after use
            self.InputS.SetValue (str (m['Dimension']))
            self.InputOR.SetValue (str (m['Orientation']))
            self.InputLEGS.SetValue (str (m['Legs']))
            self.InputOTHER.SetValue (str (m['Other']))
#		# branch on lat/lon format
#		# if necessary, convert deg-min format to decimal-deg
            lg = m['Anchor'][0]
            lt = m['Anchor'][1]
#           print "values from referenced module are ",lg," and ",lt
            if self.DegMin.GetValue ():	# deg-min section
                degrees, minutes = ConvertToDegMin (lg)
                slg = format (degrees, 'd') + '  ' \
                    + format (minutes, '.1f')
                degrees, minutes = ConvertToDegMin (lt)
                slt = format (degrees, 'd') + '  ' \
                    + format (minutes, '.1f')
            else:
                slg = format (lg, '.3f')
                slt = format (lt, '.3f')
            self.InputLG.SetValue (slg)
            self.InputLT.SetValue (slt)
            self.InputZ.SetValue (format (m['Anchor'][2], '.0f'))
            self.Drifting.SetValue (m['Drift'])
# also restore end properties of previous track
            if (modNumber < 2):
                ModuleConstructor.AC = Specs.TakeoffLocation ()
                ModuleConstructor.Time = 0.
                ModuleConstructor.Fuel = Specs.FuelUseLimit ()
            else:
                Test = 'Module' + format (modNumber-1, '03d')
                if Test not in ModuleConstructor.Track.keys ():
                    print 'Selected module number ' \
                    + format (modNumber, 'd') + \
                    ' is not in defined track\n'
                else:
                    m = ModuleConstructor.Track[Test]
                    KeyListManeuvers = m.keys ()
                    KeyListManeuvers.sort ()
# loop through maneuvers; time and fuel will be that of last
# maneuver after loop completes
                    for kym in KeyListManeuvers:
                        if (kym.find ('Manvr') != -1): 
                            mm = m[kym]
                            time = mm['Time']
                            fuel = mm['Fuel']
                            fd = mm['FlightDirection']
                            manvNo = int (kym.replace ('Manvr',''))
                    ModuleConstructor.Fuel = fuel
                    ModuleConstructor.Time = time
                    ModuleConstructor.FlightDirection = fd
                    ModuleConstructor.ManeuverNumber = manvNo

    def OnOther (self, event):
        "This button provides information on the \'Other\' entry."
        if (ModuleConstructor.WorkingModuleName == 'InitialClimb'):
            OtherMessage = 'For InitialClimb \'Other\' specifies the\n'\
                         + 'climb rate in ft/min. If the value\n'\
                         + 'specified exceeds the maximum for the\n'\
                         + 'aircraft, then that maximum will be used.\n'\
                         + 'Entry boxes with \'None\' do not apply.\n'
        elif (ModuleConstructor.WorkingModuleName == 'Enroute'):
            OtherMessage = 'For Enroute, the \'Other\' entry limits\n'\
                         + 'the climb or descent rate (in ft/min) to \n'\
                         + 'the specified value or less, unless the \n'\
                         + 'value is \'0\', in which case no limit \n'\
                         + 'is imposed. Text boxes with  \'None\'\n'\
                         + 'do not apply and should not be set.'
        elif (ModuleConstructor.WorkingModuleName == 'DelayInArea'):
            OtherMessage = 'For DelayInArea \'Size\' denotes\n'\
                 + 'the desired size of the working area during\n'\
                 + 'the delay and the \'Other\' entry gives the\n'\
                 + 'duration of the delay in hours. Text boxes\n'\
                 + 'with \'None\' do not apply and should not be\n'\
                 + 'set.'
        elif (ModuleConstructor.WorkingModuleName == 'OutAndBack'):
            OtherMessage = 'For OutAndBack \'Size\' denotes\n'\
                 + 'the desired length of the two legs and the\n'\
                 + '\'Angle\' entry gives the orientation of the\n'\
                 + 'outbound leg. Text boxes with \'None\' do not\n'\
                 + 'apply and should not be set.'
        elif (ModuleConstructor.WorkingModuleName == 'VerticalSection'):
            OtherMessage = 'For VerticalSection \'Size\' denotes\n'\
                 + 'the desired length of the legs and the\n'\
                 + '\'Angle\' entry gives the orientation of the\n'\
                 + 'outbound legs. The \'Other\' entry gives the\n'\
                 + 'altitude change (in ft) between consecutive legs,\n'\
                 + 'where the entry may be negative for a descent\n'\
                 + 'between legs. \'Size\' and \'Angle\' specify the\n'\
                 + 'length and orientation of the first leg. An even\n'\
                 + 'number of legs should be used to end at the start\n'\
                 + 'point.'
        elif (ModuleConstructor.WorkingModuleName == 'Sawtooth'):
            OtherMessage = 'For a Sawtooth pattern, \'Size\' is the\n'\
                 + 'full length of the leg, \'alt\' is the target\n'\
                 + 'altitude above or below the starting altitude,\n'\
                 + 'and \'Other\' (if > 0) limits the rate of climb.'
        elif (ModuleConstructor.WorkingModuleName == 'StairStep'):
            OtherMessage = 'For a StairStep pattern, \'Size\' is the\n'\
                 + 'full length of the pattern, \'Other\' is the change\n'\
                 + 'in altitude at each stairstep, and \'legs\' is the,\n'\
                 + 'number of steps; e.g., \'3\' -> 3 steps, 4 level legs.'
        elif (ModuleConstructor.WorkingModuleName == 'SpiralProfile'):
            OtherMessage = 'For SpiralProfile \'Other\' denotes\n'\
                 + 'the desired rate of climb or descent. The pattern\n'\
                 + 'will give a transit to the specified location and\n'\
                 + 'then climb or descent to the specified altitude.\n'\
                 + 'Text boxes with \'None\' do not apply and should\n'\
                 + 'not be set.'
        elif (ModuleConstructor.WorkingModuleName == 'Circle'):
            OtherMessage = 'For Circle \'Size\' denotes the \n'\
                 + 'diameter of the circle that will start at the\n'\
                 + 'specified start point. \'Angle\' refers to the\n'\
                 + 'initial direction at the start.  The required\n'\
                 + 'turn rate will be calculated, and if it exceeds\n'\
                 + 'the maximum for the aircraft then the size will\n'\
                 + 'be adjusted to the smallest possible value. The\n'\
                 + '\'Other\' entry is 1 for a circle turning to\n'\
                 + 'the right, -1 for turning to the left.'
        elif (ModuleConstructor.WorkingModuleName == 'Racetrack'):
            OtherMessage = 'For Racetrack \'Size\' denotes the \n'\
                 + 'length of the first leg, flown in direction\n'\
                 + '\'Angle\' (degrees) and \'Legs\' gives the\n'\
                 + 'number of legs, normally an even number. The\n'\
                 + '\'Other\' entry gives the distance of the offset\n'\
                 + 'between legs of the racetrace (in n mi), where\n'\
                 + 'a positive value denotes right turns and a\n'\
                 + 'negative value left turns for the racetrack.'
        elif (ModuleConstructor.WorkingModuleName == 'Lawnmower'):
            OtherMessage = 'For Lawnmower \'Size\' denotes the \n'\
                 + 'length of the first leg, flown in direction\n'\
                 + '\'Angle\' (degrees), and \'Legs\' gives the\n'\
                 + 'number of legs, normally an even number. The\n'\
                 + '\'Other\' entry gives the distance of the offset\n'\
                 + 'between legs of the pattern (in n mi), where\n'\
                 + 'a positive value indicates that the first turn\n'\
                 + 'is to the right.'
        elif (ModuleConstructor.WorkingModuleName == 'SquareSpiral'):
            OtherMessage = 'For SquareSpiral \'Size\' denotes the \n'\
                 + 'length of the first leg, flown in direction\n'\
                 + '\'Angle\' (degrees), and \'Legs\' should be\n'\
                 + '10 * dir1-legs + dir2-legs (i.e., 43 indicates \n'\
                 + '4 legs in the original and 3 in the orthogonal\n'\
                 + 'direction). \'Other\' indicates the direction\n'\
                 + 'of the first turn (e.g., +90 for a right turn).'
        elif (ModuleConstructor.WorkingModuleName == 'Figure4'):
            OtherMessage = 'For Figure4 \'Size\' denotes the \n'\
                 + 'length of the first leg, flown in direction\n'\
                 + '\'Angle\' (degrees). Text boxes with \'None\'\n'\
                 + 'do not apply and should not be set.'
        elif (ModuleConstructor.WorkingModuleName == 'Figure8'):
            OtherMessage = 'For Figure8 \'Size\' denotes the \n'\
                 + 'length of the pattern, oriented in direction\n'\
                 + '\'Angle\' (degrees). The \'Other\' entry is\'\n'\
                 + 'the altitude change after pairs of legs. The\n'\
                 + 'pattern is useful for loitering at a location.'
        elif (ModuleConstructor.WorkingModuleName == 'Butterfly'):
            OtherMessage = 'For Butterfly \'Size\' denotes the \n'\
                 + 'length of the diagonal legs, the first oriented\n'\
                 + 'in direction\'Angle\' (degrees). The \'Other\'\n'\
                 + 'entry is the altitude change after sets of legs.\n'\
                 + '\'Legs\' indicates the number of times the set\n'\
                 + 'of four legs that constitues the pattern is\n'\
                 + 'to be repeated.'
        elif (ModuleConstructor.WorkingModuleName == 'ThreeBlade'):
            OtherMessage = 'For ThreeBlade \'Size\' denotes the \n'\
                 + 'length of the first leg through the center of\n'\
                 + 'the pattern, flown in the directionn\'Angle\'\n'\
                 + '(degrees). The pattern connects the vertices of\n'\
                 + 'a hexagon. Text boxes with \'None\'do not apply\n'\
                 + 'and should not be set.'
        elif (ModuleConstructor.WorkingModuleName == 'ReturnToBase'):
            OtherMessage = 'For ReturnToBase there are no parameters\n'\
                 + 'and the text boxes should not be set.'
        else:
            OtherMessage = 'This module is missing.'
        d = wx.MessageDialog (self, OtherMessage,\
                                    "Specifications For This Module",\
                                     wx.OK)
#                                    wx.OK|wx.ICON_INFORMATION)
        d.ShowModal ()
        d.Destroy ()

    def OnSaveTrack (self, event): 
        'Save the defined track for possible re-use.'
#       save Specs (including format code) as part of track:
        ModuleConstructor.Track['Specs'] = Specs.Specs
#       ask for a descriptive comment and save it with the track
        d = wx.TextEntryDialog ( None, \
            'Description to save with this track:'\
           + '\n   Suggestions:\n     Date, User, Project,' \
           + '\n     Flight Objective (s)',
                'Track Description', '(please replace this text)' )
        if d.ShowModal () == wx.ID_OK:
            ModuleConstructor.Track['Comment'] = d.GetValue ()
        d.Destroy ()
        print "Comment is: %s" % ModuleConstructor.Track['Comment']
        ModuleConstructor.Track['Fuel'] = ModuleConstructor.Fuel
        ModuleConstructor.Track['Time'] = ModuleConstructor.Time
        ModuleConstructor.Track['AC'] = ModuleConstructor.AC
        ModuleConstructor.Track['FlightDirection'] \
                               = ModuleConstructor.FlightDirection
        ModuleConstructor.Track['ModuleNumber'] \
                               = ModuleConstructor.ModuleNumber
        ModuleConstructor.Track['ManeuverNumber'] \
                               = ModuleConstructor.ManeuverNumber
        dlg = wx.FileDialog (self, "Provide a file name:", \
                             os.getcwd (), "Plan.xxx", "Plan.*", wx.SAVE)
        if dlg.ShowModal () == wx.ID_OK:
            path = dlg.GetPath ()
            mypath = os.path.basename (path)
            self.SetStatusText ("You selected: %s" % mypath, 1)
#           print "path = ",path
#           print "file is ", mypath
            TrackFile = open (path, 'wb')
            pickle.dump (ModuleConstructor.Track, TrackFile)
            TrackFile.close ()
        dlg.Destroy ()

    def OnAbout (self, event):
        'Short message stating the purpose of the program.'
        d = wx.MessageDialog (self, \
            " Creates A Flight Plan \n (text and kml)"\
            + "\nContact: Al Cooper (cooperw@ucar.edu)"\
            + "\n[UCAR policy asks for this, so:]"\
            + "\nCopyright 2012 UCAR", " PlanFlight", wx.OK)
        d.ShowModal ()
        d.Destroy ()
    def OnExit (self, event):
        'Close all windows and exit.'
        self.Close (True)
        if frame2 != None and frame2: frame2.Destroy ()
        if frame3 != None and frame3: frame3.Destroy ()
        if frame4 != None and frame4: frame4.Destroy ()
        if frame5 != None and frame5: frame5.Destroy ()
        if frame6 != None and frame6: frame6.Destroy ()
        if frame7 != None and frame7: frame7.Destroy ()
        if frame8 != None and frame8: frame8.Destroy ()
        if frame9 != None and frame9: frame9.Destroy ()
        if frame10 != None and frame10: frame10.Destroy ()
        if frame11 != None and frame11: frame11.Destroy ()
        if frame12 != None and frame12: frame12.Destroy ()
        if frame13 != None and frame13: frame13.Destroy ()

    def OnOpen (self, event): 
        dlg = wx.FileDialog (self, "Choose a file", os.getcwd (), \
                             "Plan", "Plan.*", wx.OPEN)
        if dlg.ShowModal () == wx.ID_OK:
            path = dlg.GetPath ()
            mypath = os.path.basename (path)
            self.SetStatusText ("You selected: %s" % mypath, 1)
#           print "path = ",path
#           print "file is ", mypath
            ReadTrack (path)
        dlg.Destroy ()

    def OnAppend (self, event): 
        dlg = wx.FileDialog (self, "Choose a file", os.getcwd (), \
                             "Plan", "Plan.*", wx.OPEN)
        if dlg.ShowModal () == wx.ID_OK:
            path = dlg.GetPath ()
            mypath = os.path.basename (path)
            self.SetStatusText ("You selected: %s" % mypath, 1)
            ExistingTrack = ModuleConstructor.Track
            ReadTrack (path)
            NewTrack = ModuleConstructor.Track
            ModuleConstructor.Track = ExistingTrack
				# find the highest module number
            KeyList = ModuleConstructor.Track.keys ()
            KeyList.sort ()
            for ky in KeyList:
                if ('Module' in ky and 'Number' not in ky):
                    kk = int (ky.replace ('Module',''))
            KeyList = NewTrack.keys ()
            KeyList.sort ()
            for ky in KeyList:
                if ('Module' in ky and 'Number' not in ky):
                    m = NewTrack[ky]
                    kk += 1
                    key = 'Module' + format(kk, '03d')
                    ModuleConstructor.Track[key] = m
            Recalculate ()
        dlg.Destroy ()

    def OnSave (self, event): 
        "Save the text in 'listing' to a file."
        dlg = wx.FileDialog (self, "Provide a file name:", \
                             os.getcwd (), "", "*.*", wx.SAVE)
        if dlg.ShowModal () == wx.ID_OK:
            path = dlg.GetPath ()
            mypath = os.path.basename (path)
            self.SetStatusText ("You selected: %s" % mypath, 1)
            f = open (path, 'wb')
            pickle.dump (NavItems, f)
            f.close ()
        dlg.Destroy ()

    def OnPrint (self, event): 
        'Prints output from various frames; one function prints '\
        + 'various frames by checking calling routine.'
        CallingID = event.GetId ()
        if CallingID == ID_PT2:
            self.printer = Printer (self)
            self.printer.Print (frame3.listing, 'Track Definition')
        elif CallingID == ID_PT8:
            self.printer = Printer (self)
            self.printer.Print (frame8.listing, 'Instructions')
        elif CallingID == ID_PT9:
            self.printer = Printer (self)
            self.printer.Print (frame9.hint, 'Hints')
        elif CallingID == ID_PT11:
            self.printer = Printer (self)
            self.printer.Print (frame11.listing, 'Sounding')
        elif CallingID == ID_PT:
            self.printer = Printer (self)
            self.printer.Print (frame4.listing, 'Waypoints')
    # end of OnPrint definition

    def OnList (self, event): 
        'Makes a new frame with listing of waypoints, and can print or '\
         + 'save to file.'
        global frame4
        KeyList = ModuleConstructor.Track.keys ()
        KeyList.sort ()
        alast = {}
        KML.WaypointNumber = 0
        LastTime = 0
        listing = 'Waypoint Listing:\n\n'
        for ky in KeyList:
            if ('Module' in ky and 'Number' not in ky):
                kk = int (ky.replace ('Module',''))
                m = ModuleConstructor.Track[ky]
#               print m['Time'],'= Module time'
#               print"Module " + format (kk, 'd') + ' (ending at '\
#                        + format (m['Time'], '.1f')\
#                        + ' h) = ' \
#                        + format (m['Type'], 's') + '\n'
                listing += "Module " + format (kk, 'd') + '(ending at '\
                         + format (m['Time'], '.1f')\
                         + ' h) = ' \
                         + format (m['Type'], 's') + '\n'
                KeyListManeuvers = m.keys ()
                KeyListManeuvers.sort ()
                for kym in KeyListManeuvers:
                    if ('Manvr' in kym):
                        kk = int (kym.replace ('Manvr',''))
                        mn = m[kym]
#					skip turns
#                       if 'Turn' not in mn['Type']:
                        if 'ZYXW' not in mn['Type']:
                            listing +=  "    Maneuver " \
                                     + format (kk, 'd') + ' = ' \
                                     + format (mn['Type'], 's')
                            if 'Turn' in mn['Type']:
                                listing += ' to ' \
                                     + format (mn['Orientation'], '.0f')\
                                     + ' deg true'
                            elif 'Delay' in mn['Type']:
                                dtime = mn['Time'] - LastTime
                                listing += ' for ' \
                                     + format (dtime, '.1f')\
                                     + ' h'
                            else:
                                listing += ' toward ' \
                                     + format (mn['Orientation'], '.0f')\
                                     + ' deg true'
                            listing  += '\n'
                            al = mn['Anchor']
                            if (al != alast):
#                               convert to deg and decimal min:
                                degLon, minLon = ConvertToDegMin (al[0])
                                degLat, minLat = ConvertToDegMin (al[1])
                                KML.WaypointNumber += 1
                                listing += \
    "        Waypoint" + format (KML.WaypointNumber, '03d') \
    +  '    ' + format (degLat, 'd') + '  ' + format (minLat, '.1f') \
    + ' , ' + format (degLon, 'd') + '  ' \
    + format (minLon, '.1f') + ' , ' + format (al[2], '.0f') + '\n'
                                alast = al
                            an = mn['EndPoint']
                            if (an != alast):
#                               convert to deg and decimal min:
                                degLon, minLon = ConvertToDegMin (an[0])
                                degLat, minLat = ConvertToDegMin (an[1])
                                KML.WaypointNumber += 1
                                listing += \
    "        Waypoint" + format (KML.WaypointNumber, '03d')\
    +  '    ' + format (degLat, 'd') + '  ' + format (minLat, '.1f') \
    + ' , ' + format (degLon, 'd') + '  ' + format (minLon, '.1f')\
    + ' , ' + format (an[2], '.0f') + '\n'
                                alast = an
                            # end of search for waypoints in maneuver
                            LastTime = mn['Time']
                        # end of branch for not Turn
                    # end of Maneuver procesing
                # end of loop over Maneuver keys
            # end of module processing
        # end of loop over Track keys
        if frame4 != None and frame4: frame4.Close ()
        frame4 = WaypointFrame (listing)
        frame4.Show ()
    # end of OnPrint definition

    def OnShow (self, event): 
        'Display the constructed track in a plot.'
        global frame13
				# construct a list of waypoints
        KeyList = ModuleConstructor.Track.keys ()
        KeyList.sort ()
        alast = {}
        xp = []
        yp = []
        mtype = []
        anchor = []
        K = 0
        for ky in KeyList:
            if ('Module' in ky and 'Number' not in ky):
                m = ModuleConstructor.Track[ky]
                KeyListManeuvers = m.keys ()
                KeyListManeuvers.sort ()
                for kym in KeyListManeuvers:
                    if 'Manvr' in kym:
#                       kk = int (kym.replace ('Manvr', ''))
                        mn = m[kym]
                                  #  skip turns
                        if 'Turn' not in mn['Type']:
                            al = mn['Anchor']
                            if (al != alast):
                                xp.append (al[0])
                                yp.append (al[1])
                                mtype.append (m['Type'])
                                anchor.append (m['Anchor'])
#                               convert to deg and decimal min:
                                K += 1
                                alast = al
                            an = mn['EndPoint']
                            if (an != alast):
                                xp.append (an[0])
                                yp.append (an[1])
                                mtype.append (m['Type'])
                                anchor.append (m['Anchor'])
                                K += 1
                                alast = an
                            # end of search for waypoints in maneuver
                        # end of branch for maneuver not Turn
                    # end of Maneuver procesing
                # end of loop over Maneuver keys
            # end of module processing
        # end of loop over Track keys
        pylab.clf ()			# clear the plot
        pylab.plot(xp, yp)
				# get transformation from cursor coords
				# to lat/lon coordinates:
        xmin,xmax,ymin,ymax=pylab.axis()
        self.xzero = xmin
        self.yzero = ymin
        self.xmag = (xmax - xmin) / 620.	#620 = cursor-coords
        self.ymag = (ymax - ymin) / 480.	#480 = cursor-coords
        xdel = (xmax - xmin) / 50.
        ydel = (ymax - ymin) / 50.
        mlast = ''
        mindex = 0
        letters = ['A','B','C','D','E','F','G','H','I','J','K','L','M']
        for i in range (len(xp)):
            pylab.plot (xp[i], yp[i], 'bo', color='red')
            pylab.annotate(str(i+1), (xp[i],yp[i]), \
                       (xp[i]-xdel,yp[i]+ydel), color='red')
            if mtype[i] != mlast:
                xp[i] = anchor[i][0]
                yp[i] = anchor[i][1]
                pylab.annotate(letters[mindex], \
                       (xp[i], yp[i]), \
                       (xp[i]-2.*xdel,yp[i]+2.*ydel), color='green')
                pylab.figtext (\
                       0.65, \
                       0.90-0.05*mindex,\
                       letters[mindex]+' '+mtype[i], color='green')
                mindex += 1
                mlast = mtype[i]
            #end of module-type loop
        #end of waypoint loop
				# add VORs to plot
        vlines = open ('./NavPoints/VOR.txt', 'r').readlines ()
        for i in range (len(vlines)):
            pts = string.split (vlines[i], ',')
            pts[1] = float (pts[1])
            pts[2] = float (pts[2])
            pts[4] = float (pts[4])
            if pts[2] > ymin and pts[2] < ymax:
                if pts[1] > xmin and pts[1] < xmax:
                    pylab.plot(pts[1], pts[2], 'bo', color='lightgrey')
                    pylab.annotate (pts[0], (pts[1], pts[2]),\
                                    (pts[1]-xdel, pts[2]+ydel), color='lightgrey')
        pylab.xlabel('Longitude')
        pylab.ylabel('Latitude')
        pylab.title('Current Track')
        pylab.grid(True)
        pylab.savefig('Plan')
#       pylab.show()
        pylab.clf ()
        pylab.close ()
        if frame13 != None and frame13: frame13.Close ()
        frame13 = PlotFrame ()
        frame13.Show ()

    def OnEdit (self, event): 
        'Display the specifications for possible modification.'
        global frame2
        frame2 = SpecsFrame ()
        frame2.Show ()

    def OnGet (self, event):
        "Fetch latest sounding from the UWyo archives."
        import datetime
        from time import sleep

        now = datetime.datetime.now ()
        if now.hour < 8: h = 0
        else: h = 12
        dh = format (now.day, '02d') + format (h, '02d')
# station 72469 is the DNR sounding:
        http = \
'http://weather.uwyo.edu/cgi-bin/sounding?region=naconf&TYPE=TEXT%3ALIST&YEAR=' + str (now.year) + '&MONTH=' + format (now.month, '02d') + '&FROM=' + dh + '&TO='+dh + '&STNM=72469'

        cmd = 'wget --output-document=./Sounding.UWyo ' + '\'' \
            + http + '\''
        result = True
        while result:
            os.system (cmd)
            sleep (2.0)
            statinfo = os.stat ('./Sounding.UWyo')
            if statinfo.st_size < 1000:
                dlg = wx.MessageDialog (self,\
                'Download rejected by server.\nClick OK to try again,\n'\
                 + 'Cancel to proceed without downloading' , \
                 'Download Error', wx.OK|wx.CANCEL|wx.ICON_QUESTION)
                r = dlg.ShowModal ()
                if r == wx.ID_OK: result = True
                else: result = False
                dlg.Destroy ()
            else: 
                result = False
                os.system ('mv Sounding.UWyo Sounding')

    def SetParameters (self, event): pass


    def OnIC (self, event): pass
    def OnTR (self, event): pass
    def OnOB (self, event): pass
    def OnVS (self, event): pass
    def OnLM (self, event): pass
    def OnRT (self, event): pass
    def OnDA (self, event): pass
    def OnMD (self, event): pass
    def OnSV (self, event): pass
    def OnSA (self, event): pass

    def OnIS (self, event): 		# insert a module
        'Starting with currently selected module, move all modules up '\
         + 'so AddModule inserts a new module into the track.'
# note that this effectively duplicates the currently defined module 
# if no changes are made. The current module can be changed after 
# copying (to insert something new) or left (which implements a copy 
# function).
        if ModuleConstructor.ModuleNumber == 0: return () 
        KeyList = ModuleConstructor.Track.keys ()
        KeyList.sort (reverse=True)
        for Key in KeyList:
            if ('Module' in Key and 'Number' not in Key):
                kk = int (Key.replace ('Module',''))
                if kk > ModuleConstructor.ModuleNumber :
                    mx = ModuleConstructor.Track[Key]
                    newKey = 'Module' + format (kk+1, '03d')
                    ModuleConstructor.Track[newKey] = mx
            #end of Module loop
        #end of Key loop
        Recalculate ()

    def OnDX (self, event): 
        "Deletes a module by changing the module keys to move down "\
        + "from selection to top"
        if ModuleConstructor.ModuleNumber == 0: return () 
        KeyList = ModuleConstructor.Track.keys ()
        KeyList.sort ()
        for Key in KeyList:
            if ('Module' in Key and 'Number' not in Key):
                kk = int (Key.replace ('Module',''))
                if kk > ModuleConstructor.ModuleNumber + 1 :
                    mx = ModuleConstructor.Track[Key]
                    newKey = 'Module' + format (kk-1, '03d')
                    ModuleConstructor.Track[newKey] = mx
#			remove old item; this only matters for the last
                    del ModuleConstructor.Track[Key]
            #end of Module loop
        #end of Key loop 
        Recalculate ()
#				now show the new item:
        MainWindow.OnSpin (self, event)
        
    def OnReorder (self, event):
        "Event handler for Reorder; displays frame for defining order."
        global frame5
#       	# Ask for desired order, then recalculate in this order.
        if frame5 != None and frame5: frame5.Close ()
        frame5 = ReorderFrame ()
        frame5.Show ()
        
    def OnRecalculate (self, event):
        'Calls the \'Recalculate\' function to regenerate the track '\
         + 'from the module definitions.'
        Recalculate ()

    def OnNav (self, event):
        'Generates and displays the navigation frame.'
        global frame7
        if frame7 != None and frame7: frame7.Close ()
        frame7 = NavFrame ()
        frame7.Show ()

    def OnMap (self, event):
        'Generates and displays the map displays.'
        global frame6
        if frame6 != None and frame6: frame6.Close ()
        frame6 = MapFrame ()
        frame6.Show ()
  
    def OnSR (self, event):
        'Displays specific range charts for the GV.'
        global frame10
        if frame10 != None and frame10: frame10.Close ()
        frame10 = SRFrame ()
        frame10.Show ()
				# also, generate a pylab plot
        altitude = ModuleConstructor.AC[2]
        GW = Specs.GrossWeight () / 1000.
				# get desired altitude and GW
        dlg = wx.TextEntryDialog (self, 'Gross Weight:',\
                                  'Specific Range Parameters')
        dlg.SetValue (format (GW*1000., '.0f'))
        if dlg.ShowModal () == wx.ID_OK:
            GW = float (dlg.GetValue ()) / 1000.
            self.SetStatusText ('You entered: %s\n' % GW)
        dlg = wx.TextEntryDialog (self, 'Altitude:',\
                                  'Specific Range Parameters')
        dlg.SetValue (format (altitude, '.0f'))
        if dlg.ShowModal () == wx.ID_OK:
            altitude = float (dlg.GetValue ())
            self.SetStatusText ('You entered: %s\n' % GW)
				# find the corresponding Mach No:
        MachNo = Specs.MachNumber (altitude)
        Altitudes = Specs.SRdata.keys ()
        Altitudes.sort ()
        ix_alt, f_alt = Specs.Interp (Specs.SRdata, altitude)
				# f_alt is fraction of [ix_alt] to use
				# get the two altitude dictionaries:
        G1 = Specs.SRdata[Altitudes[ix_alt]]
        G0 = Specs.SRdata[Altitudes[ix_alt-1]]
				# now find interpolation factor in each
				# gross-weight dictionary:
        keysGW1 = G1.keys ()
        keysGW1.sort ()
        ig1, f_GW1 = Specs.Interp (G1, GW)
				# get the bracketing Mach numbers
        M1G1 = G1[keysGW1[ig1]]
        M0G1 = G1[keysGW1[ig1-1]]
        im, f_Mach = Specs.Interp (M1G1, MachNo)
#       print 'Mach index', im, f_Mach, ig1, f_GW1, ix_alt, f_alt
        k1G1 = M1G1.keys ()
        k1G1.sort ()
        sr1 = M1G1[k1G1[im-1]] * (1. - f_Mach) + f_Mach * M1G1[k1G1[im]]
        im, f_Mach = Specs.Interp (M0G1, MachNo)
        k0G1 = M0G1.keys ()
        k0G1.sort ()
        sr0 = M0G1[k0G1[im-1]] * (1. - f_Mach) + f_Mach * M0G1[k0G1[im]]
				# interpolate in gross weight
        srZ1 = sr0 + f_GW1 * (sr1 - sr0)
				# repeat for other altitude
        keysGW0 = G0.keys ()
        keysGW0.sort ()
        ig0, f_GW0 = Specs.Interp (G0, GW)
				    # get the bracketing Mach numbers
        M1G0 = G0[keysGW0[ig0]]
        M0G0 = G0[keysGW0[ig0-1]]
        im, f_Mach = Specs.Interp (M1G0, MachNo)
        k1G0 = M1G0.keys ()
        k1G0.sort ()
        sr1 = M1G0[k1G0[im-1]] * (1. - f_Mach) + f_Mach * M1G0[k1G0[im]]
        im, f_Mach = Specs.Interp (M0G0, MachNo)
        k0G0 = M0G0.keys ()
        k0G0.sort ()
        sr0 = M0G0[k0G0[im-1]] * (1. - f_Mach) + f_Mach * M0G0[k0G0[im]]
				# interpolate in gross weight
        srZ0 = sr0 + f_GW0 * (sr1 - sr0)
				# finally, interpolate in altitude.
				# (bilinear interpolation in GW and 
				# altitude would be better...)
        sr = srZ0 + f_alt * (srZ1 - srZ0)
				# compensate for departure from ISA
        sr *= (1. - 0.0007 * Specs.ISADelta (altitude))
				# This is SR at flight conditions.
        sr_actual = sr
				# Now do plot vs Mach Number.
				# (same altitude and GW, so f_GW1, 
				#  f_GW2,  and f_alt, ig1, ig2, ix_alt 
				#  stay the same)
				# Find region of overlap between two
				# Mach ranges for bracketing GWs and
				# bracketing altitudes:
        imin = 0
        imax = len (k0G0) - 1
#       print k0G0[imin], k0G0[imax]
        imax01 = len (k0G1) - 1
        imax10 = len (k1G0) - 1
        imax11 = len (k1G1) - 1
        while float (k0G0[imin]) < float (k1G0[0]): imin += 1
        while float (k0G0[imin]) < float (k0G1[0]): imin += 1
        while float (k0G0[imin]) < float (k1G1[0]): imin += 1
        while float (k0G0[imax]) > float (k1G0[imax10]): imax -= 1
        while float (k0G0[imax]) > float (k0G1[imax01]): imax -= 1
        while float (k0G0[imax]) > float (k1G1[imax11]): imax -= 1
#       print imin, imax, imax01, imax10, imax11
#       print k0G0[imin], k0G0[imax]
#       print k0G1[0], k0G1[imax01]
#       print k1G0[0], k1G0[imax10]
#       print k1G1[0], k1G1[imax11]
        xp = []
        yp = []
        for i in range (imin, imax + 1):
            ky = k0G0[i]
            mach = float (ky)
#           print i, mach
            srZ1 = M0G1[ky] + f_GW1 * (M1G1[ky] - M0G1[ky])
            srZ0 = M0G0[ky] + f_GW0 * (M1G0[ky] - M0G0[ky])
            sr = srZ0 + f_alt * (srZ1 - srZ0)
				# compensate for departure from ISA
            sr *= (1. - 0.0007 * Specs.ISADelta (altitude))
            xp.append (mach)
            yp.append (sr)
        pylab.clf ()			# clear the plot
        pylab.plot(xp, yp)
        pylab.plot(MachNo, sr_actual, 'bo')
        pylab.xlabel('Mach No.')
        pylab.ylabel('Specific Range (n mi / lb)')
        GW *= 1000.
        pylab.title('Specific Range, GW = ' + format (GW, '.0f') \
                   + ' lb and Z = ' + format (altitude, '.0f'))
        pylab.grid(True)
#       pylab.savefig('SpecificRange')
        pylab.show()


  
    def OnCP (self, event):
        'Displays the climb profile for the GV.'
        tpp = 0.
        zpp = ModuleConstructor.AC[2]
        Specs.ceiling = 0.
        tp = [0.]
        zp = [zpp]
        Zt = 51000.
        Za = zpp
        Zb = math.floor ((Za + 1000.) / 1000.) * 1000.
        while Zb <= Zt:
            delta = Specs.DeltaClimb (Za, Zb)
            if delta[0] <= 0.: break
            tpp += delta[0] * 60.	# convert to minutes
#           print "DeltaClimb step, Za=%.0f, Zb=%.0f, tpp=%f" % (Za, Zb, tpp)
            tp.append (tpp)
            zp.append (Zb)
            Za = Zb
            Zb += 1000.
        pylab.clf ()			# clear the plot
        pylab.plot(tp, zp)
        pylab.xlabel('time (min)')
        pylab.ylabel('altitude (ft)')
        GW = Specs.GrossWeight()
        pylab.title('GV climb profile, GW = ' + format (GW, '.0f') \
                   + ' lb')
        pylab.grid(True)
#       pylab.savefig('ClimbProfile')
        pylab.show()
#       global frame10
#       if frame10 != None and frame10: frame10.Close ()
#       frame10 = CPFrame ()
#       frame10.Show ()
  
    def OnTAS (self, event):
        'Displays the TAS vs altitude, considering the atmospheric sounding.'
        yp = []; xp = []; xpT = []
        for altitude in range (0,50000,200):
            xp.append (Specs.FlightSpeed (altitude))
            xpT.append (Specs.TurbulenceFlightSpeed (altitude))
            yp.append (altitude)
        pylab.clf ()			# clear the plot
        pylab.plot(xp, yp)
        pylab.plot (xpT, yp, 'red')
        pylab.annotate('normal', (400,19000), (410,17000), \
                       color='blue')
        pylab.annotate('if turbulent', (400,24500), (340,28000), \
                       arrowprops=dict(arrowstyle="->", color='red'),\
                       color='red')
        pylab.xlabel('TAS [kts]')
        pylab.ylabel('altitude (ft)')
        pylab.title('Flight Speed vs Altitude')
        pylab.grid(True)
#       pylab.savefig('TAS')
        pylab.show()

    def OnDP (self, event):
        'Displays the descent profile for the GV.'
        tpp = 0.
        zpp = ModuleConstructor.AC[2]
        tp = [0.]
        zp = [zpp]
        Zt = Specs.TakeoffLocation()[2]
        Za = zpp
        Zb = math.floor ((Za - 1000.) / 1000.) * 1000.
        while Zb >= Zt:
            delta = Specs.DeltaDescent (Za, Zb)
            if delta[0] <= 0.: break
            tpp += delta[0] * 60.	# convert to minutes
            print "DeltaDescent step, Za=%.0f, Zb=%.0f, tpp=%f" % (Za, Zb, tpp)
            tp.append (tpp)
            zp.append (Zb)
            Za = Zb
            Zb -= 1000.
        if Zb > Zt:
            delta = Specs.DeltaDescent (Zb, Zt)
            tpp +- delta[0] * 60.
            tp.append (tpp)
            zp.append (Zt)
        pylab.clf ()			# clear the plot
        pylab.plot(tp, zp)
        pylab.xlabel('time (min)')
        pylab.ylabel('altitude (ft)')
        GW = Specs.GrossWeight()
        pylab.title('GV max descent profile to airport, GW = ' \
                    + format (GW, '.0f') + ' lb')
        pylab.grid(True)
#       pylab.savefig('DescentProfile')
        pylab.show()
#       pdb.set_trace ()
#       global frame10
#       if frame10 != None and frame10: frame10.Close ()
#       frame10 = CPFrame ()
#       frame10.Show ()
  
    def OnSND (self, event):
        'Displays the sounding in use.'
        global frame11
        f = open ('./Sounding','rb')
        listing = 'Sounding In Use:\n\n\n'
        enable = False
        for line in f:
			# add a pressure altitude
            work = line.split()
            if len (line) < 10: pass
            elif '--------' in line: enable = True
            elif 'PRES' in work[0]: line = '  PALT   ISA '+line
            elif 'hPa' in work[0]: 
                line = '   ft   degC'+line
                line = line.replace ('m', 'ft')
            elif '<' in work[0]: enable = False
            elif enable:
                p = float (work[0])
                if p > 226.3206:
                    alt = 145442.156 * (1. - (p / 1013.25)**0.1902632)
                else:
                    alt = 36089.24 + 47907.22 * math.log10 (226.3206 / p)
				# given pressure altitude, find ISA dev.
                delT = Specs.ISADelta (alt)
                z = float (work[1])
                if z < 10000.: work[1] = ' ' + work[1]
                if z < 1000.: work[1] = ' ' + work[1]
                if z < 100.: work[1] = ' ' + work[1]
                if z < 10.: work[1] = ' ' + work[1]
                z = format (float (work[1]) / 0.3048, '5.0f')
                line = line.replace (work[1], z)
                line = format (alt, '6.0f') + format (delT, '+6.1f') + line
            line = line.replace ('<H2>', '')
            line = line.replace ('</H2>', '')
            if '<' not in line: listing += line
        if frame11 != None and frame11: frame11.Close ()
        frame11 = SNDFrame (listing)
        frame11.Show ()

# here are a few unused callback routines that were included here
# while the program was being developed. They can be removed now.
    def choosecolor (self, event):
        dlg = wx.ColourDialog (self)
        dlg.GetColourData ().SetChooseFull (True)
        if dlg.ShowModal () == wx.ID_OK:
            data = dlg.GetColourData ()
            self.SetStatusText ('Selected: %s\n' % \
                                str (data.GetColour ().Get ()))
        dlg.Destroy ()

    def openfile (self, event):
       dlg = wx.FileDialog (self, "Choose a file", os.getcwd (), "",\
                            "*.*", wx.OPEN)
       if dlg.ShowModal () == wx.ID_OK:
                path = dlg.GetPath ()
                mypath = os.path.basename (path)
                self.SetStatusText ("Selected: %s" % mypath)
       dlg.Destroy ()

    def pagesetup (self, event):
        dlg = wx.PageSetupDialog (self)
        if dlg.ShowModal () == wx.ID_OK:
            data = dlg.GetPageSetupData ()
            tl = data.GetMarginTopLeft ()
            br = data.GetMarginBottomRight ()
            self.SetStatusText ('Margins are: %s %s' \
                                % (str (tl), str (br)))
        dlg.Destroy ()

    def choosefont (self, event):
        default_font = wx.Font (10, wx.SWISS , wx.NORMAL, \
                                wx.NORMAL, False, "Verdana")
        data = wx.FontData ()
        if sys.platform == 'win32':
            data.EnableEffects (True)
        data.SetAllowSymbols (False)
        data.SetInitialFont (default_font)
        data.SetRange (10, 30)
        dlg = wx.FontDialog (self, data)
        if dlg.ShowModal () == wx.ID_OK:
            data = dlg.GetFontData ()
            font = data.GetChosenFont ()
            color = data.GetColour ()
            text = 'Face: %s, Size: %d, Color: %s' \
                   % (font.GetFaceName (), font.GetPointSize (),  \
                      color.Get ())
            self.SetStatusText (text)
        dlg.Destroy ()

    def lonentry (self, event):
        dlg = wx.TextEntryDialog (self, 'Base longitude, deg.',\
                                  'Base Location')
        dlg.SetValue ("-105.1172")
        if dlg.ShowModal () == wx.ID_OK:
            self.SetStatusText ('You entered: %s\n' % dlg.GetValue ())
        dlg.Destroy ()



    # end of event handlers
# end of MainWindow class

class TrackFrame (wx.Frame):
    def __init__(self, message):
        'Display window for showing the current track definition.'
        wx.Frame.__init__(self, None, -1,'Track Definition', \
                          size=(400,400))
        scrollWin = wx.PyScrolledWindow (self, -1)
        wx.Button (scrollWin, ID_PT2, 'Print', (240,8), (60,32))
        wx.Button (scrollWin, ID_FL2, 'Save', (305,8), (55,32))
        self.printer = Printer (frame)
        self.listing = message
        stTxt = wx.StaticText (scrollWin, -1, message, pos=(20,20))
        w, h = stTxt.GetSize ()
        scrollWin.SetScrollbars (0, 12, 0, h/12+2)
        scrollWin.SetScrollRate (1,12)
        self.Bind (wx.EVT_BUTTON, frame.OnPrint, id=ID_PT2)
        self.Bind (wx.EVT_BUTTON, frame.onFile, id=ID_FL2)
    #end __init__ def
#end TrackFrame class    

def ReadTrack (path):
    'Read a previously defined track from a storage file.'
    TrackFile = open (path, 'rb')
    ModuleConstructor.Track = pickle.load (TrackFile)
    TrackFile.close ()
#   print ModuleConstructor.Track
#	# 	Restore counters and initialize
    ModuleConstructor.ModuleNumber \
                     = ModuleConstructor.Track['ModuleNumber']
    ModuleConstructor.ManeuverNumber \
                     = ModuleConstructor.Track['ManeuverNumber']
    ModuleConstructor.FlightDirection \
                     = ModuleConstructor.Track['FlightDirection']
    ModuleConstructor.AC = ModuleConstructor.Track['AC']
    ModuleConstructor.Time = ModuleConstructor.Track['Time']
    ModuleConstructor.Fuel = ModuleConstructor.Track['Fuel']

def Recalculate ():
    "Use module definitions to recalculate maneuvers, entire track."
    global frame
#		Now take into account that, if module is being
#		edited, there may be subsequent modules that are
#		affected. Must loop through all subsequent
#		modules and reconstruct them to get Time and Fuel
#		to be correct.
#		Save and restore some ModuleConstructor variables:
    MCTime = ModuleConstructor.Time
    MCFuel = ModuleConstructor.Fuel
    MCFD   = ModuleConstructor.FlightDirection
    MCMN   = ModuleConstructor.ModuleNumber
    MCMV   = ModuleConstructor.ManeuverNumber
    MCAC   = ModuleConstructor.AC
    ModuleConstructor.Time = 0.
    ModuleConstructor.Fuel = Specs.FuelUseLimit ()
    ModuleConstructor.ManeuverNumber = 0
    ModuleConstructor.FlightDirection = 0
    ModuleConstructor.AC = Specs.TakeoffLocation ()
    KeyList = ModuleConstructor.Track.keys ()
    KeyList.sort ()
    for Key in KeyList:
        if ('Module' in Key and 'Number' not in Key):
                mx = ModuleConstructor.Track[Key]
                md = ModuleConstructor.Module (mx['Type'],\
                     mx['Dimension'], mx['Anchor'],\
                     mx['Orientation'],mx['Drift'], mx['Legs'],
                     mx['Other'], frame)
                md['Time'] = ModuleConstructor.Time
                ModuleConstructor.Track[Key] = md
        #end of Module loop
    #end of Key loop -- all modules now reconstructed
    ModuleConstructor.Track['Specs'] = Specs.Specs
# add counters and other information needed to restore the track:
    ModuleConstructor.Track['Fuel'] = ModuleConstructor.Fuel
    ModuleConstructor.Track['Time'] = ModuleConstructor.Time
    ModuleConstructor.Track['AC'] = ModuleConstructor.AC
    ModuleConstructor.Track['FlightDirection'] \
                           = ModuleConstructor.FlightDirection
    ModuleConstructor.Track['ModuleNumber'] \
                           = ModuleConstructor.ModuleNumber
    ModuleConstructor.Track['ManeuverNumber'] \
                           = ModuleConstructor.ManeuverNumber
    ModuleConstructor.Time = MCTime
    ModuleConstructor.Fuel = MCFuel
    ModuleConstructor.FlightDirection = MCFD
    ModuleConstructor.ModuleNumber = MCMN
    ModuleConstructor.ManeuverNumber = MCMV
    ModuleConstructor.AC = MCAC

def ReadNav (path):
    "Read saved file of navigation coordiantes, list in the Nav window."
    global NavItems
    try:
        NavFile = open (path, 'rb')
        NavItems =  pickle.load (NavFile)
        NavFile.close ()
    except IOError as e:
        dlg = wx.MessageDialog (frame,\
                'The standard navigation-point file is missing.\n'\
              + 'It can be created using the Save button on the\n'\
              + 'NavPoints menu and saving to \'Standard.nav\'.',\
                'File Missing', wx.OK)
        dlg.ShowModal ()
        dlg.Destroy ()


def ConvertToDegMin (x):
    "Usage: degrees, minutes = ConvertToDegMin (DecimalLat)"
    degrees = int (x)
    minutes = abs (round ((x - degrees) * 60., 1))
#		deal with points less than 1 deg S of equator
    if degrees == 0 and x < 0: minutes *= -1
    return (degrees, minutes)

def ConvertToDecDegrees (degrees, minutes):
    'Usage: DecimalLat = ConvertToDecDegrees (degrees, minutes)'
    x = degrees
    if x < 0.: x -= minutes / 60.
#	# x == 0 reaches 'else' case, so -ve minutes are treated right
    else: x += minutes / 60.
    return (x)

def main ():
    'Specify and edit plans for flight and produce coordinate '\
    + 'listings and .kml plot'
    global frame
    app = wx.PySimpleApp ()
    frame = MainWindow (None, -1, "PlanFlight")
#   frame.Center ()
    frame.Move ((0,0))
    frame.Show ()
    if len (sys.argv) > 1:
        ReadTrack (sys.argv[1])
#			Read the standard list of nav points
    ReadNav ('Standard.nav')
    app.MainLoop ()

if __name__ == '__main__':
    main ()

