VERSION 5.00
Begin VB.Form frmRAOB 
   AutoRedraw      =   -1  'True
   Caption         =   "RAOB Display"
   ClientHeight    =   2235
   ClientLeft      =   5970
   ClientTop       =   1455
   ClientWidth     =   4200
   Icon            =   "RAOB.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   2235
   ScaleWidth      =   4200
End
Attribute VB_Name = "frmRAOB"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' Some Documentation
'
' ExtForm .................. used to align display wrt to calling form
'
' Properties that can be set remotely or using FormatRAOB Dialog Box
' LineColor
' LineThickness
' ShowDots
' ShowTropopause
' ShowAircraft
' ShowSelectLevel
' ShowDocument
' ShowPressureAltitude
' PlotPT
' PLotEPT
Public ExtForm As Form
Public RAOBinitialize As Boolean, UseTexAQS As Boolean

Public Bl!, Br!, Bt!, Bb!, xoffset!, yoffset!, Ystep!, Xstep!
Public XVl%, XVr%, XVs, XVc%, XVscale!, XVlast!, XVvalue!, XunitPerPixel!
Public YVl%, YVr%, YVs%, YVc%, YVscale!, YVlast!

Public T1%, T2%, PT1%, PT2%, T1K%, T2K%, PT1K%, PT2K%
Public T1def%, T2def%, PT1Def%, PT2Def%, T1KDef%, T2KDef%, PT1KDef%, PT2KDef%

Public Zt%, Zb%, ztkm%, zbkm%, ztkft%, zbkft%, ztmb%, zbmb%, zthPa%, zbhPa%
Public ztDef%, zbDef%, ztkmDef%, zbkmDef%, ztkftDef%, zbkftDef%, ztmbDef, zbmbDef%, zthPaDef, zbhPzDef%

Public zscale!, Tscale%
Public PlotT0 As Boolean, PlotT1 As Boolean, PlotT2 As Boolean, PlotT3 As Boolean

Public ShowRH As Boolean
Public EnableAveraging As Boolean, PlotSingleScan As Boolean
Public ShowAircraft As Boolean, ShowTropopause As Boolean, ShowSelectLevel As Boolean
Public RAOBFileOpen As Boolean, ShowDocument As Boolean, ShowPressureAltitude As Boolean
Public Documentation$
Public Xcoord%, Ycoordl%, Ycoordr%, Xunit%, Yunitl%, Yunitr%
Public PT_flag, BadNr%, EndRecord%, ShowDots As Boolean
Public EnableFit As Boolean  'Set to interpolate ATP

Public LRL%, LRP%, ACP%, ACS%, PBG%, PBC%, PFG%, FHL%, FDL%, PTM%, Pgm%, PSL%, INL%
Public FastFwd As Boolean, FastBwd As Boolean
Public S_flag As Boolean, T_flag As Boolean, GM_flag As Boolean
Public LR_flag As Boolean, P_flag As Boolean, G_flag As Boolean
Public L_flag As Boolean, PlotDone As Boolean, DoNAT As Boolean
Public AutoStep, AutoStepInterval%, FastFBInterval%
'Public Source$, Speed%, Drive$
Public FooterTitle$, FooterFormat$
Public INPlu%, OUTlu%, LineColor%, LineThickness%
Public ALTkm!, Zt1!, Zt2!, Tac!, TT1!, TT2!

'frmRAOB variables
Public Taco!, jt1%, jt2%, jOut%
'Public zt1!, zt2!, Tzt1!, Tzt2!, Theta1!, Theta2!, Tac!
Dim zcs!(1 To 14000), Tcs!(1 To 14000), RCs!(1 To 14000), ETcs!(1 To 14000)
Dim PTcs!(1 To 14000), EPTcs!(1 To 14000)
Sub RAOB_Plot_Description(Description$)
'  frmRAOB.FontSize = frmRAOB.FontSize + 1
  frmRAOB.CurrentX = XVl - Bl
  frmRAOB.CurrentY = YVl - Bb / 1.5
  
  frmRAOB.Print " " + Description$

End Sub

Sub RAOB_Plot_Documentation2()
Dim temp$

  With frmRAOB
    .FontSize = 10
    .ForeColor = QBColor(White)
    .FontSize = frmRAOB.FontSize + 1
    .CurrentX = XVl - Bl
    .CurrentY = YVl - Bb / 1.5
  End With
  frmRAOB.Print Documentation$
End Sub

Sub RAOB_Plot_Documentation()
Dim temp$

If ShowDocument Then
  FontSize = FontSize + 1
  CurrentX = XVl - Bl
  CurrentY = YVl - Bb / 1.5
  temp$ = " WMO " + Format$(WMOnumber, "00000") + " " + WMO4l + " at " + Format$(Iyear, "##") + " "
  temp$ = temp$ + Format$(Imonth, "00") + " "
  temp$ = temp$ + Format$(Iday, "00") + " "
  temp$ = temp$ + Format$(Ihour, "00")
  temp$ = temp$ + " Zt1=" + Format(Zt1, "00.00") + " km"
  If Zt2 > 0 Then temp$ = temp$ + " Zt2=" + Format(Zt2, "00.00") + " km"
  Print temp$
End If

End Sub

Sub RAOB_Plot_Dots(Pindex%)
Dim x!, y!
  If ShowDots Then
    frmRAOB.ForeColor = QBColor(White%)
    frmRAOB.DrawWidth = 5
    For j% = 1 To jOut  'Plot data points
      y = zcs(j%)
      Select Case Pindex
      Case 0: x = Tcs(j)
      Case 1: x = ETcs(j)
      Case 2: x = PTcs(j) 'fTheta(x, fZtoP(y))
      Case 3: x = EPTcs(j)
      End Select
      If x > XVl And x < XVr And y < YVr And y > YVl Then frmRAOB.PSet (x, y)
    Next
  End If
End Sub

Sub RAOB_SelectLevel(lvl%)
Dim y!, x!
' Draw pointers at selected level.
  If lvl > 0 And ShowSelectLevel Then
    If ExtForm.chkGeoPotHeight(0).Value = 1 Then
      y = ZGraob(lvl)
    Else
      y = fPtoZ(PZraob(lvl))
    End If
    x = TZraob(lvl)
    Call RAOB_Pointer_Pair(y, ACP%, -1)
    frmRAOB.Line (x - 5, y)-(x + 5, y), QBColor(ACS%)
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



Sub RAOBupdate()
Dim P!, x!(1 To 3), y!(1 To 3), C!(1 To 3), slope!, pt!
  If frmFormatRAOB.chkTropopause.Value = 1 Then ShowTropopause = True Else ShowTropopause = False
'  EnableFit = True
  If EnableFit Then
  ' PROBLEM: ATP turns on this flag but doesn't turn it off!
    DoSplineFits (EnableFit)   'Spline Fit Data profile and Ghost profile
    For i = 1 To jOut
      P = fZtoP(zcs(i))
      PTcs(i) = fTheta(Tcs(i), P)
      RCs(i) = Rplot(i) + 200  'Range RH from 200 to 300 K
      ETcs(i) = fET(fW(Rplot(i), P, Tcs(i)), Tcs(i))
      EPTcs(i) = fTheta(ETcs(i), P)
      'Debug.Print i; zcs(i); Tcs(i)
    Next i
  Else
    For i = 1 To Nlevr
      zcs(i) = Zplot(i)
      Tcs(i) = Tplot(i)
      P = fZtoP(zcs(i))
      PTcs(i) = fTheta(Tcs(i), P)
      RCs(i) = Rplot(i) + 200  'Range RH from 200 to 300 K
      ETcs(i) = fET(fW(Rplot(i), P, Tcs(i)), Tcs(i))
      EPTcs(i) = fTheta(ETcs(i), P)
      'Debug.Print i; zcs(i); Tcs(i)
    Next i
    jOut = Nlevr
  End If
    
  If 1 = 2 Then
    Call SPLINE(zcs(), PTcs!(), TD1#(), TD2#(), jt1%, jOut)
    For i = 1 To jOut
      If zcs(i) > 12 And zcs(i) < 17 Then
'        y(1) = PTcs(i - 1): y(2) = PTcs(i): y(3) = PTcs(i + 1)
'        x(1) = zcs(i - 1): x(2) = zcs(i): x(3) = zcs(i + 1)
'        Call Qfit(x!(), y!(), 3, c(), 3)
'        slope = c(2) + c(3) * x(2)
        If zcs(i) <> zcs(i - 1) Then dth_dZ = (PTcs(i) - PTcs(i - 1)) / (zcs(i) - zcs(i - 1))
        Debug.Print i; zcs(i); PTcs(i); dth_dZ; TD1(i); TD2(i)
        If zcs(i) = Zt1 Then Debug.Print C(1); C(2); C(3)
      End If
    Next i
  End If
  'For i% = 1 To Iend
  '  RAavg!(i%) = ZPr(i%): RTavg!(i%) = TTr(i%)
 ' Next i%
  jt1% = 1: jt2% = jOut
  If PlotSingleScan Then
    SetDefaultScales
    RAOB_Plot_Set
    RAOB_Plot_Axes_Labels
    RAOB_Plot_Grid_Marks
    RAOB_Plot_Tic_Marks
    RAOB_Plot_Documentation
  End If
  RAOB_Plot

DoEvents

If CurtainFileOpen Then frmCurtain.tmrCurtainAutoStep.Enabled = True

End Sub

Sub DoSplineFits(EnableFit As Boolean)
Dim i%, j%, k%, dZ!, Wg!, jt1%, jt2%

If Nlevr = 0 Then Exit Sub

If EnableFit Then
  ' Do the SPLINE fit to retrieved ATP----------------------------<<<<<
  jt1% = 1: jt2% = Nlevr: jOut% = 1
  Call SPLINE(Zplot!(), Tplot!(), TD1#(), TD2#(), jt1%, Nlevr)
  
  'For i = 1 To Nlevr: Debug.Print i; Zplot(i); Tplot(i); TD1!(i); TD2(i): Next i
  
  zcs!(1) = Zplot!(1)                    ' Then, set levels for Spline Fit
  For k% = jt1% + 1 To jt2%
    dZ! = Zplot!(k%) - Zplot!(k% - 1)
    If dZ! < 2 Then
      jOut = jOut + 1: zcs!(jOut%) = Zplot(k% - 1) + 0.5 * dZ!
      jOut = jOut + 1: zcs!(jOut%) = Zplot(k%)
    Else
      jOut = jOut + 1: zcs!(jOut%) = Zplot(k% - 1) + 0.25 * dZ!
      jOut = jOut + 1: zcs!(jOut%) = Zplot(k% - 1) + 0.5 * dZ!
      jOut = jOut + 1: zcs(jOut%) = Zplot(k% - 1) + 0.75 * dZ!
      jOut = jOut + 1: zcs(jOut%) = Zplot(k%)
    End If
  Next

  ' Calculate the interpolated values for the spline fit display
  jt2% = jOut%
  For j% = 1 To jt2%
    Tcs(j%) = SPLINT(Zplot(), Tplot(), TD2#(), jt1%, Nlevr, zcs(j%), 1)
    'Debug.Print j; zcs(j); Tcs(j); Zplot(j); Tplot(j)
  Next
  If Zt1 < 99# Then TT1 = SPLINT(Zplot(), Tplot(), TD2#(), jt1%, Nlevr, Zt1, 1)
  If Zt2 < 99# Then TT2 = SPLINT(Zplot(), Tplot(), TD2#(), jt1%, Nlevr, Zt2, 1)
  
  Call SPLINE(Zplot!(), Rplot!(), TD1#(), TD2#(), jt1%, Nlevr)
  For j% = 1 To jOut%
    RCs(j%) = SPLINT(Zplot(), Rplot(), TD2#(), jt1%, Nlevr, zcs(j%), 1)
  Next
' Update ghost profile
'    If Tg!(17) = 0 Then
'      If RTp!(17) > 170 And RTp!(17) < 310 Then  'chk quality of OAT
'        For i% = 1 To 33: Zg(i%) = RAp!(i%): Tg(i%) = RTp!(i%): Next i%
'      End If
'    Else
'      Wg! = 0.06
'      For j% = 1 To 33
'        Zg(j%) = (Zg(j%) + Wg! * RAp!(j%)) / (1 + Wg!)
'        Tg(j%) = (Tg(j%) + Wg! * RTp!(j%)) / (1 + Wg!)
'      Next j%
'    End If
Else
  ' Do the SPLINE fit to retrieved ATP----------------------------<<<<<
  jt1% = 1: jt2% = Nlevr: jOut% = 1
  Call SPLINE(Zplot!(), Tplot!(), TD1#(), TD2#(), jt1%, jt2%)

  zcs!(1) = Zplot!(1)                    ' Then, set levels for Spline Fit
  Tcs(1) = Tplot(1)
  For k% = jt1% + 1 To jt2%
    jOut = k
    zcs(jOut%) = Zplot(k%)
    Tcs(jOut) = Tplot(k)
  Next
  If Zt1 < 99# Then TT1 = SPLINT(Zplot(), Tplot(), TD2#(), jt1%, jt2%, Zt1, 1)
  If Zt2 < 99# Then TT2 = SPLINT(Zplot(), Tplot(), TD2#(), jt1%, jt2%, Zt2, 1)
End If

End Sub


Sub RAOB_ChangeLimits()
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




Sub RAOB_Plot_Curve(z!(), T!(), ad%, colour%)
' Plot a curve given by array pairs (z,T) and array dimension ad%
' Modified 2004.07.14 to properly extrapolate to the edges of the window using an
' algorithm based on boundaries determined by the edges and slopes from a given
' point to each of the corners.
Dim i%, LastPointOutsideWindow As Boolean, M!, m1!, m2!, m3!, m4!

frmRAOB.ForeColor = QBColor(colour%)
If LineColor >= 0 Then frmRAOB.ForeColor = QBColor(LineColor)
If LineThickness > 0 Then frmRAOB.DrawWidth = LineThickness

' Initialize flag to indicate whether last point plotted was outside window
If (T(1) > XVl And T(1) < XVr) And (z(1) > YVl And z(1) < YVr) Then
  LastPointOutsideWindow = False
Else
  LastPointOutsideWindow = True
End If

For i = 1 To ad
  ' First check that last point was not equal to border value
  If (T(i) >= XVl And T(i) <= XVr) And (z(i) >= YVl And z(i) <= YVr) Then
  ' Point is inside window
    If LastPointOutsideWindow And i > 1 Then
      M = (T(i) - T(i - 1)) / (z(i) - z(i - 1)) 'Slope dT/dz
      m1 = (XVr - T(i)) / (YVr - z(i))
      m2 = (XVr - T(i)) / (YVl - z(i))
      m3 = (XVl - T(i)) / (YVl - z(i))
      m4 = (XVl - T(i)) / (YVr - z(i))
      ' Top of window
      If z(i - 1) > YVr And (M > m4 And M <= m1) Then
        frmRAOB.PSet (T(i) + M * (YVr - z(i)), YVr)
      End If
      ' Right of window
      If T(i - 1) > XVr And (M > m1 Or M <= m2) Then
        frmRAOB.PSet (XVr, z(i) + (XVr - T(i)) / M)
      End If
      ' Bottom of window
      If z(i - 1) < YVl And (M > m2 And M <= m3) Then
        frmRAOB.PSet (T(i) + M * (YVl - z(i)), YVl)
      End If
      ' Left of window
      If T(i - 1) < XVl And (M > m3 Or M <= m4) Then
        frmRAOB.PSet (XVl, z(i) + (XVl - T(i)) / M)
      End If
      frmRAOB.Line -(T!(i), z!(i))      'And continue from there
      LastPointOutsideWindow = False
    Else
      If i = 1 Then
        frmRAOB.PSet (T(i), z(i))
      Else
        frmRAOB.Line -(T!(i), z!(i))
      End If
    End If
  
  Else  ' Extend from last point inside window to edge of window
    If i > 1 Then
    If (YVr - z(i - 1) <> 0) And (YVl - z(i - 1) <> 0) And (YVl - z(i - 1) <> 0) And (YVr - z(i - 1) <> 0) Then
      If Not LastPointOutsideWindow Then
        M = (T(i) - T(i - 1)) / (z(i) - z(i - 1)) 'Slope dT/dz
        m1 = (XVr - T(i - 1)) / (YVr - z(i - 1))
        m2 = (XVr - T(i - 1)) / (YVl - z(i - 1))
        m3 = (XVl - T(i - 1)) / (YVl - z(i - 1))
        m4 = (XVl - T(i - 1)) / (YVr - z(i - 1))
        ' Top of window
        If z(i) > YVr And (M > m4 And M <= m1) Then
          frmRAOB.Line -(T(i - 1) + M * (YVr - z(i - 1)), YVr)
        End If
        ' Right of window
        If T(i) > XVr And (M > m1 Or M <= m2) Then
          frmRAOB.Line -(XVr, z(i - 1) + (XVr - T(i - 1)) / M)
        End If
        ' Bottom of window
        If z(i) < YVl And (M > m2 And M <= m3) Then
          frmRAOB.Line -(T(i - 1) + M * (YVl - z(i - 1)), YVl)
        End If
        ' Left of window
        If T(i) < XVl And (M > m3 Or M <= m4) Then
          frmRAOB.Line -(XVl, z(i - 1) + (XVl - T(i - 1)) / M)
        End If
      End If
      LastPointOutsideWindow = True
    End If
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
'  frmRAOB.RAOBupdate
'
'  If CurtainFileOpen Then
'    If flag Then frmCurtain.tmrCurtainAutoStep.Enabled = True
'  End If

RequestRAOBupdate = True
End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub


Private Sub Form_Load()
  
  RAOBinitialize = True
Picture = LoadPicture("c:\MTP\VB6\DIB\rainbow.dib")  ' Load bitmap.
  Show

'With frmRAOB
'  .Top = 0
'  .Left = frmRAOBman.Width
'  .Width = 800 * Screen.TwipsPerPixelX
'  .Height = 600 * Screen.TwipsPerPixelY
'End With
'  RAOB_Initialize
' NB Resize event is where initialization occurs
End Sub


Private Sub Form_Resize()

' Don't Resize when form is loaded
If Not RAOBinitialize Then
  RAOBupdate
Else
  RAOB_Initialize
  RAOBinitialize = False
End If

End Sub


Sub RAOB_Plot_Grid_Marks()
Dim z%, T%
frmRAOB.ForeColor = QBColor(Pgm%)
frmRAOB.DrawWidth = 2
  Select Case zscale              'Deal with grid marks
  Case 1
    For z% = YVl% + 1 To YVr% - 1 Step 2
      For T% = XVl% + 10 To XVr% - 10 Step 10: frmRAOB.Line (T% - 0.06, z%)-(T% + 0.06, z%): Next T%
    Next z%

    For z% = YVl% + 2 To YVr% - 2 Step 2
      For T% = XVl% + 5 To XVr% - 5 Step 5: frmRAOB.Line (T% - 0.1, z%)-(T% + 0.1, z%): Next T%
    Next z%

  Case Is > 1
    For z% = YVl% + 1 To YVr% - 1
      For T% = XVl% + 2 To XVr% - 2 Step 2                    ' Dot field
        If T% <= 250 Then frmRAOB.Line (T% - 0.05, z%)-(T% + 0.05, z%) ' 2 K increments, ea Z
        If T% > 250 Then frmRAOB.Line (T% - 0.1, z%)-(T% + 0.1, z%)   ' empirical fix
        If T% Mod 10 = 0 Then                               ' 10 K inc, ea Z
          frmRAOB.Line (T%, z% - 0.05)-(T%, z% + 0.05)
          If T% <= 250 Then frmRAOB.Line (T% - 0.05, z%)-(T% + 0.05, z%)
          If T% > 250 Then frmRAOB.Line (T% - 0.15, z%)-(T% + 0.15, z%) ' empirical fix
        End If
      Next T%
    Next z%

    For z% = YVl% + 0.5 To YVr% - 0.5 Step 1
      For T% = XVl% + 10 To XVr% - 10 Step 10: frmRAOB.Line (T%, z - 0.02)-(T%, z + 0.02): Next T%
    Next z%
  End Select

End Sub


Sub RAOB_Plot_Tic_Marks()
Dim z%, T%, tic!, zk!, Ykstep!, Pmb!
  frmRAOB.ForeColor = QBColor(PTM%)
' Now deal with tic marks
' Tic marks for left hand side (km)
  For z% = YVl% + 1 To YVr% - 1 Step 1
    If z% Mod 2 = 0 Then tic! = 1.5 / zscale Else tic! = 0.75 / zscale
    frmRAOB.Line (XVl%, z%)-(XVl% + tic!, z%)
  Next z%

' Tic marks for upper & lower temperature scale (K)
  If YVr < 6 Then Ykstep = 0.125 Else Ykstep = 0.25
  For T% = XVl% + 5 To XVr% - 5 Step 5       'Upper/lower border tics
    If T% Mod 10 = 0 Then tic! = 2 * Ykstep / zscale Else tic! = Ykstep / zscale
    frmRAOB.Line (T%, YVl%)-(T%, YVl% + tic!): frmRAOB.Line (T%, YVr%)-(T%, YVr% - tic)
  Next T%

' Make ticks on right side (kft)
Select Case Yunitr
Case 0 'km
  For z% = YVl% + 1 To YVr% - 1 Step 1
    If z% Mod 2 = 0 Then tic! = 1.5 / zscale Else tic! = 0.75 / zscale
    frmRAOB.Line (XVr% - tic!, z%)-(XVr%, z%)
  Next z%

Case 1 'kft
  Ykstep = Ystep * 2.5
  If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
  For z% = zk To kft_km * YVr% Step Ykstep
    If z% Mod 10 = 0 Then tic! = 1.5 / zscale Else tic! = 0.75 / zscale
    frmRAOB.Line (XVr% - tic, z% / kft_km)-(XVr%, z% / kft_km)
  Next z%
Case 2 'mb
  For z% = YVl% + 1 To YVr% - 1 Step 1
    If z% Mod 2 = 0 Then tic! = 1.5 / zscale Else tic! = 0.75 / zscale
    frmRAOB.Line (XVr% - tic!, z%)-(XVr%, z%)
  Next z%

Case 3 'hPa
  For i% = 10 To 1 Step -1
    Pmb = i% * 100
    zkm = fPtoZ(Pmb)
    If zkm > YVl And zkm < YVr Then
      tic! = 1.5 / zscale
      frmRAOB.Line (XVr% - tic!, zkm)-(XVr%, zkm)
    End If
  Next i%
  Pmb = 50 ' mb
  zkm = fPtoZ(Pmb)
  If zkm > YVl And zkm < YVr Then
    frmRAOB.Line (XVr% - tic!, zkm)-(XVr%, zkm)
  End If
  Pmb = 25 ' mb
  zkm = fPtoZ(Pmb)
  If zkm > YVl And zkm < YVr Then
    frmRAOB.Line (XVr% - tic!, zkm)-(XVr%, zkm)
  End If
End Select
End Sub


Sub RAOB_Pointer_Pair(z!, C%, f%)
' z  ... Altitude of pointer
' C% ... Color of pointer
' F% ... Flag to indicate whether or not to fill with color
Dim z_inc!, T_inc!
  If z <> 99.9 Then
    z_inc = 0.4 / zscale: T_inc = 2 / zscale
    frmRAOB.ForeColor = QBColor(C%)
   
    frmRAOB.Line (XVl%, z)-Step(T_inc, z_inc)     'Left pointer
    frmRAOB.Line -Step(0, -2 * z_inc)
    frmRAOB.Line -(XVl%, z)
'    If F% Then paint STEP(T_inc * 0.9, 0)
   
    frmRAOB.Line (XVr%, z)-Step(-T_inc, z_inc)    'Right pointer
    frmRAOB.Line -Step(0, -2 * z_inc)
    frmRAOB.Line -(XVr%, z)
'    If F% Then paint STEP(-T_inc * 0.9, 0)
  End If
End Sub



Sub RAOB_Plot()
Dim x!, y!, z!, Lmax%, LmaxI%, LmaxN%, L%, Tnat!, C%, Pn%, Pi%, Zi!, Tit!, Ti!, Zn!, TN!
Dim Tile$, DrawWidthSave%, j%, jt1%, jt2%, DrawColorSave%
jt1 = 1: jt2 = jOut
' Plot LR=-2 [K/km] line if Trops exist, also draw Trop pointers
  If ShowTropopause Then
    Call RAOB_Pointer_Pair(Zt1, LRP%, -1)
'  Line (XVl%, zt1)-(TN(1), zt1), QBColor(LRP%)
    If Zt2 < 99 Then Call RAOB_Pointer_Pair(Zt2, LRP%, -1)              'Plot trop#2 pointers
  End If
' Plot Caspar if he is visible
'  RAOB_Draw_Ghost_Line
  frmRAOB.ForeColor = QBColor(White%)
  DrawWidthSave = frmRAOB.DrawWidth
  
'  LineColor = 14
  If PlotT0 Then  'Temperature
'   Plot lines between data points
'    frmRAOB.DrawWidth = 2
'    LineColor = Yellow
    Call RAOB_Plot_Curve(zcs(), Tcs(), jOut, PFG)
    RAOB_Plot_Dots (0)
    frmRAOB.DrawWidth = DrawWidthSave
  End If
  
  If PlotT1 Then  'Equivalent Temperature
    LineColor = LRed
    Call RAOB_Plot_Curve(zcs(), ETcs(), jOut, PFG)
    RAOB_Plot_Dots (1)
    frmRAOB.DrawWidth = DrawWidthSave
  End If
  
  If PlotT2 Then  'Potential Temperature
    LineColor = Yellow
    Call RAOB_Plot_Curve(zcs(), PTcs(), jOut, PFG)
    RAOB_Plot_Dots (2)
    frmRAOB.DrawWidth = DrawWidthSave
    y = fTheta(TT1, fZtoP(Zt1))
    If ShowTropopause Then frmRAOB.Line (y - 5, Zt1)-(y + 5, Zt1), QBColor(ACS%)
End If
  
  If PlotT3 Then  'Equivalent Potential Temperature
    LineColor = Yellow
    Call RAOB_Plot_Curve(zcs(), EPTcs(), jOut, PFG)
    RAOB_Plot_Dots (3)
    frmRAOB.DrawWidth = DrawWidthSave
  End If
  
  If ShowRH Then
    frmRAOB.DrawWidth = 1
    LineColor = 10
    Call RAOB_Plot_Curve(zcs(), RCs(), jOut, PFG)
  End If
  
  ' Draw pointers at A/C Altitude. Do last to put on top
  If ALTkm <> 99.9 And ShowAircraft Then
    'Call ATP_Pointer_Pair(ALTkm, ACP%, -1)
    If PT_flag Then y = fTheta(Tac, fZtoP(ALTkm)) Else y = Tac
    frmRAOB.Line (y - 5, ALTkm)-(y + 5, ALTkm), QBColor(ACS%)
  End If

    frmRAOB.DrawWidth = 2 'DrawWidthSave
  If ShowTropopause Then Call RAOB_Draw_LR_Lines(Zt1!, TT1!, Zt2!, TT2!)          'Uses old values to draw lines
 
End Sub

Sub RAOB_Draw_Ghost_Line()
Dim Tg1!(1 To 33), Tg2!(1 To 33), jg1%, jg2%, j%

If Not G_flag Then Exit Sub
'Plot new ghost
' Figure out number of non-zero ghost profile levels
'  jg1% = 0: Do: INC jg1%: Loop Until Zg(jg1%) > 0 Or jg1% = 33
'  jg2% = 34: Do: DEC jg2%: Loop Until Zg(jg2%) > 0 Or jg2% = 1
'  INC jg1%: DEC jg2%
'  If jg1% < 33 Then   'No retrieved ghost levels
'   Do the SPLINE fit to ghost profile
'    Call SPLINE(Zg(), Tg(), Tg1(), Tg2(), jg1%, jg2%)
'   And calculate the interpolated values for the spline fit display
'    For j% = 1 To jout%
'      Tgs(j%) = SPLINT(Zg(), Tg(), Tg2(), jg1%, jg2%, zcs(j%), 1)
'    Next
'    Call RAOB_Plot_Curve(zcs(), Tgs(), jout%, Cyan%)
'  End If
End Sub

Sub RAOB_Draw_LR_Lines(Zt1!, TT1!, Zt2!, TT2!)
Dim T_inc!
  T_inc = 3 / zscale
  If Not L_flag Then Exit Sub
  frmRAOB.ForeColor = QBColor(LRL%)
  frmRAOB.DrawStyle = 2

  If Zt1 < 99.9 Then
    frmRAOB.DrawWidth = 1

    If PT_flag Then
      frmRAOB.Line (XVr% - T_inc, Zt1)-(XVl% + T_inc, Zt1)
    Else
      If TT1 > XVl Then
        frmRAOB.Line (XVr% - T_inc, Zt1)-(TT1, Zt1)
        frmRAOB.Line (TT1, Zt1)-(TT1 - 2 * (YVl - Zt1), YVl)
      End If
      If TT1 > XVl And TT1 < XVr Then
        If Zt1 > YVl And Zt1 < YVr Then
        frmRAOB.DrawWidth = 6
        frmRAOB.PSet (TT1, Zt1)
        End If
      End If
    End If
  End If
 
  If Zt2 < 99.9 Then
    frmRAOB.DrawWidth = 1

    If PT_flag Then
      frmRAOB.Line (XVr% - T_inc, Zt2)-(XVl% + T_inc, Zt2)
    Else
      frmRAOB.Line (XVr% - T_inc, Zt2)-(TT2, Zt2)
      frmRAOB.Line (TT2, Zt2)-(TT2 + 2 * Zt2, 0)
      frmRAOB.DrawWidth = 6
      frmRAOB.PSet (TT1, Zt1)
     End If
  End If
  frmRAOB.DrawWidth = 1
  frmRAOB.DrawStyle = 0
End Sub



Sub RAOB_Plot_Set()
Dim x!, y!, Fonts%

With frmRAOB
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

frmRAOB.Scale (XVl% - Bl, YVr% + Bt)-(XVr% + Br, YVl% - Bb)
frmRAOB.DrawWidth = 2
frmRAOB.Line (XVl%, YVr%)-(XVr%, YVl%), QBColor(PBG%), BF  'Blue background
frmRAOB.Line (XVl%, YVr%)-(XVr%, YVl%), QBColor(PTM%), B   'Yellow border
frmRAOB.DrawWidth = 1

' Set right (km) and left (kft) step sizes for labelling
If XVs > 80 Then Xstep! = 10 Else Xstep! = 5
If YVs > 15 Then Ystep! = 2 Else Ystep! = 1

'Determine new font size
frmRAOB.FontName = "Small Fonts"

If frmFormatRAOB.chkFontSize.Value = 0 Then
  Fonts% = 1
  Do
    frmRAOB.FontSize = Fonts%
    Fonts% = Fonts% + 1
    xoffset! = frmRAOB.TextWidth("2")
    x = 1.75 * (3 * xoffset!)    'Separate 3 characters by half their width
  Loop Until x > Xstep!
  'frmFormatRAOB.Visible = False
Else  ' Or user-specified font size
  Fonts% = frmFormatRAOB.txtFontSize
  frmRAOB.FontSize = Fonts%
End If

frmFormatRAOB.vscFontSize.Value = 19  'Fonts%
yoffset! = frmRAOB.TextHeight("1")
xoffset! = frmRAOB.TextWidth("1")
If frmRAOB.FontSize > 7 Then frmRAOB.FontName = "Courier New"  '"MS Sans Serif"

End Sub

Sub RAOB_Plot_Axes_Labels()
Dim z%, zk, T%, temp$, Ykstep!, zkm!, Pmb!
' Label km axis
Select Case Yunitl
Case 0
  For z% = YVl% To YVr% Step Ystep!
    frmRAOB.CurrentX = XVl% - 4.5 * xoffset!
    frmRAOB.CurrentY = z% - yoffset! / 2
    If z% < 10 Then frmRAOB.Print Format$(z%, " 0") Else frmRAOB.Print Format$(z%, "##")
  Next z%
'Case 1
'Ykstep = 5 * Ystep
''If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
'For z% = zk To YVr% Step Ykstep '* kft_km! Step Ykstep!
'  frmRAOB.CurrentX = XVr% + xoffset!
'  frmRAOB.CurrentY = z% - yoffset! / 2 ' / kft_km! - yoffset! / 2
'  If z% < 10 Then frmRAOB.Print Format$(z%, " 0") Else frmRAOB.Print Format$(z%, "##")
'Next z%
End Select

' Label kft on right side
Select Case Yunitr
Case 0  'km
For z% = YVl% To YVr% Step Ystep!
  frmRAOB.CurrentX = XVr% + xoffset!
  frmRAOB.CurrentY = z% - yoffset! / 2
  If z% < 10 Then frmRAOB.Print Format$(z%, " 0") Else frmRAOB.Print Format$(z%, "##")
Next z%

Case 1  'kft
If YVr < 6 Then Ykstep = 2 * Ystep Else Ykstep = 5 * Ystep
If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl% * kft_km! / Ykstep) * Ykstep
For z% = zk To YVr% * kft_km! Step Ykstep!
  frmRAOB.CurrentX = XVr% + xoffset!
  frmRAOB.CurrentY = z% / kft_km! - yoffset! / 2
  If z% < 10 Then frmRAOB.Print Format$(z%, " 0") Else frmRAOB.Print Format$(z%, "##")
Next z%

Case 2  ' mb
  For z% = YVl% To YVr% Step Ystep!
    Pmb = fZtoP(z% * 1#)
    frmRAOB.CurrentX = XVr% + xoffset!
    frmRAOB.CurrentY = z% - yoffset! / 2
    Select Case Pmb
    Case Is < 100: frmRAOB.Print Format$(Pmb, " #0")
    Case Is < 1000: frmRAOB.Print Format$(Pmb, "###")
    Case Else
      frmRAOB.CurrentX = XVr% + xoffset! / 10
      frmRAOB.Print Format$(Pmb, "###")
    End Select
  Next z%
Case 3  ' hPa steps of 100
  For i% = 10 To 1 Step -1
    Pmb = i% * 100
    zkm = fPtoZ(Pmb)
    If zkm > YVl And zkm < YVr Then
      frmRAOB.CurrentX = XVr% + xoffset!
      frmRAOB.CurrentY = zkm - yoffset! / 2
      Select Case zkm
      Case Is < 100: frmRAOB.Print Format$(Pmb, " #0")
      Case Is < 1000: frmRAOB.Print Format$(Pmb, "###")
      Case Else
        frmRAOB.CurrentX = XVr% + xoffset! / 10
        frmRAOB.Print Format$(Pmb, "###")
      End Select
    End If
  Next i%
  ' Do a few special cases
  Pmb = 50 ' mb
  zkm = fPtoZ(Pmb)
  If zkm > YVl And zkm < YVr Then
    frmRAOB.CurrentX = XVr% + xoffset!
    frmRAOB.CurrentY = zkm - yoffset! / 2
    frmRAOB.Print Format$(Pmb, " #0")
  End If
  Pmb = 25 ' mb
  zkm = fPtoZ(Pmb)
  If zkm > YVl And zkm < YVr Then
    frmRAOB.CurrentX = XVr% + xoffset!
    frmRAOB.CurrentY = zkm - yoffset! / 2
    frmRAOB.Print Format$(Pmb, " #0")
  End If

  
End Select

' Write T-axis label
For T% = XVl% To XVr% Step Xstep!
  temp$ = Format$(T%, "###")
  frmRAOB.CurrentX = T% - frmRAOB.TextWidth(temp$) / 2
  frmRAOB.CurrentY = YVl% + yoffset! / 2
  frmRAOB.Print T%
Next T%

frmRAOB.FontSize = frmRAOB.FontSize + 1
frmRAOB.FontBold = True

temp$ = "T [K]"
frmRAOB.CurrentX = XVc - frmRAOB.TextWidth(temp$) / 2
frmRAOB.CurrentY = YVl% + 1.5 * frmRAOB.TextHeight(temp$)
frmRAOB.Print temp$

'temp$ = "Z"
'frmRAOB.CurrentX = XVl% - (Bl + frmRAOB.TextWidth(temp$) + 3 * xoffset!) / 2
'frmRAOB.CurrentY = YVc - 1.5 * yoffset!
'frmRAOB.Print temp$

'temp$ = "[km]"
'frmRAOB.CurrentX = XVl% - (Bl + frmRAOB.TextWidth(temp$) + 3 * xoffset!) / 2
'frmRAOB.CurrentY = YVc
'frmRAOB.Print temp$
Select Case Yunitl
Case 0
temp$ = "km"
Case 1
temp$ = "kft"
End Select
frmRAOB.CurrentX = XVl% - Bl / 2 - frmRAOB.TextWidth(temp$) / 2
frmRAOB.CurrentY = YVl% + 1.5 * frmRAOB.TextHeight(temp$)
frmRAOB.Print temp$

'temp$ = "Z"
'frmRAOB.CurrentX = XVr% + (Br - frmRAOB.TextWidth(temp$) + 3 * xoffset!) / 2
'frmRAOB.CurrentY = YVc - 1.5 * yoffset!
'frmRAOB.Print temp$

'temp$ = "[kft]"
'frmRAOB.CurrentX = XVr% + (Br - frmRAOB.TextWidth(temp$) + 3 * xoffset!) / 2
'frmRAOB.CurrentY = YVc
'frmRAOB.Print temp$
Select Case Yunitr
Case 0: temp$ = "km"
Case 1: temp$ = "kft"
Case 2: temp$ = "mb"
Case 3: temp$ = "hPa"
End Select

frmRAOB.CurrentX = XVr% + Br / 2 - frmRAOB.TextWidth(temp$) / 2
frmRAOB.CurrentY = YVl% + 1.5 * frmRAOB.TextHeight(temp$)
frmRAOB.Print temp$

frmRAOB.FontBold = False
frmRAOB.FontSize = frmRAOB.FontSize - 1
End Sub


Sub RAOB_Initialize()
' And assign to some attributes to facilitate changing
  LRL = White               'Lapse Rate lines
  LRP = LGray               'Lapse Rate pointers
  ACP = White               'Aircraft pointer
  ACS = White               'Aircraft symbol
  PBG = Blue                'Plot Background color
  PBC = Black               'Plot border color
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
  G_flag = False            'Flag to turn RAOB ghost on and off        Ctrl+G
  L_flag = True             'Show -2K/km lapse rate lines             Ctrl+L
  DoNAT = False             'NAT flag
  RAOBFileOpen = False       'True if RAOB form opens file
  ShowDocument = True       'True if Documentation to be shown
Dim x%, y%
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

  Formatothis$ = "9999999 999.999  99.9  99.9 999.9 999.9 999.9 999.9 999.9 999.999  999.999 999.9"
  FormatCount$ = "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
  
  If UseTexAQS Then
    ztkmDef = 3: zbkmDef = 0         'NB Default values changed when file is opened
    ztkftDef = 10: zbkftDef = 0
    ztmbDef = 250: zbmbDef = 1000
    zthPaDef = 250: zthPaDef = 1000
    T1KDef = 280: T2KDef = 320          'XV, YV, etc
    PT1KDef = 290: PT2KDef = 430        'Potential Temperature
  Else
    ztkmDef = 40: zbkmDef = 0         'NB Default values changed when file is opened
    ztkftDef = 90: zbkftDef = 0
    ztmbDef = 30: zbmbDef = 1000
    zthPaDef = 30: zthPaDef = 1000
    T1KDef = 180: T2KDef = 300          'XV, YV, etc
    PT1KDef = 290: PT2KDef = 430        'Potential Temperature
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
End Sub





