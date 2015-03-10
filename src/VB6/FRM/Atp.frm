VERSION 5.00
Begin VB.Form frmATP 
   AutoRedraw      =   -1  'True
   Caption         =   "JPL MTP - Altitude Temperature Profile"
   ClientHeight    =   4845
   ClientLeft      =   9345
   ClientTop       =   2235
   ClientWidth     =   6900
   Icon            =   "Atp.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   2  'Custom
   ScaleHeight     =   4845
   ScaleWidth      =   6900
   Begin VB.Timer Timer1 
      Left            =   165
      Top             =   195
   End
End
Attribute VB_Name = "frmATP"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Some Documentation
'
' ExtForm .................. used to align display wrt to calling form
'
' Properties that can be set remotely or using FormatRAOB Dialog Box
' ShowDots
' ShowSelectLevel
' EnableFit    Do Spline Fit interpolation of profile
Public UseTexAQS As Boolean                   'Use PBL display scales
Public EnableAveraging As Boolean             'Average overlapping scans in CTC plot
Public PlotSingleScan As Boolean              'Plot only one scan at a time
Public ShowAircraft As Boolean                'Show aircraft altitude
Public ShowTropopause As Boolean              'Show tropopause altitude
Public ShowAAprofile As Boolean               'Show AA temperature profile
Public TwoAAprofiles As Boolean               '
Public ATPFileOpen As Boolean                 '
Public ShowDocument As Boolean                'Put documentation at bottom of plot
Public ShowPressureAltitude As Boolean        'Plot in pressure altitude rather than geometric
Public EnableFit As Boolean                   'Do spline fit to ATP
Public ShowPSCthresholds As Boolean           'Show Type I and II PSC Temperature thresholds
Public DefaultBG As Boolean                   'Default Background color scheme
Public ATPinitialize As Boolean
Public LineThickness%                         'Selected pen thickness
Public LineColor%                             'Selected pen color

Public Bl!, Br!, Bt!, Bb!, xoffset!, yoffset!, Ystep!, Xstep!
Public XVl%, XVr%, XVs, XVc%, XVscale!, XVlast!, XVvalue!, XunitPerPixel!
Public YVl%, YVr%, YVs%, YVc%, YVscale!, YVlast!, FooterType%

Public T1%, T2%, PT1%, PT2%, T1K%, T2K%, PT1K%, PT2K%
Public T1def%, T2def%, PT1Def%, PT2Def%, T1KDef%, T2KDef%, PT1KDef%, PT2KDef%

Public Zt%, Zb%, ztkm%, zbkm%, ztkft%, zbkft%, ztmb%, zbmb%, zthPa%, zbhPa%
Public ztDef%, zbDef%, ztkmDef%, zbkmDef%, ztkftDef%, zbkftDef%, ztmbDef, zbmbDef%, zthPaDef, zbhPaDef%

Public zscale!, Tscale%

Public Xcoord%, Ycoordl%, Ycoordr%, Xunit%, Yunitl%, Yunitr%
Public PT_flag, BadNr%

Public LRL%, LRP%, ACP%, ACS%, PBG%, PFG%, PBC%, FHL%, FDL%, PTM%, Pgm%, PSL%, INL%
Public FastFwd As Boolean, FastBwd As Boolean
Public S_flag As Boolean, T_flag As Boolean, GM_flag As Boolean
Public LR_flag As Boolean, P_flag As Boolean, G_flag As Boolean
Public L_flag As Boolean, PlotDone As Boolean
Public AutoStep, AutoStepInterval%, FastFBInterval%
Public FooterTitle$, FooterFormat$
Public INPlu%, OUTlu%
Dim zcs!(1 To 99), Tcs!(1 To 99), zcss!(1 To 99), Tcss!(1 To 99), Zg!(1 To 33), Tg!(1 To 33), zgs!(1 To 99), Tgs!(1 To 99)

'frmATP variables
Public Taco!, jt1%, jt2%, jOut%
Public dZg!, Zt1!, Zt2!, ALTkm!, Tac!, TT1!, TT2!


Sub ATP_Plot_Documentation()
Dim V!(1 To 20), FooterTitle$, FooterFormat$, ALTft&, HHMMSS&, i%
      
  If pALT < 0 Then ALTft = 0 Else ALTft = Int(3280.8 * pALT)
  HHMMSS& = Val(fSecToTstring$(UTsec, False))
  Select Case FooterType
'  Case 0 'Default 2 Channel
'     FooterTitle$ = "HHMMSS ALTft Pitch  Roll  Tifa  Ttgt  Twin  OAT1  OAT2 Gain1 Gain2  .....      "
'    FooterFormat$ = "#00000 #0000 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #0.00 #0.00 #00000      "
'    V(1) = HHMMSS: V(2) = ALTft: V(3) = Pitch: V(4) = Roll
'    V(5) = Tifa: V(6) = Ttgt: V(7) = Twin: 'V(8) = OAT1: V(9) = OAT2
'    V(10) = g(1): V(11) = g(2): V(12) = 0
  Case 0     'Default 3 Channel
     FooterTitle$ = "hhmmss ALTft Latitud Longitud Hdg Pitch  Roll  MRI   OAT Wsp/dir  LRac"
    FooterFormat$ = "#00000 #0000 #00.000 #000.000 #00 #00.0 #00.0 #0.0 #00.0 #00 #00 #00.0"
    V(1) = HHMMSS: V(2) = ALTft: V(3) = Latitude: V(4) = Longitude
    V(5) = Heading: V(6) = Pitch: V(7) = Roll: V(8) = MRI
    V(9) = OATnav: V(10) = Wspd: V(11) = Wdir: V(12) = LRac
    
  Case 1 '2 Channel Gains
     FooterTitle$ = " utsec   G1eqn G2eqn    G1nd  G2nd   G1mms G2mms   G1use G2use Trend           "
    FooterFormat$ = "#00000  #00.00 #0.00  #00.00 #0.00  #00.00 #0.00  #00.00 #0.00 #00000          "
    V(1) = UTsec: V(2) = Geqn(1): V(3) = Geqn(2): V(4) = Gnd(1): V(5) = Gnd(2): V(6) = Goat(1): V(7) = Goat(2)
    V(8) = g(1): V(9) = g(2)
  
  Case 2 '3 Channel Gains
     FooterTitle$ = " G1eqn G2eqn G3eqn   G1nd  G2nd  G3nd   G1nav  G2nav  G3nav  G1use G2use G3use  "
    FooterFormat$ = " #0.00 #0.00 #0.00  #0.00 #0.00 #0.00   #0.00  #0.00 #00.00  #0.00 #0.00 #0.00  "
    V(1) = Geqn(1): V(2) = Geqn(2): V(3) = Geqn(3): V(4) = Gnd(1): V(5) = Gnd(2): V(6) = Gnd(3)
    V(7) = Gnav(1): V(8) = Gnav(2): V(9) = Gnav(3): V(10) = g(1): V(11) = g(2): V(12) = g(3)
  
  Case 3 'Some Mux Data, CB, dND
     FooterTitle$ = " Tmxr   Tnd  Tsyn  Tdc1  Tdc2  Tmtr  Ttgt   CB1   CB2   CB3  Dnd1  Dnd2  Dnd3   "
    FooterFormat$ = "#00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #0000 #0000 #0000 #0000 #0000 #0000   "
    Select Case AC$
    Case "NG"
    FooterFormat$ = "#00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #0000 #0000 #0000 #0000 #0000 #0000   "
     FooterTitle$ = " Tmxr   Tnd  Tsyn  Tpsp  Tsmp  Tmtr  Ttgt   CB1   CB2   CB3  Dnd1  Dnd2  Dnd3   "
      V(1) = Tmix: V(2) = Tnd: V(3) = Tsyn: V(4) = Tpsp: V(5) = Tsmp: V(6) = Tmtr
      V(7) = Ttg1: V(8) = CB(1): V(9) = CB(2): V(10) = CB(3): V(11) = dND(1): V(12) = dND(2): V(13) = dND(3)
    Case Else
      V(1) = Tmxr: V(2) = Tnd: V(3) = Tlo2: V(4) = Tdc1: V(5) = Tdc2: V(6) = Tmtr
      V(7) = Ttgt: V(8) = CB(1): V(9) = CB(2): V(10) = CB(3): V(11) = dND(1): V(12) = dND(2): V(13) = dND(3)
    End Select
    
  Case 4 'All Mux Data
    Select Case AC$
    Case "NG"
    FooterFormat$ = "#00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0"
     FooterTitle$ = " Tmxr   Tnd  Tsyn  Tpsp  Tsmp  Tmtr  Ttg1  Ttg2  Tdat  Tair  Vvid  Vp05  Vp15  Vm15  Vp08 "
      V(1) = Tmix: V(2) = Tnd: V(3) = Tsyn: V(4) = Tpsp: V(5) = Tsmp: V(6) = Tmtr: V(7) = Ttg1
      V(8) = Ttg2: V(9) = Tdat: V(10) = Tair: V(11) = Vvid: V(12) = Vp05: V(13) = Vp15: V(14) = Vm15: V(15) = Vp08
    Case Else
       FooterTitle$ = "  Tnd  Tamp  Tsyn  Tmix Ttgt1 Ttgt2  Acc+  Twin  Tmtr Spare  Vref Tdat1 Tdat2 Vcc/2 15V/n  Trad  Acc-"
      FooterFormat$ = "#00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0 #00.0"
      For i = 1 To 17: V(i) = Muxs(i): Next i
    End Select
    
  Case 5 'B,C Line Differences
    Call FindDifference(LineCurr$(2), LineLast(2), FooterTitle$, 2)
    Call FindDifference(LineCurr$(3), LineLast(3), FooterFormat$, 3)
    
  
  Case 6 'D,E Line Differences
    Call FindDifference(LineCurr$(4), LineLast(4), FooterTitle$, 4)
    Call FindDifference(LineCurr$(6), LineLast(6), FooterFormat$, 6)
  
  End Select
  
  If frmATP.ShowDocument Then
    frmATP.FontBold = False
    frmATP.ForeColor = QBColor(LGreen)
    frmATP.Font = "Courier"
    If frmATP.WindowState = vbMaximized Then frmATP.FONTSIZE = frmATP.FONTSIZE - 2
    If frmATP.FooterType > 4 Then frmATP.FONTSIZE = frmATP.FONTSIZE - 1
    If frmATP.FooterType = 6 Then frmATP.FONTSIZE = frmATP.FONTSIZE - 1
    
    frmATP.CurrentX = XVl - Bl
    frmATP.CurrentY = YVl - Bb / 2
    frmATP.Print FooterTitle$
  
    frmATP.ForeColor = QBColor(White)
    frmATP.CurrentX = XVl - Bl
    frmATP.CurrentY = YVl - 3 * Bb / 4
    If FooterType < 6 Then frmATP.Print fUsing$(FooterFormat$, V()) Else frmATP.Print FooterFormat$
  End If
' Mapping of Mux data for NGV relative to other aircraft
'   "NG"                "All others"
' Following are M01 parameters
' Same name = *
' *  Ttgt = MV(0, r)    Ttgt   = Ttgtlo or Ttgthi
'    Vm08 = MV(1, r)    Tnd
'    Vvid = MV(2, r)    Tlo1  amp
'    Vp08 = MV(3, r)    Tlo2  syn
'    Vmtr = MV(4, r)    Tifa = Tmxr
'    Vsyn = MV(5, r)    Ttgtlo
'    Vp15 = MV(6, r)    Ttgthi
'    Vp05 = MV(7, r)    ACCp
'    Vm15 = MV(8, r)    Twin
' Following are M02 parameters
'    ACCp = MV(9, r)    Tmtr
'    Tdat = MV(10, r)   asterisk
' *  Tmtr = MV(11, r)   Vref
'    Tair = MV(12, r)   Tdc1     'dc1 = 11 on DC8
'    Tsmp = MV(13, r)   Tdc2     'dc2 = 12 on DC8
'    Tpsp = MV(14, r)   Vps5
'    Tnc  = MV(15, r)   Vps12    nc=no connection (humidity?)
'    Tsyn = MV(16, r)   ACCm
' Following are Pt parameters
'    R350 = MV(17, r)
'    Ttg1 = MV(18, r)
'    Ttg2 = MV(19, r)
' *  Twin = MV(20, r)
'    Tmix = MV(21, r)
'    Tamp = MV(22, r)
' *  Tnd  = MV(23, r)
'    R600 = MV(24, r)
' In addition:
'    Tmix = Tifa
'    Ttg1 = Ttgtlo
'    Ttg2 = Ttgthi
'    Vp05 = Vps5
'    Tamp = Tlo1
'    Vsyn = Tlo2


End Sub


Sub ATP_Plot_Static()
    
  SetDefaultScales
  ATP_Plot_Set
  ATP_Plot_Axes_Labels
  ATP_Plot_Grid_Marks
  ATP_Plot_Tic_Marks
  ATP_Plot_Documentation

End Sub

Sub DoSplineFits(EnableFit As Boolean)
Dim i%, j%, k%, dZ!, Wg!, jt1%, jt2%

If Nlev > 0 Then
  ' Do the SPLINE fit to retrieved ATP----------------------------<<<<<
  jt1% = 1: jt2% = Nlev: jOut% = 1
  Call SPLINE(RAp!(), RTp!(), TD1(), TD2(), jt1%, jt2%)

  zcs!(1) = RAp!(1)                    ' Then, set levels for Spline Fit
  Tcs(1) = RTp(1)
  For k% = jt1% + 1 To jt2%
    If EnableFit Then
      dZ! = RAp!(k%) - RAp!(k% - 1)
      If dZ! < 2 Then
        jOut = jOut + 1: zcs!(jOut%) = RAp!(k% - 1) + 0.5 * dZ!
        jOut = jOut + 1: zcs!(jOut%) = RAp!(k%)
      Else
        jOut = jOut + 1: zcs!(jOut%) = RAp!(k% - 1) + 0.25 * dZ!
        jOut = jOut + 1: zcs!(jOut%) = RAp!(k% - 1) + 0.5 * dZ!
        jOut = jOut + 1: zcs(jOut%) = RAp!(k% - 1) + 0.75 * dZ!
        jOut = jOut + 1: zcs(jOut%) = RAp!(k%)
      End If
    Else
      jOut = k
      zcs(jOut%) = RAp!(k%)
      Tcs(jOut) = RTp(k)
    End If
  Next

  If EnableFit Then
  ' Calculate the interpolated values for the spline fit display
    For j% = 1 To jOut%
      Tcs(j%) = SPLINT(RAp!(), RTp!(), TD2(), jt1%, jt2%, zcs(j%), 1)
      'Debug.Print j; zcs(j); Tcs(j)
      'If PT_flag Then Tcs!(j%) = fTheta(Tcs!(j%), fZtoP(zcs!(j%)))
    Next
  End If
  If Zt1 < 99# Then TT1 = SPLINT(RAp!(), RTp!(), TD2(), jt1%, jt2%, Zt1, 1)
  If Zt2 < 99# Then TT2 = SPLINT(RAp!(), RTp!(), TD2(), jt1%, jt2%, Zt2, 1)

  ' Update ghost profile
    If Tg!(17) = 0 Then
      If RTp!(17) > 170 And RTp!(17) < 310 Then  'chk quality of OAT
        For i% = 1 To 33: Zg(i%) = RAp!(i%): Tg(i%) = RTp!(i%): Next i%
      End If
    Else
      Wg! = 0.06
      For j% = 1 To 33
        Zg(j%) = (Zg(j%) + Wg! * RAp!(j%)) / (1 + Wg!)
        Tg(j%) = (Tg(j%) + Wg! * RTp!(j%)) / (1 + Wg!)
      Next j%
    End If

End If

End Sub

Sub Calc_Intercepts(z!(), T!(), Zt%, T1%, L%, Lmax%)
Dim M!, Tx!, Zx!

  If L% >= Lmax% Or L% = 0 Then Exit Sub

' Linearly extrapolate last two points to T and Z axes
  M = (z(L%) - z(L% - 1)) / (T(L%) - T(L% - 1))
  Tx = (Zt% - z(L%)) / M + T(L%)
  Zx = (T1% - T(L%)) * M + z(L%)
' And adjust arrays to reflect appropriate intercept
  L% = L% + 1
  If Tx > XVl% Then
    z(L%) = Zt%
    T(L%) = Tx
  Else
    z(L%) = Zx
    T(L%) = XVl%
  End If

End Sub

Sub SetDefaultScales()

  Select Case Xcoord
  Case 0
    XVl = T1K: XVr = T2K
  Case 1
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



Sub ATPupdate()
Dim colour%

  DoSplineFits (EnableFit)   'Spline Fit Data profile and Ghost profile
  If PlotSingleScan Then ATP_Plot_Static
  ATP_Plot
  
  If ShowAAprofile Then
    colour = White
    Call ATP_Plot_Curve(RAp(), RTp2(), Nlev, colour%)
    If TwoAAprofiles Then
      Call ATP_Plot_Curve(RAp(), RTp3(), Nlev, colour%)
    End If
    DoEvents
  End If
  
End Sub

Sub ATP_ChangeLimits()
  
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




Sub ATP_Plot_Curve(z!(), T!(), ad%, colour%)
' Plot a curve given by array pairs (z,T) and array dimension ad%
' Modified 2004.07.14 to properly extrapolate to the edges of the window using an
' algorithm based on boundaries determined by the edges and slopes from a given
' point to each of the corners.
Dim i%, LastPointOutsideWindow As Boolean, M!, m1!, m2!, m3!, m4!

frmATP.ForeColor = QBColor(colour%)

' Initialize flag to indicate whether last point plotted was outside window
If (T(1) > XVl And T(1) < XVr) And (z(1) > YVl And z(1) < YVr) Then
  LastPointOutsideWindow = False
Else
  LastPointOutsideWindow = True
End If

For i = 1 To ad
  If (T(i) > XVl And T(i) < XVr) And (z(i) > YVl And z(i) < YVr) Then
  ' Point is inside window
    If LastPointOutsideWindow Then
      M = (T(i) - T(i - 1)) / (z(i) - z(i - 1)) 'Slope dT/dz
      m1 = (XVr - T(i)) / (YVr - z(i))
      m2 = (XVr - T(i)) / (YVl - z(i))
      m3 = (XVl - T(i)) / (YVl - z(i))
      m4 = (XVl - T(i)) / (YVr - z(i))
      ' Top of window
      If z(i - 1) > YVr And (M > m4 And M <= m1) Then
        frmATP.PSet (T(i) + M * (YVr - z(i)), YVr)
      End If
      ' Right of window
      If T(i - 1) > XVr And (M > m1 Or M <= m2) Then
        frmATP.PSet (XVr, z(i) + (XVr - T(i)) / M)
      End If
      ' Bottom of window
      If z(i - 1) < YVl And (M > m2 And M <= m3) Then
        frmATP.PSet (T(i) + M * (YVl - z(i)), YVl)
      End If
      ' Left of window
      If T(i - 1) < XVl And (M > m3 Or M <= m4) Then
        frmATP.PSet (XVl, z(i) + (XVl - T(i)) / M)
      End If
      frmATP.Line -(T!(i), z!(i))      'And continue from there
      LastPointOutsideWindow = False
    Else
      If i = 1 Then
        frmATP.PSet (T(i), z(i))
      Else
        frmATP.Line -(T!(i), z!(i))
        'Debug.Print z(i); T(i)
      End If
    End If
  
  Else  ' Extend from last point inside window to edge of window
    If i > 1 And Not LastPointOutsideWindow Then
      M = (T(i) - T(i - 1)) / (z(i) - z(i - 1)) 'Slope dT/dz
      m1 = (XVr - T(i - 1)) / (YVr - z(i - 1))
      m2 = (XVr - T(i - 1)) / (YVl - z(i - 1))
      m3 = (XVl - T(i - 1)) / (YVl - z(i - 1))
      m4 = (XVl - T(i - 1)) / (YVr - z(i - 1))
      ' Top of window
      If z(i) > YVr And (M > m4 And M <= m1) Then
        frmATP.Line -(T(i - 1) + M * (YVr - z(i - 1)), YVr)
      End If
      ' Right of window
      If T(i) > XVr And (M > m1 Or M <= m2) Then
        frmATP.Line -(XVr, z(i - 1) + (XVr - T(i - 1)) / M)
      End If
      ' Bottom of window
      If z(i) < YVl And (M > m2 And M <= m3) Then
        frmATP.Line -(T(i - 1) + M * (YVl - z(i - 1)), YVl)
      End If
      ' Left of window
      If T(i) < XVl And (M > m3 Or M <= m4) Then
        frmATP.Line -(XVl, z(i - 1) + (XVl - T(i - 1)) / M)
      End If
      LastPointOutsideWindow = True
    End If
  End If
  
Next i

End Sub





Private Sub Form_Click()
Static flag As Boolean

'  If CurtainFileOpen Then
'    If frmCurtain.tmrCurtainAutoStep.Enabled Then flag = True Else flag = False
'    frmCurtain.tmrCurtainAutoStep.Enabled = False
'  End If
'
'  frmATP.ATPupdate
'
'  If CurtainFileOpen Then
'    If flag Then frmCurtain.tmrCurtainAutoStep.Enabled = True
'  End If


RequestATPupdate = True
End Sub

Private Sub Form_DblClick()
Dim Filename$
  
  Filename$ = "ATPimage_" + frmFLTINFO.cboFltDates.Text + ".PNG"
  CaptureFormMJ Me, MNpath$ + "PNG\" + Filename$, 3, True
  
End Sub


Private Sub Form_Load()
  
  ATPinitialize = True
  Show
  Picture = LoadPicture("c:\MTP\VB6\DIB\rainbow.dib")  ' Load bitmap.
'  Timer1.Interval = 1000
  Show
  With frmATP
  .ScaleMode = 1                           'Twips
  .Top = CallingForm.Height
  .Left = 0
'  .Width = 7080      'Twips
'  .Height = 5010     'Twips
  End With

End Sub


Private Sub Form_Resize()

' Don't Resize when form is loaded
If Not ATPinitialize Then
  ATPupdate
Else
  ATP_Initialize
  ATPinitialize = False
End If

End Sub


Sub ATP_Plot_Grid_Marks()
Dim z%, T%
  
  frmATP.ForeColor = QBColor(Pgm%)
  frmATP.DrawWidth = 2
  Select Case zscale              'Deal with grid marks
  Case 1
    For z% = YVl% + 1 To YVr% - 1 Step 2
      For T% = XVl% + 10 To XVr% - 10 Step 10: frmATP.Line (T% - 0.06, z%)-(T% + 0.06, z%): Next T%
    Next z%

    For z% = YVl% + 2 To YVr% - 2 Step 2
      For T% = XVl% + 5 To XVr% - 5 Step 5: frmATP.Line (T% - 0.1, z%)-(T% + 0.1, z%): Next T%
    Next z%

  Case Is > 1
    For z% = YVl% + 1 To YVr% - 1
      For T% = XVl% + 2 To XVr% - 2 Step 2                    ' Dot field
        If T% <= 250 Then frmATP.Line (T% - 0.05, z%)-(T% + 0.05, z%) ' 2 K increments, ea Z
        If T% > 250 Then frmATP.Line (T% - 0.1, z%)-(T% + 0.1, z%)   ' empirical fix
        If T% Mod 10 = 0 Then                               ' 10 K inc, ea Z
          frmATP.Line (T%, z% - 0.05)-(T%, z% + 0.05)
          If T% <= 250 Then frmATP.Line (T% - 0.05, z%)-(T% + 0.05, z%)
          If T% > 250 Then frmATP.Line (T% - 0.15, z%)-(T% + 0.15, z%) ' empirical fix
        End If
      Next T%
    Next z%

    For z% = YVl% + 0.5 To YVr% - 0.5 Step 1
      For T% = XVl% + 10 To XVr% - 10 Step 10: frmATP.Line (T%, z - 0.02)-(T%, z + 0.02): Next T%
    Next z%
  End Select

End Sub


Sub ATP_Plot_Tic_Marks()
Dim z%, T%, tic!, zk!, Ykstep!, Pmb!, i%, zkm!
  
  frmATP.ForeColor = QBColor(PTM%)
' Now deal with tic marks
' Tic marks for left hand side (km)
  For z% = YVl% + 1 To YVr% - 1 Step 1
    If z% Mod 2 = 0 Then tic! = 1.5 / zscale Else tic! = 0.75 / zscale
    frmATP.Line (XVl%, z%)-(XVl% + tic!, z%)
  Next z%

' Tic marks for upper & lower temperature scale (K)
  For T% = XVl% + 5 To XVr% - 5 Step 5       'Upper/lower border tics
    If T% Mod 10 = 0 Then tic! = 0.5 / zscale Else tic! = 0.25 / zscale
    If UseTexAQS Then tic = tic / 4
    frmATP.Line (T%, YVl%)-(T%, YVl% + tic!): frmATP.Line (T%, YVr%)-(T%, YVr% - tic)
  Next T%

' Make ticks on right side (kft)
  Select Case Yunitr
  Case 0 'km
    For z% = YVl% + 1 To YVr% - 1 Step 1
      If z% Mod 2 = 0 Then tic! = 1.5 / zscale Else tic! = 0.75 / zscale
      frmATP.Line (XVr% - tic!, z%)-(XVr%, z%)
    Next z%

  Case 1 'kft
    Ykstep = Ystep * 2.5
    If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
    For z% = zk To kft_km * YVr% Step Ykstep
      If z% Mod 10 = 0 Then tic! = 1.5 / zscale Else tic! = 0.75 / zscale
      frmATP.Line (XVr% - tic, z% / kft_km)-(XVr%, z% / kft_km)
    Next z%

  Case 2 'mb
    For z% = YVl% + 1 To YVr% - 1 Step 1
      If z% Mod 2 = 0 Then tic! = 1.5 / zscale Else tic! = 0.75 / zscale
      frmATP.Line (XVr% - tic!, z%)-(XVr%, z%)
    Next z%

  Case 3 'hPa
    For i% = 10 To 1 Step -1
      Pmb = i% * 100
      zkm = fPtoZ(Pmb)
      If zkm > YVl And zkm < YVr Then
        tic! = 1.5 / zscale
        frmATP.Line (XVr% - tic!, zkm)-(XVr%, zkm)
      End If
    Next i%
    Pmb = 50 ' mb
    zkm = fPtoZ(Pmb)
    If zkm > YVl And zkm < YVr Then
      frmATP.Line (XVr% - tic!, zkm)-(XVr%, zkm)
    End If
    Pmb = 25 ' mb
    zkm = fPtoZ(Pmb)
    If zkm > YVl And zkm < YVr Then
      frmATP.Line (XVr% - tic!, zkm)-(XVr%, zkm)
    End If
  End Select

End Sub


Sub ATP_Pointer_Pair(z!, C%, f%)
' z  ... Altitude of pointer
' C% ... Color of pointer
' F% ... Flag to indicate whether or not to fill with color
Dim z_inc!, T_inc!
  
  If z <> 99.9 Then
    z_inc = 0.4 / zscale: T_inc = 2 / zscale
    If UseTexAQS Then z_inc = z_inc / 5: T_inc = T_inc / 2
    frmATP.ForeColor = QBColor(C%)
   
    frmATP.Line (XVl%, z)-Step(T_inc, z_inc)     'Left pointer
    frmATP.Line -Step(0, -2 * z_inc)
    frmATP.Line -(XVl%, z)
'    If F% Then paint STEP(T_inc * 0.9, 0)
   
    frmATP.Line (XVr%, z)-Step(-T_inc, z_inc)    'Right pointer
    frmATP.Line -Step(0, -2 * z_inc)
    frmATP.Line -(XVr%, z)
'    If F% Then paint STEP(-T_inc * 0.9, 0)
  End If

End Sub



Sub ATP_Plot()
Dim X!, y!, z!, Lmax%, LmaxI%, LmaxN%, L%, Tnat!, C%, Pn%, Pi%, Zi!, Tit!, Ti!, Zn!, TN!
Dim Tile$, DrawWidthSave%, j%, jt1%, jt2%, dZgSave!
Dim zIN!(1 To 30)         'Zice
Dim TIs!(1 To 30)         'Tice
Dim TNs!(1 To 30)         'Tnat

'jt1 = 1: jt2 = Nlev
jt1 = 33 - Nlev + 1: jt2 = 33
If Zt1 < 99.9 And ShowPSCthresholds Then
' Calculate Saturation Curves
  z = Zt1 - 0.5
  LmaxI% = 0: LmaxN% = 0: Lmax% = 30
  For L = 1 To Lmax%                    'Start at tropopause altitude
    z = z + 0.5                         'and step 0.5 km to 15 km above trop
    'Debug.Print z; TNs(L); TIs(L)
    zIN(L) = z                          'Save altitude
    TIs(L) = fTice(z)                   'Tice
    If z < YVr% And TIs(L) > XVl% Then LmaxI% = L
    Tnat = TIs(L) + 6.8                 '8 ppbv, 15 km; reduce to 5.8 K
                                        'for 4 ppbv
    Tnat = Tnat + 0.06 * (z - 15)       'small altitude correction
    If z <= (Zt1 + 5) Then Tnat = Tnat - 7 * (((Zt1 + 5) - z) / 5) ^ 2
    TNs(L) = Tnat                       'Tnat
'   Debug.Print Z; TNs(L); TIs(L)
    If Tnat > XVl% And z < YVr% Then LmaxN% = L
  Next L
' Calculate end points on z or T axes
  Call Calc_Intercepts(zIN!(), TIs!(), YVr%, XVl%, LmaxI%, Lmax%)
  Call Calc_Intercepts(zIN!(), TNs!(), YVr%, XVl%, LmaxN%, Lmax%)
 
  'temp% = XVr%: If XVl% < 200 Then temp% = 200
  'Line (XVl%, 33)-(temp%, 33), QBColor(LBlue)        'top-off NAT zone (for Paint)
  C% = LBlue                          'Plot Ice and NAT curves
  Call ATP_Plot_Curve(zIN!(), TIs!(), LmaxI%, C%)
  If Not P_flag Then C% = LGreen
  Call ATP_Plot_Curve(zIN!(), TNs!(), LmaxN%, C%)
  If TIs(1) > XVl Then frmATP.Line (XVl%, Zt1)-(TIs(1), Zt1), QBColor(C%)

  If P_flag Then
  ' Calc T/Z locations for painting ICE & NAT region
    Pn% = 1: Pi% = 1
    Zi = Zt1 + 0.1: Tit = 193 - 0.93 * (Zt1 - 15):
    Ti = Tit - 0.4 - 0.2: Zn = Zi + 0.9: TN = Ti
    If YVr% - Zn > 1.5 Then Zn = Zn + 1.4

    If Zn > YVr% Or Zn < YVl% Or Tit < XVl% Or Tit > XVr% Then Pn% = 0
    If Zi > YVr% Or Zi < YVl% Or Ti < XVl% Or Ti > XVr% Then Pi% = 0
    If Pn% = 1 Then
      Tile$ = Chr$(&H0) + Chr$(&H0) + Chr$(&H0) + Chr$(&HFF)
 '     PAINT (Tit, Zn), Tile$, 9, CHR$(&H6)         'Paint NAT region
    End If
 '   IF pi% = 1 THEN PAINT (TI, Zi), 9, 9           'Paint ICE region
    frmATP.Line (XVl%, Zt1)-(zIN(1), Zt1), QBColor(Blue%)
  End If
  
' Plot LR=-2 [K/km] line if Trops exist, also draw Trop pointers
  If ShowTropopause Then Call ATP_Pointer_Pair(Zt1, LRP%, -1)
'  Line (XVl%, zt1)-(TN(1), zt1), QBColor(LRP%)
End If

  If ShowTropopause Then Call ATP_Pointer_Pair(Zt2, LRP%, -1)                'Plot trop#2 pointers

' Plot Caspar if he is visible
  ATP_Draw_Ghost_Line
  frmATP.ForeColor = QBColor(White%)
  DrawWidthSave = frmATP.DrawWidth
'  frmATP.DrawWidth = 6
  If PlotSingleScan Then
    frmATP.DrawWidth = 5
    For j% = 1 To Nlev   'Plot data points
      X = RTp!(j%): y = RAp!(j%)
      If PT_flag Then X = fTheta(X, fZtoP(y))
      If X > XVl And X < XVr And y < YVr And y > YVl Then frmATP.PSet (X, y)
    Next
    frmATP.DrawWidth = DrawWidthSave
'   Plot lines between data points
    frmATP.DrawWidth = 2
    Call ATP_Plot_Curve(zcs!(), Tcs!(), jOut%, PFG)
  Else
    frmATP.DrawWidth = 1
    Call ATP_Plot_Curve(zcs!(), Tcs!(), jOut%, PFG)
  End If
  frmATP.DrawWidth = 2 'DrawWidthSave

' Draw pointers at A/C Altitude. Do last to put on top
  dZgSave = dZg
  
  If ShowPressureAltitude Then dZg = 0#
  If ALTkm <> 99.9 And ShowAircraft Then
    Call ATP_Pointer_Pair(ALTkm + dZg, ACP%, -1)
    If PT_flag Then y = fTheta(Tac, fZtoP(ALTkm + dZg)) Else y = Tac
      frmATP.Line (y - 5, ALTkm + dZg)-(y + 5, ALTkm + dZg), QBColor(ACS%)
  End If
  dZg = dZgSave
 
  If ShowTropopause Then ATP_Draw_LR_Lines          'Uses old values to draw lines
 
End Sub



Sub ATP_Draw_Ghost_Line()
Dim Tg1#(1 To 33), Tg2#(1 To 33), jg1%, jg2%, j%

If Not G_flag Then Exit Sub
'Plot new ghost
' Figure out number of non-zero ghost profile levels
  jg1% = 0: Do: jg1 = jg1 + 1: Loop Until Zg(jg1%) > 0 Or jg1% = 33
  jg2% = 34: Do: jg2 = jg2 - 1: Loop Until Zg(jg2%) > 0 Or jg2% = 1
  jg1 = jg1 + 1: jg2 = jg2 - 1
  If jg1% < 33 Then   'No retrieved ghost levels
'   Do the SPLINE fit to ghost profile
    Call SPLINE(Zg(), Tg(), Tg1(), Tg2(), jg1%, jg2%)
'   And calculate the interpolated values for the spline fit display
    For j% = 1 To jOut%
      Tgs(j%) = SPLINT(Zg(), Tg(), Tg2(), jg1%, jg2%, zcs(j%), 1)
    Next
    Call ATP_Plot_Curve(zcs(), Tgs(), jOut%, Cyan%)
  End If
End Sub

Sub ATP_Draw_LR_Lines()
Dim T_inc!
  T_inc = 3 / zscale
  If Not L_flag Then Exit Sub
  frmATP.ForeColor = QBColor(LRL%)
  frmATP.DrawStyle = 2

  If Zt1 < 99.9 Then
    frmATP.DrawWidth = 1

    If PT_flag Then
      frmATP.Line (XVr% - T_inc, Zt1)-(XVl% + T_inc, Zt1)
    Else
      If TT1 > XVl Then
        frmATP.Line (XVr% - T_inc, Zt1)-(TT1, Zt1)
        frmATP.Line (TT1, Zt1)-(TT1 - 2 * (YVl - Zt1), YVl)
      End If
      If TT1 > XVl And TT1 < XVr Then
        If Zt1 > YVl And Zt1 < YVr Then
        frmATP.DrawWidth = 6
        frmATP.PSet (TT1, Zt1)
        End If
      End If
    End If
  End If
 
  If Zt2 < 99.9 Then
    frmATP.DrawWidth = 1

    If PT_flag Then
      frmATP.Line (XVr% - T_inc, Zt2)-(XVl% + T_inc, Zt2)
    Else
      frmATP.Line (XVr% - T_inc, Zt2)-(TT2, Zt2)
      frmATP.Line (TT2, Zt2)-(TT2 + 2 * Zt2, 0)
      frmATP.DrawWidth = 6
      frmATP.PSet (TT1, Zt1)
     End If
  End If
  frmATP.DrawWidth = 1
  frmATP.DrawStyle = 0
  
End Sub



Sub ATP_Plot_Set()
Dim X!, y!, Fonts%

  With frmATP
  .Cls
  .ScaleMode = 1
  .ForeColor = QBColor(PFG%)
  .BackColor = QBColor(PBC%)
  End With

  If ShowDocument Then  'Make extra room on bottom for documentation
'  Bl = TextWidth(" 000 "): Br = Bl
'  Bt = 2 * TextHeight("O"): Bb = 5 * TextHeight("1")
    Bl = XVs / 10: Br = Bl      'Bl was 6
    Bt = YVs / 30: Bb = YVs / 5 'Bt was 15, Bb was 2*
  Else
    Bl = XVs / 10: Br = Bl      'Bl was 6
    Bt = YVs / 30: Bb = 3 * Bt  'Bt was 15, Bb was 2*
  End If

  If BorderStyle = 0 Then
    Bl = XVs / 7: Br = Bl      'Bl was 6
    Bt = YVs / 20: Bb = 3 * Bt  'Bt was 15, Bb was 2*
  End If
  
  frmATP.Scale (XVl% - Bl, YVr% + Bt)-(XVr% + Br, YVl% - Bb)
  frmATP.DrawWidth = 2
  frmATP.Line (XVl%, YVr%)-(XVr%, YVl%), QBColor(PBG%), BF  'Blue background
  frmATP.Line (XVl%, YVr%)-(XVr%, YVl%), QBColor(PFG%), B   'Yellow border
  frmATP.DrawWidth = 1

' Set right (km) and left (kft) step sizes for labelling
  If XVs > 80 Then Xstep! = 10 Else Xstep! = 5
  If YVs > 15 Then Ystep! = 2 Else Ystep! = 1
' Determine new font size
  frmATP.FontName = "Small Fonts"
  If frmFormatATP.chkFontSize.Value = 0 Then
    Fonts% = 1
    Do
      frmATP.FONTSIZE = Fonts%
      Fonts% = Fonts% + 1
      xoffset! = frmATP.TextWidth("2")
      X = 1.75 * (3 * xoffset!)    'Separate 3 characters by half their width
    Loop Until X > Xstep!
  Else  ' Or user-specified font size
    Fonts% = frmFormatATP.txtFontSize
    frmATP.FONTSIZE = Fonts%
  End If

  frmFormatATP.vscFontSize.Value = Fonts%
  yoffset! = frmATP.TextHeight("1")
  xoffset! = frmATP.TextWidth("1")
  frmATP.Font = "MS Sans Serif"  '"Courier New"  '

  If BorderStyle = 0 Then
    Xstep = 20
    Ystep = 5
    FONTSIZE = 6
    yoffset! = -1
    xoffset! = 2.3
  End If

End Sub

Sub ATP_Plot_Axes_Labels()
Dim z%, zk, T%, temp$, Ykstep!, zkm!, Pmb!, i%

If BorderStyle = 0 Then frmATP.FontBold = False Else frmATP.FontBold = True

' Label km axis
Select Case Yunitl
Case 0
  For z% = YVl% To YVr% Step Ystep!
    frmATP.CurrentX = XVl% - 4.5 * xoffset!
    frmATP.CurrentY = z% - yoffset! / 2
    If z% < 10 Then
      If BorderStyle = 0 And z > 4 Then
        frmATP.Print Format$(z%, " 0")
      Else
        If BorderStyle > 0 Then frmATP.Print Format$(z%, " 0")
      End If
    Else
        frmATP.Print Format$(z%, "##")
    End If
  Next z%
'Case 1
'Ykstep = 5 * Ystep
''If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
'For z% = zk To YVr% Step Ykstep '* kft_km! Step Ykstep!
'  frmATP.CurrentX = XVr% + xoffset!
'  frmATP.CurrentY = z% - yoffset! / 2 ' / kft_km! - yoffset! / 2
'  If z% < 10 Then frmATP.Print Format$(z%, " 0") Else frmATP.Print Format$(z%, "##")
'Next z%
End Select

' Label kft on right side
Select Case Yunitr
Case 0  'km
For z% = YVl% To YVr% Step Ystep!
  frmATP.CurrentX = XVr% + xoffset! / 4
  frmATP.CurrentY = z% - yoffset! / 2
  If z% < 10 Then
    If BorderStyle = 0 And z > 4 Then
      frmATP.Print Format$(z%, " 0")
    Else
      If BorderStyle > 0 Then frmATP.Print Format$(z%, " 0")
    End If
  End If
Next z%

Case 1  'kft
'Ykstep = 5 * Ystep
If YVr < 6 Then Ykstep = 2 * Ystep Else Ykstep = 5 * Ystep
If BorderStyle = 0 Then Ykstep = 20
If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
For z% = zk To YVr% * kft_km! Step Ykstep!
  frmATP.CurrentX = XVr% + xoffset! / 4
  frmATP.CurrentY = z% / kft_km! - yoffset! / 2
  If z% < 10 Then
    If BorderStyle = 0 And z > 4 Then
      frmATP.Print Format$(z%, " 0")
    Else
      If BorderStyle > 0 Then frmATP.Print Format$(z%, " 0")
    End If
  Else
    If z >= 100 Then
      frmATP.Print Format$(z%, "000")
    Else
      frmATP.Print Format$(z%, "00")
    End If
  End If
Next z%

'If YVr < 6 Then Ykstep = 2 * Ystep Else Ykstep = 5 * Ystep
'If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
'For z% = zk To YVr% * kft_km! Step Ykstep!
'  frmRAOB.CurrentX = XVr% + xoffset!
'  frmRAOB.CurrentY = z% / kft_km! - yoffset! / 2
'  If z% < 10 Then frmRAOB.Print Format$(z%, " 0") Else frmRAOB.Print Format$(z%, "##")
'Next z%




Case 2  ' mb
  For z% = YVl% To YVr% Step Ystep!
    Pmb = fZtoP(z% * 1#)
    frmATP.CurrentX = XVr% + xoffset! / 4
    frmATP.CurrentY = z% - yoffset! / 2
    Select Case Pmb
    Case Is < 100: frmATP.Print Format$(Pmb, " #0")
    Case Is < 1000: frmATP.Print Format$(Pmb, "###")
    Case Else
      frmATP.CurrentX = XVr% + xoffset! / 4
      frmATP.Print Format$(Pmb, "###")
    End Select
  Next z%
Case 3  ' hPa steps of 100
  For i% = 10 To 1 Step -1
    Pmb = i% * 100
    zkm = fPtoZ(Pmb)
    If zkm > YVl And zkm < YVr Then
      frmATP.CurrentX = XVr% + xoffset! / 4
      frmATP.CurrentY = zkm - yoffset! / 2
      Select Case Pmb
      Case Is < 100: frmATP.Print Format$(Pmb, " #0")
      Case Is < 1000: frmATP.Print Format$(Pmb, "###")
      Case Else
        frmATP.CurrentX = XVr% + xoffset! / 4
        frmATP.Print Format$(Pmb, "####")
      End Select
    End If
  Next i%
  ' Do a few special cases
  Pmb = 50 ' mb
  zkm = fPtoZ(Pmb)
  If zkm > YVl And zkm < YVr Then
    frmATP.CurrentX = XVr% + xoffset! / 4
    frmATP.CurrentY = zkm - yoffset! / 2
    frmATP.Print Format$(Pmb, " #0")
  End If
  Pmb = 25 ' mb
  zkm = fPtoZ(Pmb)
  If zkm > YVl And zkm < YVr Then
    frmATP.CurrentX = XVr% + xoffset! / 4
    frmATP.CurrentY = zkm - yoffset! / 2
    frmATP.Print Format$(Pmb, " #0")
  End If

  
End Select

' Write T-axis label
For T% = XVl% To XVr% Step Xstep!
  temp$ = Format$(T%, "###")
  frmATP.CurrentX = T% - frmATP.TextWidth(temp$) '/ 2
  frmATP.CurrentY = YVl% + yoffset! / 2
  frmATP.Print T%
Next T%

frmATP.FONTSIZE = frmATP.FONTSIZE + 1
If BorderStyle = 0 Then frmATP.FontBold = False Else frmATP.FontBold = True

'temp$ = "T [K]"
'frmATP.CurrentX = XVc - frmATP.TextWidth(temp$) / 2
'frmATP.CurrentY = YVl% + 1.5 * frmATP.TextHeight(temp$)
'frmATP.Print temp$

'temp$ = "Z"
'frmATP.CurrentX = XVl% - (Bl + frmATP.TextWidth(temp$) + 3 * xoffset!) / 2
'frmATP.CurrentY = YVc - 1.5 * yoffset!
'frmATP.Print temp$

'temp$ = "[km]"
'frmATP.CurrentX = XVl% - (Bl + frmATP.TextWidth(temp$) + 3 * xoffset!) / 2
'frmATP.CurrentY = YVc
'frmATP.Print temp$
Select Case Yunitl
Case 0
temp$ = "km"
Case 1
temp$ = "kft"
End Select
frmATP.CurrentX = XVl% - Bl   '/ 2 - frmATP.TextWidth(temp$) / 2
frmATP.CurrentY = YVl% + yoffset / 2 '1.5 * frmATP.TextHeight(temp$)
If BorderStyle <> 0 Then frmATP.Print temp$

'temp$ = "Z"
'frmATP.CurrentX = XVr% + (Br - frmATP.TextWidth(temp$) + 3 * xoffset!) / 2
'frmATP.CurrentY = YVc - 1.5 * yoffset!
'frmATP.Print temp$

'temp$ = "[kft]"
'frmATP.CurrentX = XVr% + (Br - frmATP.TextWidth(temp$) + 3 * xoffset!) / 2
'frmATP.CurrentY = YVc
'frmATP.Print temp$
Select Case Yunitr
Case 0: temp$ = "km"
Case 1: temp$ = "kft"
Case 2: temp$ = "mb"
Case 3: temp$ = "hPa"
End Select

frmATP.CurrentX = XVr% + Br - frmATP.TextWidth(temp$)
frmATP.CurrentY = YVl% + yoffset / 2 '1.5 * frmATP.TextHeight(temp$)
If BorderStyle <> 0 Then frmATP.Print temp$

frmATP.FontBold = False
frmATP.FONTSIZE = frmATP.FONTSIZE - 1
End Sub


Sub ATP_Initialize()
Dim PVersion$

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
  LineColor = PFG
  LineThickness = 1
' The System Flags
  S_flag = True             'Show status bar in bottom LH corner      Ctrl+I
  T_flag = True             'Show trop info in upper RH corner        Ctrl+T
  GM_flag = False           'Show global minimum                      Ctrl+Z
  FastFwd = False           'TRUE if fast forwarding thru data (PgUp)
  FastBwd = False           'TRUE if fast backwarding thru data (PgDn)
  AutoStep = False          'TRUE if auto-stepping thru data (INS)
  P_flag = False            'Paint ICE and NAT regions on plot        Ctrl+P
  G_flag = False            'Flag to turn ATP ghost on and off        Ctrl+G
  L_flag = True             'Show -2K/km lapse rate lines             Ctrl+L
  DoNAT = False             'NAT flag
  ATPFileOpen = False       'True if ATP form opens file
  ShowDocument = True       'True if Documentation to be shown
  ShowAAprofile = True      'True if AA Profle to be shown
  ShowPSCthresholds = True  'True if PSC threshold lines are to be drawn
  frmFormatATP.DefaultBG = True
  Dim X%, y%
' Initialize plot window center position, size and borders
  Tscale = 1
  zscale = 1                 'Default Plot scale (1, 2, or 3)           1,2,3
'  zc% = 12: zs% = 24: Tc% = 250: Ts% = 120
'  X% = Ts% / 2 ^ (Tscale - 1) / 2: XVl% = Tc% - X%: XVr% = Tc% + X%
'  Y% = zs% / 2 ^ (zscale - 1) / 2: YVl% = zc% - Y%: YVr% = zc% + Y%

  AutoStepInterval% = 100    'Auto Step cycle time in ms
  FastFBInterval% = 100      'FastFwd/FastBwd Step cycle time in ms
  
  PVersion$ = "MPVIEWER.BAS 960707.0"
  'FooterTitle$ = "Rcrd utsec pALTkm Pitch  Roll   OAT  Zt1  Zt2 Thet1 Thet2 Latitud Longitud LRate"
  'FooterFormat$ = "#### ##### ##.### ###.# ###.# ###.# ##.# ##.# ###.# ###.# ###.### ####.### ###.#"
  FooterTitle$ = " Record  UTsec  pALTkm  Pitch   Roll    OAT   Zt1   Zt2  Theta1  Theta2  Latitude  Longitude  LRate"
  FooterFormat$ = "######  #####  ##.###  ###.#  ###.#  ###.#  ##.#  ##.#  ####.#  ####.#  ####.###  #####.###  ###.#"

' Formatothis$ = "9999999 999.999  99.9  99.9 999.9 999.9 999.9 999.9 999.9 999.999  999.999 999.9"
' FormatCount$ = "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
  If UseTexAQS Then
    ztkmDef = 3: zbkmDef = 0         'NB Default values changed when file is opened
    ztkftDef = 10: zbkftDef = 0
    ztmbDef = 250: zbmbDef = 1000
    zthPaDef = 250: zthPaDef = 1000
    T1KDef = 280: T2KDef = 320          'XV, YV, etc
    PT1KDef = 290: PT2KDef = 430        'Potential Temperature
  Else
    ztkmDef = 28: zbkmDef = 0         'NB Default values changed when file is opened
    ztkftDef = 90: zbkftDef = 0
    ztmbDef = 30: zbmbDef = 1000
    zthPaDef = 30: zthPaDef = 1000
    T1KDef = 180: T2KDef = 300          'XV, YV, etc
    PT1KDef = 250: PT2KDef = 550        'Potential Temperature
  End If

' Deal with C and F later

  ztkm = ztkmDef: zbkm = zbkmDef      'NB Default values changed when file is opened
  ztkft = ztkftDef: zbkft = zbkftDef
  ztmb = ztmbDef: zbmb = zbmbDef
  zthPa = zthPaDef: zbhPa = zbhPaDef
  T1K = T1KDef: T2K = T2KDef            'XV, YV, etc
  PT1K = PT1KDef: PT2K = PT2KDef        'Potential Temperature
' Default parameters
  Xcoord = 0                 '0=Temperature, 1=Potential Temperature
  Ycoordl = 0                '0=pALT, 1=gALT
  Ycoordr = 0                '0=pALT, 1=gALT
' Default units
  Xunit = 0                  '0=K, 1=C, 2=F
  Yunitl = 0                 '0=z[km], 1=z[kft], 2=z[mb], 3=z[hPa]
  Yunitr = 1                 '0=z[km], 1=z[kft], 2=z[mb], 3=z[hPa]
  
  SetDefaultScales
  If BorderStyle = 1 Then
    With frmATP
    .LRL = White               'Lapse Rate lines
    .LRP = Black 'BGray               'Lapse Rate pointers
    .ACP = bGray               'Aircraft pointer
    .ACS = bGray               'Aircraft symbol
    .PBG = White                'Plot Background color
    .PBC = White               'Plot border color
    .PFG = Black              'Plot Foreground color
    .FHL = Green               'Footer header line
    .FDL = Green               'Footer data line
    .PTM = Black              'Plot tic marks and border
    .Pgm = Black 'LGray              'Plot grid color
    .PSL = Green               'Plot status line
    .INL = LGray               'Input line
    End With
    ShowTropopause = False
    ShowDocument = False
    ShowPSCthresholds = False
    Height = 2000                    'Enable to get small ATP plot
    Width = 2000
    ztkmDef = 30: zbkmDef = 0         'NB Default values changed when file is opened
    ztkftDef = 100: zbkftDef = 0
    ztmbDef = 30: zbmbDef = 1000
    zthPaDef = 30: zthPaDef = 1000
    T1KDef = 180: T2KDef = 260          'XV, YV, etc
    PT1KDef = 290: PT2KDef = 430        'Potential Temperature
    With frmFormatATP
    .HScroll(1).Value = 260
    .HScroll(3).Value = 30
    .cmdRefresh_Click
    End With
  End If
  
End Sub





Private Sub Picture1_MouseMove(Button As Integer, Shift As Integer, X As Single, y As Single)
'Picture1.ToolTipText = "X=" + Str(x)
End Sub


