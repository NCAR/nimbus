VERSION 5.00
Begin VB.Form frmNDP 
   AutoRedraw      =   -1  'True
   Caption         =   "JPL MTP - Number Density Profile"
   ClientHeight    =   3585
   ClientLeft      =   5970
   ClientTop       =   1455
   ClientWidth     =   5070
   Icon            =   "Ndp.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   3582.374
   ScaleMode       =   0  'User
   ScaleWidth      =   5068.59
End
Attribute VB_Name = "frmNDP"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Public NDPinitialize As Boolean, NDPmonth%
Public Bl!, Br!, Bt!, Bb!, xoffset!, yoffset!, Ystep!, Xstep!
Public XVl%, XVr%, XVs, XVc%, XVscale!, XVlast!, XVvalue!, XunitPerPixel!
Public YVl%, YVr%, YVs%, YVc%, YVscale!, YVlast!, FooterType%

Public T1%, T2%, PT1%, PT2%, T1K%, T2K%, PT1K%, PT2K%
Public T1def%, T2def%, PT1Def%, PT2Def%, T1KDef%, T2KDef%, PT1KDef%, PT2KDef%

Public Zt%, Zb%, ztkm%, zbkm%, ztkft%, zbkft%, ztmb%, zbmb%, zthPa%, zbhPa%
Public ztDef%, zbDef%, ztkmDef%, zbkmDef%, ztkftDef%, zbkftDef%, ztmbDef, zbmbDef%, zthPaDef, zbhPaDef%

Public zscale!, Tscale%, Nlev%, PCstart%

Public EnableAveraging As Boolean
Public ShowAircraft As Boolean, ShowTropopause As Boolean
Public NDPFileOpen As Boolean, ShowDocument As Boolean
Public Xcoord%, Ycoordl%, Ycoordr%, Xunit%, Yunitl%, Yunitr%
Public PT_flag, BadNr%

Public LRL%, LRP%, ACP%, ACS%, PBG%, PFG%, FHL%, FDL%, PTM%, Pgm%, PSL%, INL%
Public FastFwd As Boolean, FastBwd As Boolean
Public S_flag As Boolean, T_flag As Boolean, GM_flag As Boolean
Public LR_flag As Boolean, P_flag As Boolean, G_flag As Boolean
Public L_flag As Boolean, PlotDone As Boolean
Public AutoStep, AutoStepInterval%, FastFBInterval%
Public FooterTitle$, FooterFormat$
Public INPlu%, OUTlu%
Public PlotSingleScan As Boolean

'frmNDP variables
Public Taco!, jt1%, jt2%, jOut%
Public dZg!, Zt1!, Zt2!, ALTkm!, Tac!
Dim zcs!(1 To 99), Tcs!(1 To 99), zcss!(1 To 99), Tcss!(1 To 99), Zg!(1 To 33), Tg!(1 To 33), zgs!(1 To 99), Tgs!(1 To 99)

Sub NDP_Plot_Documentation()
Dim V!(1 To 20), FooterTitle$, FooterFormat$, ALTft&, HHMMSS&
Exit Sub

If frmNDP.ShowDocument Then
  frmNDP.FontBold = False
  frmNDP.ForeColor = QBColor(LGreen)
  If frmNDP.WindowState = vbMaximized Then frmNDP.FONTSIZE = frmNDP.FONTSIZE - 2
  frmNDP.CurrentX = XVl - Bl
  frmNDP.CurrentY = YVl - Bb / 2
'  frmNDP.Print FooterTitle$
  
  frmNDP.ForeColor = QBColor(White)
  frmNDP.CurrentX = XVl - Bl
  frmNDP.CurrentY = YVl - 3 * Bb / 4
'  frmNDP.Print fUsing$(FooterFormat$, v())
End If



End Sub


Sub SetDefaultScales()

Select Case Xcoord
Case 0 'Mass Density
  XVl = T1K: XVr = T2K
Case 1 'Number Density
  XVl = PT1K: XVr = PT2K
End Select

XVs = XVr - XVl: XVc = XVl + XVs / 2

Select Case Yunitl
Case 0   ' km
  YVl = zbkm: YVr = ztkm
'Case 1   ' kft
'  YVl = zbkft: YVr = ztkft
'Case 2   ' mb
'  YVl = zbmb: YVr = ztmb
'Case 3   ' hPa
'  YVl = zbhPa: YVr = zthPa
End Select

YVs = YVr - YVl: YVc = YVl + YVs / 2

End Sub



Sub NDPupdate()

  If PlotSingleScan Then
    SetDefaultScales
    NDP_Plot_Set
    NDP_Plot_Axes_Labels
    NDP_Plot_Grid_Marks
    NDP_Plot_Tic_Marks
    NDP_Plot_Documentation
  End If
  NDP_Plot

DoEvents

'If CurtainFileOpen Then frmCurtain.tmrCurtainAutoStep.Enabled = True

End Sub

Sub NDP_ChangeLimits()
Select Case Xcoord
Case 0
  T1 = XVc - XVs / 2
  T2 = T1 + XVs
Case 1
  PT1 = XVc - XVs / 2
  PT2 = PT1 + XVs
End Select

Zb = YVc - YVs / 2
Zt = Zb + YVs
SetDefaultScales
End Sub




Sub NDP_Plot_Curve(z!(), T!(), ad%, colour%)
' Plot a curve given by array pairs (z,T) and array dimension ad%
Dim i%, ii%
frmNDP.ForeColor = QBColor(colour%)
ii = 0
Do
ii = ii + 1
  If ii > ad Then Exit Sub
Loop Until (z(ii) >= YVl And T(ii) <= XVr)
' Go to first Point

frmNDP.PSet (T!(ii), z!(ii))
For i = ii + 1 To ad
  If T(i) < XVl Or z(i) < YVl Then Exit Sub
  If z(i) < YVr And T(i) > XVl Then frmNDP.Line -(T!(i), z!(i))      'And continue from there
Next i

End Sub





Private Sub Form_Click()
Static flag As Boolean

'  If CurtainFileOpen Then
'    If frmCurtain.tmrCurtainAutoStep.Enabled Then flag = True Else flag = False
'    frmCurtain.tmrCurtainAutoStep.Enabled = False
'  End If
'
'  frmNDP.NDPupdate
'
'  If CurtainFileOpen Then
'    If flag Then frmCurtain.tmrCurtainAutoStep.Enabled = True
'  End If

'RequestNDPupdate = True   ' mjmmjm
End Sub

Private Sub Form_DblClick()
  CaptureFormMJ Me, "", 3, True
End Sub


Private Sub Form_Load()
  
  NDPinitialize = True
  Show
'  NDP_Initialize
' NB Resize event is where initialization occurs
End Sub


Private Sub Form_Resize()

' Don't Resize when form is loaded
If Not NDPinitialize Then
  NDPupdate
Else
  NDP_Initialize
  NDPinitialize = False
End If

End Sub


Sub NDP_Plot_Grid_Marks()
Dim z%, T!
frmNDP.ForeColor = QBColor(Pgm%)
frmNDP.DrawWidth = 2
  Select Case zscale              'Deal with grid marks
  Case 1
'    For z% = YVl% + 1 To YVr% - 1 Step 2
'      For T = XVl% To XVr% Step Xstep: frmNDP.Line (T - 0.005, z%)-(T + 0.005, z%): Next T
'    Next z%

    For z% = YVl% + 2 To YVr% - 2 Step 2
      For T = XVl% + 5 To XVr% - 5 Step 5: frmNDP.Line (T - 0.1, z%)-(T + 0.1, z%): Next T
    Next z%

  Case Is > 1
    For z% = YVl% + 1 To YVr% - 1
      For T = XVl% + 2 To XVr% - 2 Step 2                    ' Dot field
        If T <= 250 Then frmNDP.Line (T - 0.05, z%)-(T + 0.05, z%) ' 2 K increments, ea Z
        If T > 250 Then frmNDP.Line (T - 0.1, z%)-(T + 0.1, z%)   ' empirical fix
        If T Mod 10 = 0 Then                               ' 10 K inc, ea Z
          frmNDP.Line (T, z% - 0.05)-(T, z% + 0.05)
          If T <= 250 Then frmNDP.Line (T - 0.05, z%)-(T + 0.05, z%)
          If T > 250 Then frmNDP.Line (T - 0.15, z%)-(T + 0.15, z%) ' empirical fix
        End If
      Next T
    Next z%

    For z% = YVl% + 0.5 To YVr% - 0.5 Step 1
      For T = XVl% + 10 To XVr% - 10 Step 10: frmNDP.Line (T, z - 0.02)-(T, z + 0.02): Next T
    Next z%
  End Select

End Sub


Sub NDP_Plot_Tic_Marks()
Dim z%, T%, tic!, zk!, Ykstep!, Pmb!, X!, tich!, ticv!, i%, zkm!

  frmNDP.ForeColor = QBColor(PTM%)

' Now deal with tic marks
  tich = frmNDP.ScaleWidth / 80
  ticv = 1.33 * frmNDP.ScaleHeight / 80

' Tic marks for left hand side (km)
  For z% = YVl% + 1 To YVr% - 1 Step 1
    If z% Mod 2 = 0 Then tic! = tich Else tic! = tich / 2
    frmNDP.Line (XVl%, z%)-(XVl% + tic!, z%)
  Next z%

' Tic marks for upper & lower temperature scale (K)
  For X = XVl To XVr Step 5#        'Upper/lower border tics
    If X Mod 10 = 0 Then tic! = -ticv
    frmNDP.Line (X, YVl%)-(X, YVl% + tic!)
    frmNDP.Line (X, YVr%)-(X, YVr% - tic)
  Next X

' Make ticks on right side (kft)
Select Case Yunitr
Case 0 'km
  For z% = YVl% + 1 To YVr% - 1 Step 1
    If z% Mod 2 = 0 Then tic! = tich Else tic! = tich / 2#
    frmNDP.Line (XVr% - tic!, z%)-(XVr%, z%)
  Next z%

Case 1 'kft
  Ykstep = Ystep * 2.5
  If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
  For z% = zk To kft_km * YVr% Step Ykstep
    If z% Mod 10 = 0 Then tic! = tich Else tic! = tich / 2
    frmNDP.Line (XVr% - tic, z% / kft_km)-(XVr%, z% / kft_km)
  Next z%
Case 2 'mb
  For z% = YVl% + 1 To YVr% - 1 Step 1
    If z% Mod 2 = 0 Then tic! = tic! = tich Else tic! = tich / 2
    frmNDP.Line (XVr% - tic!, z%)-(XVr%, z%)
  Next z%

Case 3 'hPa
  For i% = 10 To 1 Step -1
    Pmb = i% * 100
    zkm = fPtoZ(Pmb)
    If zkm > YVl And zkm < YVr Then
      tic! = tich
      frmNDP.Line (XVr% - tic!, zkm)-(XVr%, zkm)
    End If
  Next i%
  Pmb = 50 ' mb
  zkm = fPtoZ(Pmb)
  If zkm > YVl And zkm < YVr Then
    frmNDP.Line (XVr% - tic!, zkm)-(XVr%, zkm)
  End If
  Pmb = 25 ' mb
  zkm = fPtoZ(Pmb)
  If zkm > YVl And zkm < YVr Then
    frmNDP.Line (XVr% - tic!, zkm)-(XVr%, zkm)
  End If
End Select

End Sub


Sub NDP_Pointer_Pair(z!, C%, f%)
' z  ... Altitude of pointer
' C% ... Color of pointer
' F% ... Flag to indicate whether or not to fill with color
Dim z_inc!, T_inc!
  If z <> 99.9 Then
    frmNDP.DrawWidth = 1
    z_inc = 0.4 / zscale: T_inc = 2 / zscale
    frmNDP.ForeColor = QBColor(C%)
   
    frmNDP.Line (XVl%, z)-Step(T_inc, z_inc)     'Left pointer
    frmNDP.Line -Step(0, -2 * z_inc)
    frmNDP.Line -(XVl%, z)
'    If F% Then paint STEP(T_inc * 0.9, 0)
   
    frmNDP.Line (XVr%, z)-Step(-T_inc, z_inc)    'Right pointer
    frmNDP.Line -Step(0, -2 * z_inc)
    frmNDP.Line -(XVr%, z)
'    If F% Then paint STEP(-T_inc * 0.9, 0)
  End If
End Sub



Sub NDP_Plot()
Dim X!, y!, z!, Lmax%, LmaxI%, LmaxN%, L%, Tnat!, C%, Pn%, Pi%, Zi!, Tit!, Ti!, Zn!, TN!
Dim Tile$, DrawWidthSave%, i%, j%, jt1%, jt2%, NAs!(1 To 77), NDs!(1 To 77)
Dim iND%, dND!(1 To 77), NDac!, ALTac!

  jt1 = 1: jt2 = Nlev

  If ShowTropopause Then Call NDP_Pointer_Pair(Zt1, LRP%, -1)                'Plot trop#2 pointers
  If frmNDP.WindowState = vbMaximized Then frmNDP.FONTSIZE = frmNDP.FONTSIZE - 2


' Plot Actual Density Profile
  For i = 1 To Nlev
    ' Calc Std ND at each altitude
    NDs(i) = fStandardDensity(NDPmonth, RAp(i), Latitude)
    Select Case Xunit
    Case 0 'kg/m3
    Case 1 '#/cm3 E+18
      NDs(i) = 20.8 * NDs(i)
      RTp(i) = RTp(i) / 1E+24
    Case 2 '#/m3  E+24
      NDs(i) = 20.8 * NDs(i)
      RTp(i) = RTp(i) / 1E+18
    End Select
    dND(i) = -PCstart + 100# * (RTp(i) - NDs(i)) / NDs(i)
Next i
If Nlev1 > 16 Then
  NDac = NDs(Nlev1)
  ALTac = RAp(Nlev1)
Else
  NDac = NDs(16 - Nlev1 + 1)
  ALTac = RAp(16 - Nlev1 + 1)
End If
  frmNDP.DrawWidth = 1
'  Call NDP_Plot_Curve(RAp(), RTp(), Nlev, Yellow)
  
  Call NDP_Plot_Curve(RAp(), NDs(), Nlev, White)
  frmNDP.DrawWidth = 2
  
  Call NDP_Plot_Curve(RAp(), dND(), Nlev, LGray)
  
  frmNDP.DrawWidth = 6
  If PlotSingleScan Then
    frmNDP.DrawWidth = 4
    frmNDP.ForeColor = QBColor(Yellow)
    For j = jt1% To jt2%   'Plot data points
      X = RTp!(j%): y = RAp!(j%)
      If X > XVl And X < XVr And y < YVr And y > YVl Then PSet (X, y)
    Next j
  End If

' Draw pointers at A/C Altitude. Do last to put on top
  If ShowAircraft Then
    Call NDP_Pointer_Pair(ALTac, ACP%, -1)
    y = NDac
    frmNDP.Line (y - 2, ALTac)-(y + 2, ALTac), QBColor(ACS%)
  End If
End Sub



Sub NDP_Plot_Set()
Dim X!, y!, Fonts%

frmNDP.Cls
frmNDP.ScaleMode = 0
frmNDP.ForeColor = QBColor(PFG%)
frmNDP.BackColor = QBColor(Black)

'If ShowDocument Then  'Make extra room on bottom for documentation
'  Bl = TextWidth(" 000 "): Br = Bl
'  Bt = 2 * TextHeight("O"): Bb = 5 * TextHeight("1")
  Bl = XVs / 10: Br = Bl      'Bl was 6
  Bt = YVs / 8: Bb = YVs / 8 'Bt was 15, Bb was 2*
'Else
'  Bl = XVs / 10: Br = Bl      'Bl was 6
'  Bt = YVs / 30: Bb = 3 * Bt  'Bt was 15, Bb was 2*
'End If

frmNDP.Scale (XVl% - Bl, YVr% + Bt)-(XVr% + Br, YVl% - Bb)
frmNDP.DrawWidth = 2
frmNDP.Line (XVl%, YVr%)-(XVr%, YVl%), QBColor(PBG%), BF  'Blue background
frmNDP.Line (XVl%, YVr%)-(XVr%, YVl%), QBColor(PFG%), B   'Yellow border
frmNDP.DrawWidth = 1

' Set right (km) and left (kft) step sizes for labelling
If XVs > 15 Then Xstep! = 5 Else Xstep! = 3
If YVs > 15 Then Ystep! = 2 Else Ystep! = 1

'Determine new font size
frmNDP.FontName = "Small Fonts"
'If frmFormatNDP.chkFontSize.value = 0 Then
  Fonts% = 1
  Do
    frmNDP.FONTSIZE = Fonts%
    Fonts% = Fonts% + 1
    xoffset! = frmNDP.TextWidth("2")
    X = 1.75 * (5 * xoffset!)    'Separate 3 characters by half their width
  Loop Until X > Xstep!
'Else  ' Or user-specified font size
'  Fonts% = frmFormatNDP.txtFontSize
'  frmNDP.FontSize = Fonts%
'End If
frmNDP.FONTSIZE = Fonts% - 2
'frmFormatNDP.vscFontSize.value = Fonts%
yoffset! = frmNDP.TextHeight("1")
xoffset! = frmNDP.TextWidth("1")
If frmNDP.FONTSIZE > 7 Then frmNDP.FontName = "Courier New"  '"MS Sans Serif"
'frmFormatNDP.Hide
End Sub

Sub NDP_Plot_Axes_Labels()
Dim z%, zk, T%, temp$, Ykstep!, zkm!, Pmb!
Dim iND!, i%

  frmNDP.FontBold = True

' Label km axis
  Select Case Yunitl
  Case 0
    For z% = YVl% To YVr% Step Ystep!
      frmNDP.CurrentX = XVl% - 5 * xoffset!
      frmNDP.CurrentY = z% - yoffset! / 2
      If z% < 10 Then frmNDP.Print Format$(z%, " 0") Else frmNDP.Print Format$(z%, "##")
    Next z%
'Case 1
'Ykstep = 5 * Ystep
''If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
'For z% = zk To YVr% Step Ykstep '* kft_km! Step Ykstep!
'  frmNDP.CurrentX = XVr% + xoffset!
'  frmNDP.CurrentY = z% - yoffset! / 2 ' / kft_km! - yoffset! / 2
'  If z% < 10 Then frmNDP.Print Format$(z%, " 0") Else frmNDP.Print Format$(z%, "##")
'Next z%
  End Select

' Label kft on right side
  Select Case Yunitr
  Case 0  'km
    For z% = YVl% To YVr% Step Ystep!
      frmNDP.CurrentX = XVr% + xoffset! / 4
      frmNDP.CurrentY = z% - yoffset! / 2
      If z% < 10 Then frmNDP.Print Format$(z%, " 0") Else frmNDP.Print Format$(z%, "##")
    Next z%

  Case 1  'kft
    Ykstep = 5 * Ystep
    If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
    For z% = zk To YVr% * kft_km! Step Ykstep!
      frmNDP.CurrentX = XVr% + xoffset! / 4
      frmNDP.CurrentY = z% / kft_km! - yoffset! / 2
      If z% < 10 Then frmNDP.Print Format$(z%, " 0") Else frmNDP.Print Format$(z%, "##")
    Next z%

  Case 2  ' mb
    For z% = YVl% To YVr% Step Ystep!
      Pmb = fZtoP(z% * 1#)
      frmNDP.CurrentX = XVr% + xoffset! / 4
      frmNDP.CurrentY = z% - yoffset! / 2
      Select Case Pmb
      Case Is < 100: frmNDP.Print Format$(Pmb, " #0")
      Case Is < 1000: frmNDP.Print Format$(Pmb, "###")
      Case Else
        frmNDP.CurrentX = XVr% + xoffset! / 4
        frmNDP.Print Format$(Pmb, "###")
      End Select
    Next z%
  Case 3  ' hPa steps of 100
    For i% = 10 To 1 Step -1
      Pmb = i% * 100
      zkm = fPtoZ(Pmb)
      If zkm > YVl And zkm < YVr Then
        frmNDP.CurrentX = XVr% + xoffset! / 4
        frmNDP.CurrentY = zkm - yoffset! / 2
        Select Case Pmb
        Case Is < 100: frmNDP.Print Format$(Pmb, " #0")
        Case Is < 1000: frmNDP.Print Format$(Pmb, "###")
        Case Else
          frmNDP.CurrentX = XVr% + xoffset! / 4
          frmNDP.Print Format$(Pmb, "####")
        End Select
      End If
    Next i%
  ' Do a few special cases
    Pmb = 50 ' mb
    zkm = fPtoZ(Pmb)
    If zkm > YVl And zkm < YVr Then
      frmNDP.CurrentX = XVr% + xoffset! / 4
      frmNDP.CurrentY = zkm - yoffset! / 2
      frmNDP.Print Format$(Pmb, " #0")
    End If
    Pmb = 25 ' mb
    zkm = fPtoZ(Pmb)
    If zkm > YVl And zkm < YVr Then
      frmNDP.CurrentX = XVr% + xoffset! / 4
      frmNDP.CurrentY = zkm - yoffset! / 2
      frmNDP.Print Format$(Pmb, " #0")
    End If
  End Select

' Write Density-axis label in units of E+18 /cm^3
  For iND = XVl% To XVr% Step Xstep!
    temp$ = Format$(iND, "#0")
    frmNDP.CurrentX = iND - frmNDP.TextWidth(temp$) / 2
    frmNDP.CurrentY = YVl% + yoffset! / 2
    frmNDP.Print temp$
  Next iND

  temp$ = "Number Density [x E+24 /m3 or x E+18 /cm3]"
  frmNDP.CurrentX = (XVl + XVr) / 2 - frmNDP.TextWidth(temp$) / 2
  frmNDP.CurrentY = YVl% - 2 * Bl / 3
  frmNDP.Print temp$

  frmNDP.ForeColor = QBColor(LGray)
  For iND = XVl To XVr Step Xstep
    temp$ = Format$(iND + PCstart, "#0")
    frmNDP.CurrentX = iND - frmNDP.TextWidth(temp$) / 2
    frmNDP.CurrentY = YVr% - yoffset
    frmNDP.Print temp$
  Next iND

  temp$ = "Difference from Standard Atmosphere [%]"
  frmNDP.CurrentX = (XVl + XVr) / 2 - frmNDP.TextWidth(temp$) / 2
  frmNDP.CurrentY = YVr% + 1 * Bt
  frmNDP.Print temp$
  frmNDP.ForeColor = QBColor(Yellow)
  frmNDP.FONTSIZE = frmNDP.FONTSIZE + 1
  frmNDP.FontBold = True

  Select Case Yunitl
  Case 0
    temp$ = "km"
  Case 1
    temp$ = "kft"
  End Select
  frmNDP.CurrentX = XVl% - Bl   '/ 2 - frmNDP.TextWidth(temp$) / 2
  frmNDP.CurrentY = YVl% + yoffset / 2 '1.5 * frmNDP.TextHeight(temp$)
  frmNDP.Print temp$
  
  Select Case Yunitr
  Case 0: temp$ = "km"
  Case 1: temp$ = "kft"
  Case 2: temp$ = "mb"
  Case 3: temp$ = "hPa"
  End Select

  frmNDP.CurrentX = XVr% + Br - frmNDP.TextWidth(temp$)
  frmNDP.CurrentY = YVl% + yoffset / 2 '1.5 * frmNDP.TextHeight(temp$)
  frmNDP.Print temp$

  frmNDP.FontBold = False
  frmNDP.FONTSIZE = frmNDP.FONTSIZE - 1

End Sub


Sub NDP_Initialize()
Dim FormatCount$
' And assign to some attributes to facilitate changing
  LRL = White               'Lapse Rate lines
  LRP = LGray               'Lapse Rate pointers
  ACP = White               'Aircraft pointer
  ACS = White               'Aircraft symbol
  PBG = Blue                'Plot Background color
  PFG = Yellow              'Plot Foreground color
  FHL = Green               'Footer header line
  FDL = Green               'Footer data line
  PTM = Yellow              'Plot tic marks and border
  Pgm = Yellow              'Plot grid color
  PSL = Green               'Plot status line
  INL = LGray               'Input line
  
' The System Flags
  S_flag = True             'Show status bar in bottom LH corner      Ctrl+I
  T_flag = True             'Show trop info in upper RH corner        Ctrl+T
  GM_flag = False           'Show global minimum                      Ctrl+Z
  FastFwd = False           'TRUE if fast forwarding thru data (PgUp)
  FastBwd = False           'TRUE if fast backwarding thru data (PgDn)
  AutoStep = False          'TRUE if auto-stepping thru data (INS)
  P_flag = False            'Paint ICE and NAT regions on plot        Ctrl+P
  G_flag = False            'Flag to turn NDP ghost on and off        Ctrl+G
  L_flag = True             'Show -2K/km lapse rate lines             Ctrl+L
  DoNAT = False             'NAT flag
  NDPFileOpen = False       'True if NDP form opens file
  ShowDocument = True       'True if Documentation to be shown
  
  Dim X%, y%
' Initialize plot window center position, size and borders
  Tscale = 1
  zscale = 1                 'Default Plot scale (1, 2, or 3)           1,2,3
'  zc% = 12: zs% = 24: Tc% = 250: Ts% = 120
'  X% = Ts% / 2 ^ (Tscale - 1) / 2: XVl% = Tc% - X%: XVr% = Tc% + X%
'  Y% = zs% / 2 ^ (zscale - 1) / 2: YVl% = zc% - Y%: YVr% = zc% + Y%

  AutoStepInterval% = 100    'Auto Step cycle time in ms
  FastFBInterval% = 100      'FastFwd/FastBwd Step cycle time in ms
  
  'PVersion$ = "MPVIEWER.BAS 960707.0"
  'FooterTitle$ = "Rcrd utsec pALTkm Pitch  Roll   OAT  Zt1  Zt2 Thet1 Thet2 Latitud Longitud LRate"
  'FooterFormat$ = "#### ##### ##.### ###.# ###.# ###.# ##.# ##.# ###.# ###.# ###.### ####.### ###.#"
  FooterTitle$ = " Record  UTsec  pALTkm  Pitch   Roll    OAT   Zt1   Zt2  Theta1  Theta2  Latitude  Longitude  LRate"
  FooterFormat$ = "######  #####  ##.###  ###.#  ###.#  ###.#  ##.#  ##.#  ####.#  ####.#  ####.###  #####.###  ###.#"

 'Formatothis$ = "9999999 999.999  99.9  99.9 999.9 999.9 999.9 999.9 999.9 999.999  999.999 999.9"
  FormatCount$ = "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
  
  ztkmDef = 28: zbkmDef = 0         'NB Default values changed when file is opened
  ztkftDef = 90: zbkftDef = 0
  ztmbDef = 30: zbmbDef = 1000
  zthPaDef = 30: zthPaDef = 1000
  T1KDef = 0: T2KDef = 2             'kg/m3
  PT1KDef = 0: PT2KDef = 30          '#/cm3 * E+18
' Deal with C and F later

  ztkm = ztkmDef: zbkm = zbkmDef      'NB Default values changed when file is opened
  ztkft = ztkftDef: zbkft = zbkftDef
  ztmb = ztmbDef: zbmb = zbmbDef
  zthPa = zthPaDef: zbhPa = zbhPaDef
  T1K = T1KDef: T2K = T2KDef            'XV, YV, etc
  PT1K = PT1KDef: PT2K = PT2KDef        'Potential Temperature
' Default parameters
  Xcoord = 1                 '0=Mass Density, 1=Number Density
  Ycoordl = 0                '0=pALT, 1=gALT
  Ycoordr = 0                '0=pALT, 1=gALT
' Default units
  Xunit = 1                  '0=kg/m3, 1=#/cm3, 2=#/m3
  Yunitl = 0                 '0=z[km], 1=z[kft], 2=z[mb], 3=z[hPa]
  Yunitr = 1                 '0=z[km], 1=z[kft], 2=z[mb], 3=z[hPa]
  PCstart = -15              'Start of percentage scale
  SetDefaultScales
End Sub





