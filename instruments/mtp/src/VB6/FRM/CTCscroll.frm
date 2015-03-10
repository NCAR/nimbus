VERSION 5.00
Begin VB.Form frmCTC 
   AutoRedraw      =   -1  'True
   BorderStyle     =   0  'None
   Caption         =   "CTCplot"
   ClientHeight    =   3855
   ClientLeft      =   75
   ClientTop       =   4605
   ClientWidth     =   6690
   Icon            =   "CTCscroll.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   PaletteMode     =   2  'Custom
   ScaleHeight     =   3855
   ScaleWidth      =   6690
   ShowInTaskbar   =   0   'False
   Begin VB.Timer tmrCurtainAutoStep 
      Enabled         =   0   'False
      Left            =   240
      Top             =   240
   End
End
Attribute VB_Name = "frmCTC"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' CTC Flags
Public Curtaininitialize As Boolean
Public CurtainPlotUpdate As Boolean
Public ShowLatitude As Boolean
Public ShowDocument As Boolean
Public AutoATPupdate As Boolean
Public RequestATPupdate As Boolean
Public ReLabel As Boolean
Public CurtainAutoStepInterval
Public CurtainFileOpen As Boolean
Public EnableAveraging As Boolean
Public ShowAircraft As Boolean
Public ShowTropopause As Boolean
Public ShowColdPoint As Boolean
Public ShowMRI As Boolean
Public IncreasingLatitude As Boolean
Public IncreasingLongitude As Boolean
Public PlotGoodScan As Boolean
Public PlotGoodTrop As Boolean
Public SubstituteLastATP As BorderStyleConstants
Public PlotPalt As Boolean
Public Preliminary As Boolean
Public ShowTheta As Boolean
Public PlotCTC As Boolean
Public PlotIAC As Boolean
Public IACstarted As Boolean

Public Bl!, Br!, Bt!, Bb!, xoffset!, yoffset!, Ystep!, Xstep!
Public SamplesAveraged, XVend!, XunitsPerPixel!
Public XVl%, XVr%, XVs!, XVc!, XVscale!, XVlast!, XVvalue!, XunitPerPixel!
Public YVl%, YVr%, YVs!, YVc!, YVscale!, YVlast!
Public ZVl%, ZVr%, ZVs!, ZVc!, ZVscale!, ZVlast!
Public Xcoord%, Ycoordl%, Zcoord%, Filename$, FileRecords%
Public BadNr%, Record%, Speed%
Public ThetaStart%, ThetaEnd%, ThetaStep%, ThetaPen%
Public ZgMinusZp!, yy_mm_dd$, yymmdd$, ymdd$  ', ZYX$
Public ColorCycles%, ColorStyle%
Public Zt1AVG!, Zt2Avg!, AltAvg!, Zt1Cnt!, Zt2Cnt!, AltCnt!, MRIavg1!, MRIcnt!
Public ZcpAVG!, ZcpCnt!
Public UTH1, UTH2, UTS1, UTS2
Public LAT2, LAT1, LON2, LON1
Public LATstart!, LATend!, LONstart!, LONend!, UTstart&, UTend&
Public PT1%, PT2%, T1%, T2%
Public Zt%, Zb%, zs%, zc%, zscale
Public zt1last!, zt2last!, ALTkmLast!, Nlevlast!, UTsecLast!
Public PBG, PFG, PTM

'frmATP variables
Public Taco!, jt1%, jt2%, jOut%, FirstPoint As Boolean

Private Type LOGFONT
  lfHeight As Long
  lfWidth As Long
  lfEscapement As Long
  lfOrientation As Long
  lfWeight As Long
  lfItalic As Byte
  lfUnderline As Byte
  lfStrikeOut As Byte
  lfCharSet As Byte
  lfOutPrecision As Byte
  lfClipPrecision As Byte
  lfQuality As Byte
  lfPitchAndFamily As Byte
' lfFaceName(LF_FACESIZE) As Byte 'THIS WAS DEFINED IN API-CHANGES MY OWN
  lfFacename As String * 33
End Type

Private Declare Function CreateFontIndirect Lib "GDI32" Alias "CreateFontIndirectA" (lpLogFont As LOGFONT) As Long
Private Declare Function SelectObject Lib "GDI32" (ByVal hDC As Long, ByVal hObject As Long) As Long
Private Declare Function DeleteObject Lib "GDI32" (ByVal hObject As Long) As Long

Function fMJcolor&(n%)
  
  If n > 25 Then n = n Mod 25
  Select Case n
  Case 1: fMJcolor = &H80&      'dark red
  Case 2: fMJcolor = &HFF&      'red
  Case 3: fMJcolor = &HC0C0FF   'light red
  Case 4: fMJcolor = &H4080&
  Case 5: fMJcolor = &H80FF&    'orange
  Case 6: fMJcolor = &HC0E0FF
  Case 7: fMJcolor = &H8080&
  Case 8: fMJcolor = &HFFFF&    'yellow
  Case 9: fMJcolor = &HC0FFFF
  Case 10: fMJcolor = &H8000&
  Case 11: fMJcolor = &HFF00&   'green
  Case 12: fMJcolor = &HC0FFC0
  Case 13: fMJcolor = &H808000
  Case 14: fMJcolor = &HFFFF00  'light blue
  Case 15: fMJcolor = &HFFFFC0
  Case 16: fMJcolor = &H800000
  Case 17: fMJcolor = &HFF0000  'blue
  Case 18: fMJcolor = &HFFC0C0
  Case 19: fMJcolor = &H800080
  Case 20: fMJcolor = &HFF00FF  'purple
  Case 21: fMJcolor = &HFFC0FF
  Case 22: fMJcolor = &H404040
  Case 23: fMJcolor = &HC0C0C0  'grey
  Case 24: fMJcolor = &HFFFFFF  'white
  Case 25: fMJcolor = &H0&      'black
  End Select

End Function


Sub ContinuousColor(T!, r%, g%, b%)
Dim Tindex%, Tfraction!, Color%

  Tfraction = 6# * T  '0 to 6
  Tindex = Int(Tfraction)                      '0, 1, 2, 3, 4, 5
  Color = Int(255# * (Tfraction - Tindex) + 0.5)
  Select Case Tindex
  Case 0: r = 0: g = 0: b = Color
  Case 1: r = 0: g = Color: b = 255
  Case 2: r = 0: g = 255: b = 255 - Color
  Case 3: r = Color: g = 255: b = 0
  Case 4: r = 255: g = 255 - Color: b = 0
  Case 5: r = 255 - Color: g = 0: b = 0
  Case Else: r = 0: g = 0: b = 0
  End Select

End Sub

Sub DIALcolor(IC%, r, g, b)
    
  Select Case IC%
  Case 0: r = 255: g = 255: b = 255
  Case 1: r = 255: g = 140: b = 255
  Case 2: r = 221: g = 111: b = 255
  Case 3: r = 187: g = 82: b = 255
  Case 4: r = 153: g = 54: b = 255
  Case 5: r = 119: g = 25: b = 255
  Case 6: r = 178: g = 255: b = 255
  Case 7: r = 133: g = 204: b = 255
  Case 8: r = 89: g = 153: b = 255
  Case 9: r = 44: g = 102: b = 255
  Case 10: r = 0: g = 51: b = 255
  Case 11: r = 191: g = 255: b = 191
  Case 12: r = 143: g = 255: b = 143
  Case 13: r = 95: g = 255: b = 95
  Case 14: r = 50: g = 255: b = 47
  Case 15: r = 0: g = 255: b = 0
  Case 16: r = 255: g = 255: b = 0
  Case 17: r = 255: g = 207: b = 0
  Case 18: r = 255: g = 159: b = 0
  Case 19: r = 255: g = 111: b = 0
  Case 20: r = 255: g = 63: b = 0
  Case 21: r = 255: g = 0: b = 0
  Case 22: r = 216: g = 0: b = 15
  Case 23: r = 178: g = 0: b = 31
  Case 24: r = 140: g = 0: b = 47
  Case 25: r = 102: g = 0: b = 63
  Case Else: r = 0: g = 0: b = 0
  End Select
' Debug.Print IC%; R; G; B
  
End Sub


Sub LoadLimitsSummary(Filename$)
Dim Readlu%

  If CurtainFileOpen Then
    REFreadHEADER (Filename$)
    Load frmSummary
    UpdateLimitSummary
    frmSummary.Show
    frmSummary.Refresh
  
  Else
    MsgBox "You have not opened a data file, please do so (F4).", 48, "File|Start Command"
  End If

End Sub


Sub ReLabelCheck()

  If ReLabel Then  'ReLabel left axis in case it is over-written by data
    Select Case Xcoord
    Case 0
      If UTH1 < UTH2 Then
        If (XVlast - XVl) > XVs / 50 Then ReLabel = False
      Else
        If (XVlast - XVr) < -XVs / 50 Then ReLabel = False
      End If
  
    Case 1
      If LAT1 < LAT2 Then
        If (XVlast - XVl) > XVs / 50 Then ReLabel = False
      Else
        If (XVlast - XVr) < -XVs / 50 Then ReLabel = False
      End If
  
    Case 2
      If LON1 < LON2 Then
        If (XVlast - XVl) > XVs / 50 Then ReLabel = False
      Else
        If (XVlast - XVr) < -XVs / 50 Then ReLabel = False
      End If
  
    Case 3
      If UTS1 < UTS2 Then
        If (XVlast - XVl) > XVs / 50 Then ReLabel = False
      Else
        If (XVlast - XVr) < -XVs / 50 Then ReLabel = False
      End If
  
    End Select

    If ReLabel = False Then Curtain_Plot_Tic_Marks

End If
End Sub

 Sub UpdateLimitSummary()
  
  With frmSummary
  .Text1.Text = Format(UTmin, "#00000")
  .Text2.Text = Format(UTmax, "#00000")
  .Text3.Text = Format(LATmin, "###0.000")
  .Text4.Text = Format(LATmax, "###0.000")
  .Text5.Text = Format(LONmin, "##0.000")
  .Text6.Text = Format(LONmax, "##0.000")
  .Text7.Text = Format(Pitchmin, "##0.0")
  .Text8.Text = Format(PitchMax, "##0.0")
  .Text9.Text = Format(Rollmin, "##0.0")
  .Text10.Text = Format(RollMax, "##0.0")
  .Text11.Text = Format(pALTmin, "#0.00")
  .Text12.Text = Format(pALTmax, "#0.00")
  .Text13.Text = Format$(Tacmin, "#00.0")
  .Text14.Text = Format$(Tacmax, "#00.0")
  .Text15.Text = Format$(Zt1min, "#0.00")
  .Text16.Text = Format$(Zt1max, "#0.00")
  .Text17.Text = Format$(T1min, "#00.0")
  .Text18.Text = Format$(T1max, "#00.0")
  .Text19.Text = Format$(PT1min, "#00.0")
  .Text20.Text = Format$(PT1max, "#00.0")
  .Text21.Text = Format$(Zt2min, "#0.00")
  .Text22.Text = Format$(Zt2max, "#0.00")
  .Text23.Text = Format$(PT2min, "#00.0")
  .Text24.Text = Format$(PT2max, "#00.0")
  .Text25.Text = Format$(T2min, "#00.0")
  .Text26.Text = Format$(T2max, "#00.0")
  .Text27.Text = Format(LRmin, "##0.0")
  .Text28.Text = Format(LRmax, "##0.0")
  .Text29.Text = Format(Zmin, "#0.00")
  .Text30.Text = Format(Zmax, "#0.00")
  .Text31.Text = Format(Tmin, "#00.0")
  .Text32.Text = Format(Tmax, "#00.0")
  .Text33.Text = Format(zTmin, "#0.00")
  .Text34.Text = Format(ZtMax, "#0.00")
  .Text35.Text = Format$(PTmin, "#00.0")
  .Text36.Text = Format$(PTmax, "#00.0")
  .Text37.Text = Format$(MRImin, "#0.0")
  .Text38.Text = Format$(MRImax, "#0.0")
  .Text43.Text = Format$(gALTmin, "#0.00")
  .Text44.Text = Format$(gALTmax, "#0.00")
  .Text41.Text = fSecToTstring(UTmin, True)
  .Text42.Text = fSecToTstring(UTmax, True)
  End With

End Sub



Sub CustomColor(IC%, r, g, b)
    
  Select Case IC%
  Case 0: r = 0: g = 0: b = 0
  Case 1: r = 0: g = 0: b = 64
  Case 2: r = 0: g = 0: b = 128
  Case 3: r = 0: g = 0: b = 192
  Case 4: r = 0: g = 0: b = 255
  Case 5: r = 0: g = 64: b = 255
  Case 6: r = 0: g = 128: b = 255
  Case 7: r = 0: g = 192: b = 255
  Case 8: r = 0: g = 255: b = 255
  Case 9: r = 0: g = 255: b = 192
  Case 10: r = 0: g = 255: b = 128
  Case 11: r = 0: g = 255: b = 64
  Case 12: r = 0: g = 255: b = 0
  Case 13: r = 64: g = 255: b = 0
  Case 14: r = 128: g = 255: b = 0
  Case 15: r = 192: g = 255: b = 0
  Case 16: r = 255: g = 255: b = 0
  Case 17: r = 255: g = 192: b = 0
  Case 18: r = 255: g = 128: b = 0
  Case 19: r = 255: g = 64: b = 0
  Case 20: r = 255: g = 0: b = 0
  Case 21: r = 192: g = 0: b = 0
  Case 22: r = 128: g = 0: b = 0
  Case 23: r = 64: g = 0: b = 0
  Case Else: r = 0: g = 0: b = 0
  End Select
 'Debug.Print IC%; R; G; B
  
End Sub

Sub SetDefaultScales()
' These are not integers; therefore, round
'If UTS1 < UTS2 Then UTS1 = UTS1 - 1: UTS2 = UTS2 + 1 Else UTS1 = UTS1 + 1: UTS2 = UTS2 - 1
'If LAT1 < LAT2 Then LAT1 = LAT1 - 1: LAT2 = LAT2 + 1 Else LAT1 = LAT1 + 1: LAT2 = LAT2 - 1
'If LON1 < LON2 Then LON1 = LON1 - 1: LON2 = LON2 + 1 Else LON1 = LON1 + 1: LON2 = LON2 - 1
'If UTH1 < UTH2 Then UTH1 = UTH1 - 1: UTH2 = UTH2 + 1 Else UTH1 = UTH1 + 1: UTH2 = UTH2 - 1

  Select Case Xcoord
  Case 0
    XVl = UTH1: XVr = UTH2
  Case 1
    If IncreasingLatitude And LAT1 < LAT2 Then
      XVl = LAT1: XVr = LAT2
    Else
      XVl = LAT2: XVr = LAT1
    End If
  Case 2
    If IncreasingLongitude And LON1 < LON2 Then
      XVl = LON1: XVr = LON2
    Else
      XVl = LON2: XVr = LON1
    End If
  Case 3
    XVl = UTS1: XVr = UTS2
  End Select

  XVs = XVr - XVl: XVc = XVl + XVs / 2
  YVl = Zb: YVr = Zt
  YVs = YVr - YVl: YVc = YVl + YVs / 2

  Select Case Zcoord
  Case 0
    ZVl = T1: ZVr = T2
  Case 1
    ZVl = PT1: ZVr = PT2
  End Select
  ZVs = ZVr - ZVl: ZVc = ZVl + ZVs / 2

  Record% = 0
 
End Sub

Sub GetREFcycle(ByVal PlotUpdateFlag As Boolean)
'Read new profile
Dim XV!
Static sTTT%(1 To 33), sZZZ%(1 To 33), i%

TryAgain:
'  On Error GoTo errhandler
  If Not RealTime And EOF(CTClu%) And Record > 1 Then  '
    mnuRunClose_Click
    Exit Sub
  End If
  
  INC Record%
  If Record% > FileRecords Then    'Or Record% < 1 Then
    mnuRunClose_Click
    Exit Sub
  End If
  
  If ReadREF Then
    Call REFread(CTClu%, Record%)
  Else
    Call MPreadNew(CTClu%, Record%)
  End If
  
  If Record% = 2 Then zt1last = Zt1
  
  If Not PlotPalt Then
    ZgMinusZp = (sZg(16) - sZZZ(16))  'Calculate Zgeo offset in km
    For i = Nlev1 To Nlev2
      sZZZ(i) = sZg(i)  'use gps alt if not pressure altitude
    Next i
  End If

' Skip records with bad Scans or Trops
  'Debug.Print Record; GoodScan
  
  If Not GoodScan And PlotGoodScan And CurtainFileOpen And Record% <= FileRecords Then
    GoTo TryAgain
    If SubstituteLastATP Then
      For i = Nlev1 To Nlev2    'Reverse order (Must be increasing altitude!)
'        zzz%(i) = sZZZ(i)
'        TTT(i) = sTTT(i)
      Next i
    End If
'   Use last good scan
    If MakeWord > 8 Then
      GoTo TryAgain
    End If
  End If
  
'  Debug.Print Record; EditWord
  If CurtainFileOpen And MakeWord > 8 Then
'    GoTo TryAgain
  End If
  
  If Not GoodTrop And PlotGoodTrop And CurtainFileOpen And Record% <= FileRecords Then
    frmCTC.ShowTropopause = False 'GoTo TryAgain
  Else
    frmCTC.ShowTropopause = True
  End If

  If CurtainFileOpen Then
    Select Case Xcoord
    Case 0
      XV = UTsec / 3600
      If XV < XVl Then GoTo TryAgain
      If XV > XVr Then mnuRunClose_Click: Exit Sub
    
    Case 1
      XV = Latitude
      If LATstart < LATend Then   'Plot left to right
        If XVl < XVr Then
          If XV < XVl Then GoTo TryAgain
          If XV > XVr Then mnuRunClose_Click: Exit Sub
        Else
          If XV > XVl Then GoTo TryAgain
          If XV < XVr Then mnuRunClose_Click: Exit Sub
        End If
      Else
        If XVl < XVr Then        'Plot right to left
          If XV > XVr Then GoTo TryAgain
          If XV < XVl Then mnuRunClose_Click: Exit Sub
        Else                     'Plot left to right
          If XV < XVl Then GoTo TryAgain
          If XV > XVr Then mnuRunClose_Click: Exit Sub
        End If
      End If
  
    Case 2
      XV = Longitude
      If LONstart < LONend Then   'Plot left to right
        If XVl < XVr Then
          If XV < XVl Then GoTo TryAgain
          If XV > XVr Then mnuRunClose_Click: Exit Sub
        Else
          If XV > XVl Then GoTo TryAgain
          If XV < XVr Then mnuRunClose_Click: Exit Sub
        End If
      Else
        If XVl < XVr Then        'Plot right to left
          If XV > XVr Then GoTo TryAgain
          If XV < XVl Then mnuRunClose_Click: Exit Sub
        Else                     'Plot left to right
          If XV < XVl Then GoTo TryAgain
          If XV > XVr Then mnuRunClose_Click: Exit Sub
        End If
      End If
    
    Case 3
      XV = UTsec / 1000
      If XV < XVl Then GoTo TryAgain
      If XV > XVr Then mnuRunClose_Click: Exit Sub
  
    End Select
  
    XVvalue = XV
    If SamplesAveraged = 0 Then
      For i% = 1 To 33: RAavg(i%) = 0: RTavg(i%) = 0:  Next
      Zt1AVG = 0#: Zt2Avg = 0#: AltAvg = 0#: MRIavg1 = 0#: ZcpAVG = 0#
      Zt1Cnt = 0: Zt2Cnt = 0: AltCnt = 0: MRIcnt = 0: ZcpCnt = 0
      
      Select Case Xcoord
      Case 0
        If UTH1 < UTH2 Then
          XVend = XV + XunitsPerPixel
        Else
          XVend = XV - XunitsPerPixel
        End If
      
      Case 1
        If LATstart < LATend Then
          XVend = XV + XunitsPerPixel
        Else
          XVend = XV - XunitsPerPixel
        End If
      
      Case 2
        If LONstart < LONend Then
          XVend = XV + XunitsPerPixel
        Else
          XVend = XV - XunitsPerPixel
        End If
    
      Case 3
        If UTS1 < UTS2 Then
          XVend = XV + XunitsPerPixel
          If XV < 86.4 And XVend > 86.4 Then GoTo TryAgain
        Else
          XVend = XV - XunitsPerPixel
        End If
    
      End Select
    End If
  End If
  
  If Not GoodScan Or Not GoodTrop Then INC BadNr%      'Increment Bad record count if new
  
  If PlotUpdateFlag Then Plot_Update
  
  If GoodScan Then       'Save scan if good
    For i = Nlev1 To Nlev2    'Reverse order (Must be increasing altitude!)
      sZZZ(i) = zzz%(i) / 100#
      sTTT(i) = TTT(i) / 10#
    Next i
  End If
  
Exit Sub

errhandler:
  Msg = "Input file unavailable?"                 ' Define message.
  Style = vbYesNo + vbCritical + vbDefaultButton1 ' Define buttons.
  Title = "WARNING!"                              ' Define title.
  Help = ""                                       ' Define Help file.
  Ctxt = 0                                        ' Define topic
  response = MsgBox(Msg, Style, Title, Help, Ctxt)
  If response = vbYes Then

  Else
    End
  End If

End Sub
Private Sub mnuRunClose_Click()
  frmCTC.tmrCurtainAutoStep.Enabled = False  'Disable during plot update
  frmCTC.Speed = 1
  'frmMTPdata.txtFile.Text = ""       'In case file is reopened
  Close INPlu%
  frmCTC.CurtainFileOpen = False
  frmCTC.Curtain_Plot_Tic_Marks      'In case tics are over-written
  frmCTC.Speed = 1
  If DoAll Then
    frmCurtain.mnuFileSaveBMP_Click
    DoAll = False
    Unload Me
  End If
End Sub


Sub Plot_Update()
Dim i%, Newzzz!(1 To 33), NewTTT!(1 To 33)
Static NLevSave

  If Speed% = 3 Then Exit Sub
' Read new data
  For i = Nlev1 To Nlev2    'Reverse order (Must be increasing altitude!)
    Newzzz!(i - Nlev1 + 1) = sZZZ(i)
    NewTTT!(i - Nlev1 + 1) = sTTT(i)
  'Debug.Print i; Newzzz(Nlev2 - i + 1); NewTTT(Nlev2 - i + 1); zzz(i); TTT(i)
  Next i

  ReLabelCheck
    
  If Abs((UTsecLast - UTsec) / (UTmax - UTmin)) > XVs / 200 Then XVlast = XVvalue


  If EnableAveraging Then
    If Abs(ALTkmLast - ALTkm) < 0.1 And Abs(zt1last - Zt1) < 1# And Nlev = Nlevlast And Abs(UTsecLast - UTsec) < 20 Then
      SamplesAveraged = SamplesAveraged + 1
      If Zt1 < 99.9 Then Zt1AVG = Zt1AVG + Zt1: Zt1Cnt = Zt1Cnt + 1
      If Zt2 < 99.9 Then Zt2Avg = Zt2Avg + Zt2: Zt2Cnt = Zt2Cnt + 1
      If Zcp < 99.9 Then ZcpAVG = ZcpAVG + Zcp: ZcpCnt = ZcpCnt + 1
      If ALTkm < 99.9 Then: AltAvg = AltAvg + ALTkm: AltCnt = AltCnt + 1
      MRIavg1 = MRIavg1 + MRI: MRIcnt = MRIcnt + 1
      'If Zcp > 0# Then ZcpAvg = ZcpAvg + 1
      For i = 1 To Nlev
        RAavg!(i) = RAavg(i) + Newzzz(i)
        RTavg!(i) = RTavg(i) + NewTTT(i)
      Next i

'   Finished summing, now calculate average
      For i = 1 To Nlev
        RAavg(i) = RAavg(i) / SamplesAveraged
        RTavg(i) = RTavg(i) / SamplesAveraged
      Next
      If Zt1Cnt > 0 Then Zt1AVG = Zt1AVG / Zt1Cnt
      If Zt2Cnt > 0 Then Zt2Avg = Zt2Avg / Zt2Cnt
      If ZcpCnt > 0 Then ZcpAVG = ZcpAVG / ZcpCnt
      If AltCnt > 0 Then AltAvg = AltAvg / AltCnt
      If MRIcnt > 0 Then MRIavg1 = MRIavg1 / MRIcnt
    
      SamplesAveraged = 0
      Curtain_Plot

    Else
      If SamplesAveraged = 0 Then    'Nothing to average
        For i = 1 To Nlev     'Reverse order (Must be increasing altitude!)
          RAavg!(i) = Newzzz(i)
          RTavg!(i) = NewTTT(i)
      '  Debug.Print i; RAavg(Nlev - i + 1); RTavg(Nlev - i + 1); zzz(i); TTT(i)
        Next i
        Zt1AVG = Zt1: Zt2Avg = Zt2: AltAvg = ALTkm: MRIavg1 = MRI
        Zt1Cnt = 1: Zt2Cnt = 1: AltCnt = 1: MRIcnt = 1
        Curtain_Plot

      Else                     'Some samples averaged before Nlev or ALTkm change
        For i = 1 To Nlevlast
          RAavg(i) = RAavg(i) / SamplesAveraged
          RTavg(i) = RTavg(i) / SamplesAveraged
        Next
        If Zt1Cnt > 0 Then Zt1AVG = Zt1AVG / Zt1Cnt
        If Zt2Cnt > 0 Then Zt2Avg = Zt2Avg / Zt1Cnt
        If AltCnt > 0 Then AltAvg = AltAvg / AltCnt
        If MRIcnt > 0 Then MRIavg1 = MRIavg1 / MRIcnt
        NLevSave = Nlev
        Nlev = Nlevlast
        Curtain_Plot           'Plot to current sample
        Nlev = NLevSave        'For code below
        Nlevlast = Nlev        'For next sample
  
  '  Then save current sample
        For i = 1 To Nlev     'Reverse order (Must be increasing altitude!)
          RAavg!(i) = zzz%(i)
          RTavg!(i) = TTT!(i)
        Next i
        SamplesAveraged = 1    'And set sample count to 1
        Zt1AVG = Zt1: Zt2Avg = Zt2: AltAvg = ALTkm: MRIavg1 = MRI
        Zt1Cnt = 1: Zt2Cnt = 1: AltCnt = 1: MRIcnt = 1
        XVend = XVvalue + XunitsPerPixel  'Set new end time
      End If
    End If

  Else  'EnableAveraging=False
    For i = 1 To Nlev     'Reverse order (Must be increasing altitude!)
      RAavg!(i) = Newzzz(i)
      RTavg!(i) = NewTTT(i)
    Next i
    Zt1AVG = Zt1: Zt2Avg = Zt2: AltAvg = ALTkm: MRIavg1 = MRI
    Zt1Cnt = 1: Zt2Cnt = 1: AltCnt = 1: MRIcnt = 1
    Curtain_Plot
    XVvalue = XVvalue + XunitsPerPixel
    Curtain_Plot
  End If

End Sub

Sub Curtain_Initialize()
' And assign to some attributes to facilitate changing
  PBG = Black                 'Plot Background color
  PFG = Yellow                'Plot Foreground color
  PTM = Yellow                'Plot tic marks and border
  PBG = White                 'Plot Background color
  PFG = Black                 'Plot Foreground color
  PTM = Black                 'Plot tic marks and border

  Record% = 1                 'Display this profile in saved matrix
  BadNr% = 0                  'Count number of bad cycles
  
' The System Flags
  ShowTropopause = True
  If ShowTropopause Then frmCurtain.mnuOptionsTropopause.Checked = True
  ShowAircraft = True
  If ShowAircraft Then frmCurtain.mnuOptionsAltitude.Checked = True
  ShowColdPoint = False
  If ShowColdPoint Then frmCurtain.mnuOptionsColdPoint.Checked = True
  ShowMRI = True
  ShowLatitude = False
  ShowDocument = True
  FirstPoint = True
  PlotGoodScan = True
  PlotGoodTrop = True
  IncreasingLatitude = True
  IncreasingLongitude = True
  frmCurtain.chkEnableAveraging.Value = 1
  RequestATPupdate = False
  If EnableAveraging Then frmCurtain.mnuOptionsAveraging.Checked = True
  CurtainFileOpen = False    'Set TRUE when a file has been opened for a curtain plot
  ColorCycles = 1
  ColorStyle = 0
  
' Initialize plot window center position, size and borders
  zscale = 1
  Zmax = 28
  Zt = 28: Zb = 0            'NB Default values changed when file is opened
  LAT1 = -90: LAT2 = 90      'Otherwise never changed
  LON1 = -180: LON2 = 180    'Plot routine use derived values
  T1 = 180: T2 = 280         'XV, YV, etc
  PT1 = 250: PT2 = 550       'Potential Temperature
  UTH1 = 0: UTH2 = 24
  UTS1 = 0: UTS2 = 87

  Xcoord = 3                 '0=UT[hr], 1=Latitude[deg], 2=Longitude[deg], 3=UT[k-sec]
  Zcoord = 0                 '0=T[[K], 1=PT[K]

  Select Case Xcoord
  Case 0:    XVl = UTH1: XVr = UTH2
  Case 1:    XVl = LAT1: XVr = LAT2
  Case 2:    XVl = LON1: XVr = LON2
  Case 3:    XVl = UTS1: XVr = UTS2
  End Select
  XVs = XVr - XVl: XVc = XVl + XVs / 2

  YVl = Zb: YVr = Zt
  YVs = YVr - YVl: YVc = YVl + YVs / 2

  Select Case Zcoord
  Case 0:    ZVl = T1: ZVr = T2
  Case 1:    ZVl = PT1: ZVr = PT2
  End Select
  ZVs = ZVr - ZVl: ZVc = ZVl + ZVs / 2

  CurtainAutoStepInterval = 1    'Auto Step cycle time in ms
  IACstarted = False

End Sub
Sub Curtain_Plot_Color_Bar()
Dim ZV!, r%, g%, b%, x!, C!, ZVstep%, temp$, Constant1!, Constant2!
Dim tnorm!, y1!, y2!, IC%

' Write T-axis label
  x = XVr + 0.4 * Br
  frmCTC.Line (x, YVl)-(x + Br / 8, YVr), QBColor(PFG), B
  If Zcoord = 0 Then ZVstep = 10 Else ZVstep = 20
  For ZV = ZVl To ZVr Step ZVstep
    temp$ = Format$(ZV, "###")
    frmCTC.CurrentY = YVl - frmCTC.TextHeight(temp$) / 2 + YVs / (ZVr - ZVl) * (ZV - ZVl)
    frmCTC.CurrentX = x + Br / 6
    frmCTC.Print temp$
  Next ZV

  frmCTC.FontBold = True
  If Zcoord = 0 Then temp$ = "T[K]" Else temp$ = "PT[K]"
  frmCTC.CurrentX = x + Br / 6
  frmCTC.CurrentY = YVl + 1.5 * frmCTC.TextHeight(temp$)
  frmCTC.Print temp$

' Paint color bar
  ZVstep = 1
  Constant1 = ColorCycles * 100 / (ZVr - ZVl)
  Constant2 = (YVr - YVl) / (ZVr - ZVl)
  For ZV = ZVl To ZVr - ZVstep Step ZVstep
    Select Case ColorStyle
    Case 0
      C = Int(24 * (ZV - ZVl) / (ZVr - ZVl)) / 24# 'Range 0 to 1
      Call ContinuousColor(C, r, g, b)
'    IC% = Int(24 * (ZV - ZVl) / (ZVr - ZVl))
'    Call CustomColor(IC%, r, g, b)
    Case 1     'Go thru 360 degrees of Hue
      tnorm = 3.6 * Constant1 * (ZV - ZVl) Mod 360
      Call HsvToRgb(tnorm, 100, 100, r, g, b)
    Case 2     'Go thru 100 levels of Saturation, Full luminosity
      IC% = Int(27 * (ZV - ZVl) / (ZVr - ZVl))
      Call DIALcolor(IC%, r, g, b)
    Case 3
      Call ContinuousColor((ZV - ZVl) / (ZVr - ZVl), r, g, b)
    End Select
    y1 = YVl + Constant2 * (ZV - ZVl)
    y2 = YVl + Constant2 * (ZV - ZVl + ZVstep)
    frmCTC.Line (x, y1)-(x + Br / 8, y2), RGB(r, g, b), BF

  Next ZV

End Sub

Sub Curtain_Plot_Tic_Marks()
Dim XV!, YV!, UT!, tic!, zk!, Ykstep!

  frmCTC.DrawWidth = 2
  frmCTC.Line (XVl, YVr)-(XVr, YVl), QBColor(PFG), B
  frmCTC.DrawWidth = 1

  frmCTC.ForeColor = QBColor(PTM)
' Now deal with tic marks
' Tic marks for left hand side (km)
  For YV = YVl To YVr Step Ystep / 2  'WAS -1 and +1
    If 10 * YV Mod 10 * Ystep = 0 Then tic = XVs / 50 Else tic = XVs / 100
    frmCTC.Line (XVl, YV)-(XVl + tic, YV)
  Next YV

' Tic marks for upper & lower time scale (hr)
  For XV = XVl To XVr Step Xstep / 2 'Upper/lower border tics
   If XV - Int(XV) = 0 Then tic = YVs / 35 Else tic = YVs / 70
    frmCTC.Line (XV, YVl)-(XV, YVl + tic)
    frmCTC.Line (XV, YVr)-(XV, YVr - tic)
  Next XV
  
  Select Case Ystep
  Case 1, 2: Ykstep = 5 * Ystep
  Case Else: Ykstep = 4 * Ystep
  End Select

  If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl * kft_km! / Ykstep) * Ykstep
' Make ticks on right side (kft)
  Select Case Ystep
  Case 1, 2: Ykstep = 5 * Ystep
  Case Else: Ykstep = 4 * Ystep
  End Select
  
  For YV = zk To kft_km * YVr Step Ykstep / 2
    If 10 * YV Mod 10 * Ykstep = 0 Then tic! = XVs / 50 Else tic! = XVs / 100
    frmCTC.Line (XVr - tic, YV / kft_km)-(XVr, YV / kft_km)
  Next YV
  
End Sub

Sub Curtain_Plot()
Dim ZV!, YV!, x!, y!, z!, T!, Zstep!, XV!, tnorm, RGBcolor, YVvalue!, ZVnorm!, IC%
Dim i%, j%, k%, dZ!, Wg!, r%, g%, b%, Constant!, Theta!(1 To 33), TD1#(1 To 33), TD2#(1 To 33)
Dim pt!, zPT!, tic!, temp$, nl%, First As Boolean, C!, YVstep!

Static zPTlast!(1 To 20), LastTrop!

' Finally, do the SPLINE fit to retrieved ATP
  Call SPLINE(RAavg!(), RTavg!(), TD1(), TD2(), 1, Nlev)

  XV = XVvalue
  YVstep = (YVr - YVl) / 100
  Constant = ColorCycles * 100 / (ZVr - ZVl)
  First = True
  
  For YV = RAavg(1) To RAavg(Nlev) Step YVstep
  'Debug.Print YV; YVl
    If YV > YVl Then
      If YV + YVstep >= YVr Then Exit For
      YVvalue! = YV + YVstep / 2
      ZV = SPLINT(RAavg!(), RTavg!(), TD2(), 1, Nlev, YVvalue!, 1)
      If Zcoord = 1 Then ZV = fTheta(ZV, fZtoP(YVvalue!)) 'Calc PT
      If ZV < ZVl Then ZV = ZVl 'Avoid RGB overflow
  
      Select Case ColorStyle
      Case 0
        C = Int(24 * (ZV - ZVl) / (ZVr - ZVl)) / 24# 'Range 0 to 1, but not continuous
        Call ContinuousColor(C, r, g, b)
'        IC% = Int(24 * (ZV - ZVl) / (ZVr - ZVl))
'        Call CustomColor(IC%, r, g, b)
      Case 1     'Go thru 360 degrees of Hue
        ZVnorm = 3.6 * Constant * (ZV - ZVl) Mod 360
        Call HsvToRgb(ZVnorm, 100, 100, r, g, b)
      Case 2     'Go thru 100 levels of Saturation, Full luminosity, Blue Hue (240)
        IC% = Int(27 * (ZV - ZVl) / (ZVr - ZVl))
        Call DIALcolor(IC%, r, g, b)
      Case 3
        Call ContinuousColor((ZV - ZVl) / (ZVr - ZVl), r, g, b)
      End Select
      frmCTC.ForeColor = RGB(r, g, b)
      If PlotCTC Then
        If First Then XVlast = XV: YVlast = YV: First = False
        frmCTC.Line (XVlast, YVlast)-(XV, YV), , BF
      End If
    End If
    YVlast = YV
  Next YV

' Plot trop altitude
  frmCTC.DrawWidth = 3  'One pixel wide
  If Not PlotPalt Then
    Zt1AVG = Zt1AVG + ZgMinusZp
    Zt2Avg = Zt2Avg + ZgMinusZp
    AltAvg = AltAvg + ZgMinusZp
  End If
  ShowTropopause = True
  If Zt1Cnt > 0 And ShowTropopause Then frmCTC.Line (XVlast, Zt1AVG)-(XV, Zt1AVG), QBColor(White)
  If Zt2Cnt > 0 And ShowTropopause Then frmCTC.Line (XVlast, Zt2Avg)-(XV, Zt2Avg), QBColor(White)
  If AltCnt > 0 And ShowAircraft And AltAvg >= YVl And AltAvg < YVr Then
    If AltAvg >= Zt1AVG Then
      frmCTC.Line (XVlast, AltAvg)-(XV, AltAvg), QBColor(Black)
    Else
      frmCTC.Line (XVlast, AltAvg)-(XV, AltAvg), QBColor(Black)
    End If
  End If
  If ZcpCnt > 0 And ShowColdPoint Then frmCTC.Line (XVlast, ZcpAVG)-(XV, ZcpAVG), QBColor(bGray)
  If MRIcnt > 0 And ShowMRI Then
    frmCTC.Line (XVlast, MRIavg1 + YVl)-(XV, MRIavg1 + YVl), QBColor(LGray)
  End If
'
  If PlotIAC And Abs(ALTkmLast - ALTkm) < 0.05 And ALTkm > 5 Then
' Code to insert PT levels
  ' Convert T profile to PT profile
    For j = 1 To Nlev
      Theta(j) = fTheta(RTavg(j), fZtoP(RAavg(j)))
    Next j
  ' Do spline fit to theta profile
    Call SPLINE(Theta!(), RAavg!(), TD1(), TD2(), 1, Nlev)
'
    frmCTC.DrawWidth = ThetaPen  'One pixel wide
    nl = Int((ThetaEnd - ThetaStart)) / ThetaStep + 1
    For j = 1 To nl
      pt = ThetaStart + (j - 1) * ThetaStep
      zPT = SPLINT(Theta!(), RAavg(), TD2(), 1, Nlev, pt, 1)
      If zPT >= YVl And zPT <= YVr Then
        If Not IACstarted And (XV - XVl) > TextWidth("00000") Then
          temp$ = Format(pt, "000")
          tic = XVs / 50
          With frmCTC
            .CurrentX = XVl + tic
            .CurrentY = zPT - YVs / 50
            .ForeColor = QBColor(Black)
          End With
          frmCTC.Print temp$
        End If
        If FirstPoint Then XVlast = XV: zPTlast(j) = zPT
        frmCTC.Line (XVlast, zPTlast(j))-(XV, zPT), QBColor(Blue)
      End If
      zPTlast(j) = zPT
    Next j
    
    If (XV - XVl) > TextWidth("00000") Then IACstarted = True
    LastTrop = Zt1
  End If
' End of PT code

  frmCTC.DrawWidth = 1  'One pixel wide
  XVlast = XV
  zt1last = Zt1
  zt2last = Zt2
  ALTkmLast = ALTkm
  UTsecLast = UTsec
  Nlevlast = Nlev
  CurtainPlotUpdate = True
  FirstPoint = False
End Sub

Sub Curtain_Plot_Set()
Dim x!, y!, Fonts%
' Define plot border areas relative to plot area
  Bl = 0.1 * XVs: Br = 1.5 * Bl
  Bt = 0.075 * YVs: Bb = 3 * Bt

  frmCTC.Cls
  frmCTC.Scale (XVl - Bl, YVr + Bt)-(XVr + Br, YVl - Bb)
  frmCTC.ScaleMode = 0
  frmCTC.ForeColor = QBColor(PFG)
  frmCTC.BackColor = QBColor(PBG)
  frmCTC.DrawWidth = 2
  frmCTC.Line (XVl, YVr)-(XVr, YVl), QBColor(PBG), BF
  frmCTC.Line (XVl, YVr)-(XVr, YVl), QBColor(PFG), B
  frmCTC.DrawWidth = 1
' Determine X-units per pixel for current display
' Note that Width is full window size including plot borders, and
' is measured in Twips (1440 twips per printed inch, screen independent)
  XunitsPerPixel = (XVr - XVl + Br + Bl) / frmCTC.Width * Screen.TwipsPerPixelX

' Determine new font size so that labels will fit in border
  frmCTC.FontName = "Small Fonts"
  Fonts = 1
  Do
    frmCTC.FONTSIZE = Fonts
    Fonts = Fonts + 1
    xoffset! = 2 * frmCTC.TextWidth("1") '20010819
    x = 2 * (2 * xoffset!)  'Plot 2 digit number w/space either side
  Loop Until Abs(x) > Abs(Bl)

  If Fonts > 13 Then frmCTC.FONTSIZE = 13 'Keep font size small enough

  yoffset! = frmCTC.TextHeight("1")
  xoffset! = frmCTC.TextWidth("2")
  If frmCTC.FONTSIZE > 7 Then frmCTC.FontName = "MS Sans Serif"

' Figure out steps on x-axis relative to scale
  Select Case Xcoord
  Case 0
    Select Case XVs
    Case Is > 4: Xstep = 1#
    Case Is > 2: Xstep = 0.5
    Case Else: Xstep = 0.25
    End Select
  Case 1, 2, 3
    Select Case XVs
    Case Is > 32: Xstep = 8
    Case Is > 16: Xstep = 4
    Case Is > 8: Xstep = 2
    Case Else: Xstep = 1
    End Select
  End Select

' Set right (km) and left (kft) step sizes for labelling
  Select Case YVs
  Case Is > 16: Ystep = 2
  Case Is > 8: Ystep = 1
  Case Else: Ystep = 0.5
  End Select

End Sub

Sub Curtain_Plot_Axes_Labels()
Dim z%, zk%, XV!, temp$, Ykstep!, y!, T!, Tstep!, UT1!, UT2!, tnorm!, r, g, b, RGBcolor
Dim offset!, Xvalue!, Ylabel$, Used$, YV!, TSfmt$, x$

  With frmCTC
  .ForeColor = QBColor(PFG)
  .FontBold = True
  .FontName = "Arial"
  End With
  If PlotPalt Then Ylabel$ = "Pressure Altitude" Else Ylabel$ = "Geometric Altitude"
  Call fRotateText(Ylabel$, FontName, FONTSIZE + 2, 90#, XVl - 0.9 * Bl, (YVl + (YVs - TextWidth(Ylabel$) / 3) / 2))

' Put title on plot
'  Select Case Zcoord
'  Case 0: temp$ = "Temperature"
'  Case 1: temp$ = "Potential Temperature"
'  End Select
'  temp$ = temp$ + " (Pressure Altitude, "

'  Select Case Xcoord
'  Case 0: temp$ = temp$ + "Time"
'  Case 1: temp$ = temp$ + "Latitude"
'  Case 2: temp$ = temp$ + "Longitude"
'  Case 3: temp$ = temp$ + "Time"
'  End Select
'  temp$ = temp$ + ")"
  temp$ = "JPL Microwave Temperature Profiler (MTP)"

  frmCTC.CurrentX = XVl
  frmCTC.CurrentY = YVr - 1.1 * TextHeight(temp$)
  frmCTC.Print temp$
  Used = TextWidth(temp$)

' Plot File Name

  Used = Used + TextWidth(temp$)
  temp$ = Mission$ + "  Flight Date: " + YYYYMMDD$

  frmCTC.CurrentX = XVr - TextWidth(temp$)
  frmCTC.CurrentY = YVr - 1.1 * TextHeight(temp$)
  frmCTC.Print temp$
  frmCTC.FontBold = False
  
' Label km axis
  For YV = YVl To YVr Step Ystep!
    frmCTC.CurrentX = XVl - 2.5 * xoffset!
    frmCTC.CurrentY = YV - yoffset! / 2
    Select Case YV
    Case Is >= 10: frmCTC.Print Format$(YV, "##")
    Case Else
      If YVr >= 8 Then frmCTC.Print Format$(YV, " 0") Else frmCTC.Print Format$(YV, "0.0")
    End Select
  Next YV
' frmCTC.FontSize = frmCTC.FontSize + 1
  temp$ = "km"
  frmCTC.FontBold = True
  frmCTC.CurrentX = XVl - 3 * xoffset!
  frmCTC.CurrentY = YVl + 1.5 * frmCTC.TextHeight(temp$)
  frmCTC.Print temp$
  frmCTC.FontBold = False
  

' Label kft on right side
  Select Case Ystep
  Case 1, 2: Ykstep = 5 * Ystep
  Case Else: Ykstep = 4 * Ystep
  End Select
  If YVl = 0 Then zk = 0 Else zk = Fix(1 + YVl * kft_km! / Ykstep) * Ykstep
  
  For YV = zk To YVr * kft_km! Step Ykstep!
    frmCTC.CurrentX = XVr + xoffset! / 2
    frmCTC.CurrentY = YV / kft_km! - yoffset! / 2
    If YV < 10 Then frmCTC.Print Format$(YV, " 0") Else frmCTC.Print Format$(YV, "##")
  Next YV
  frmCTC.FontBold = True
  temp$ = "kft"
  frmCTC.CurrentX = XVr + frmCTC.TextWidth(temp$) / 2
  frmCTC.CurrentY = YVl + 1.5 * frmCTC.TextHeight(temp$)
  frmCTC.Print temp$
  frmCTC.FontBold = False

' Write x-axis label
  For XV = XVl To XVr Step Xstep
    Select Case frmCTC.Xcoord
    Case 0
      If XV > 24 Then Xvalue = XV - 24 Else Xvalue = XV
    Case Else: Xvalue = XV
    End Select
  
    Select Case Xstep
    Case Is >= 1: temp$ = Format$(Xvalue, "##0")
    Case Is = 0.5: temp$ = Format$(Xvalue, "###.0")
    Case Else: temp$ = Format$(Xvalue, "###.00")
    End Select
    frmCTC.CurrentX = XV - frmCTC.TextWidth(temp$) / 2:
    frmCTC.CurrentY = YVl + yoffset! / 2
    frmCTC.Print temp$
  Next XV

  frmCTC.FONTSIZE = frmCTC.FONTSIZE + 1
  frmCTC.FontBold = True

  Select Case Xcoord
  Case 0:  temp$ = "Universal Time [hr]"
  Case 1:  temp$ = "Latitude [deg]"
  Case 2:  temp$ = "Longitude [deg]"
  Case 3:  temp$ = "Universal Time [ks]"
  End Select

'  If PlotPalt Then x$ = "Ordinate: pALT  " Else x$ = "Ordinate: gALT  "
'  x$ = x$ + ""
'  If Mission$ = "TexAQS" Then
'    temp$ = x$ + "          " + temp$ + "           " + "Electra" + " Flight No. " + Format(Val(Right$(FltNumber$, 2)), "00")
'  Else
'    temp$ = x$ + temp$ + "              " + Platform$ + " Flight No. " + Format(Val(Right$(FltNumber$, 2)), "00")
'  End If
  frmCTC.CurrentX = XVl + XVs / 2 - frmCTC.TextWidth(temp$) / 2
  frmCTC.CurrentY = YVl + 1.5 * frmCTC.TextHeight(temp$)
  frmCTC.Print temp$

  frmCTC.FontBold = False
  frmCTC.FONTSIZE = frmCTC.FONTSIZE - 1
  y = YVl + 4 * frmCTC.TextHeight(temp$)
  TSfmt$ = "yyyy mm dd hh:mm:ss"
  CTCstamp = Date + Time
  x$ = "History:  "
  x$ = x$ + "  Flight: " + Format$(RAWstamp, TSfmt$) + "   Retrieved: " + Format$(REFstamp, TSfmt$)
  x$ = x$ + "   Editted: " + Format$(ERFstamp, TSfmt$) + "  Plotted: " + Format$(CTCstamp, TSfmt$)

  CurrentX = XVl - Bl + (XVr + Br - XVl + Bl - TextWidth(x$)) / 2
  CurrentY = YVl - 0.8 * Bb
  If ShowDocument Then frmCTC.Print x$

  If Asc(Left$(Pi$, 1)) = 0 Then Pi$ = "MJ Mahoney (Michael.J.Mahoney@jpl.nasa.gov)"
  Pi$ = "MJ Mahoney (Michael.J.Mahoney@jpl.nasa.gov)"
  x$ = "Principal Investigator: " + Trim(Pi$)
  If Preliminary Then x$ = "PRELIMINARY DATA     " + x$ + "     PRELIMINARY DATA"
  CurrentX = XVl - Bl + (XVr + Br - XVl + Bl - TextWidth(x$)) / 2
  CurrentY = YVl - 0.6 * Bb
  If ShowDocument Then frmCTC.Print x$

End Sub

Private Function fRotateText(ByVal theText As String, ByVal theFontname As String, ByVal theSize As Integer, ByVal theAngle As Single, ByVal theX As Single, ByVal theY As Single) As Boolean

'Name:      fText90D
'Author:    Dalin Nie
'Date:      2/18/98
'Purpose:   Print a 90 degree text in the form
'Pass In:   TheText-- the text you want to print
'           theSize:   Size of the font
'           theAngle:  How many degree's you want to rotate
'           theX, theY, Where you wnat your text
'Return:    Boolean


  On Error GoTo Trap
  Dim f As LOGFONT, hPrevFont As Long, hFont As Long
  Dim FONTSIZE As Integer
  FONTSIZE = theSize

  f.lfEscapement = 10 * theAngle 'rotation angle, in tenths
  'theFontname = "Arial" + Chr$(0) 'null terminated
  f.lfFacename = theFontname + Chr$(0)
  f.lfHeight = (theSize * -20) / Screen.TwipsPerPixelY
  hFont = CreateFontIndirect(f)
  hPrevFont = SelectObject(Me.hDC, hFont)
  frmCTC.CurrentX = theX
  frmCTC.CurrentY = theY
  frmCTC.Print theText
  
'  Clean up, restore original font
  hFont = SelectObject(Me.hDC, hPrevFont)
  DeleteObject hFont
  fRotateText = True
  
  Exit Function

Trap:

End Function

Private Sub Form_Click()

  frmCTC.WindowState = (frmCTC.WindowState + 1) Mod 3

End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub


Private Sub Form_Load()
Dim i%

  XVlast = XVvalue
  Curtain_Plot_Set
  Speed% = 2
  ReLabel = True
  SamplesAveraged = 0
  zt1last = 99.9
  zt2last = 99.9
  Zt1AVG = 0#: Zt2Avg = 0#: AltAvg = 0#: MRIavg1 = 0#: ZcpAVG = 0#
  Zt1Cnt = 0: Zt2Cnt = 0: AltCnt = 0: MRIcnt = 0: ZcpCnt = 0
  If XVl < XVr Then XVend = XVvalue + XunitsPerPixel Else XVend = XVvalue - XunitsPerPixel
  For i = 1 To 33: RAavg(i) = 0: RTavg(i) = 0:  Next
  tmrCurtainAutoStep.Interval = CurtainAutoStepInterval
  tmrCurtainAutoStep.Enabled = True
  CurtainPlotUpdate = False   'True if Curtain plot just done
  Record% = 1
  
  Curtaininitialize = True

  Picture = LoadPicture("c:\mtp\VB6\DIB\rainbow.dib")  ' Load bitmap.

  Show
  With frmCTC
  .Top = 0
  .Left = 0
  .Width = 800 * Screen.TwipsPerPixelX
  .Height = 600 * Screen.TwipsPerPixelY
  End With

End Sub











Private Sub mnuExit_Click()

End Sub

Private Sub tmrCurtainAutoStep_Timer()

  tmrCurtainAutoStep.Enabled = False  'Disable during plot update
  If RequestATPupdate And CurtainPlotUpdate Then RequestATPupdate = False

  CurtainPlotUpdate = False   'True if Curtain plot just done
' Check if caught up and slow down if so
  If ReadREF Then
    FileRecords = LOF(CTClu%) \ Len(REF) - HiddenRecords
  Else
    FileRecords = 10000
  End If
  
  If FileRecords - Record > 0 Then
    CurtainAutoStepInterval = 1     'Data exists, wait 0.1 seconds
    tmrCurtainAutoStep.Interval = CurtainAutoStepInterval
    GetREFcycle (True)                'Only if data exists
  Else
    If DoAll Then
      mnuRunClose_Click
    Else                              'Wait for more data if Realtime mode
      CurtainAutoStepInterval = 300  'No more data, wait 3 seconds
      tmrCurtainAutoStep.Interval = CurtainAutoStepInterval
    End If
  End If
  
  If CurtainFileOpen Then tmrCurtainAutoStep.Enabled = True   'Then turn back on

End Sub


