VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmPointing 
   Caption         =   "MTP Pointing Assessment"
   ClientHeight    =   5010
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7830
   LinkTopic       =   "Form1"
   ScaleHeight     =   5010
   ScaleWidth      =   7830
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtPavg 
      Height          =   285
      Left            =   4005
      TabIndex        =   33
      Text            =   "Pavg:"
      Top             =   3330
      Width           =   3750
   End
   Begin VB.CheckBox chkAmpRoll 
      Caption         =   "Amplify Roll 10x"
      Height          =   195
      Left            =   4995
      TabIndex        =   32
      Top             =   3690
      Width           =   1545
   End
   Begin VB.ListBox lstElev 
      Height          =   645
      ItemData        =   "Pointing.frx":0000
      Left            =   4005
      List            =   "Pointing.frx":0007
      TabIndex        =   31
      Top             =   3960
      Width           =   915
   End
   Begin VB.ComboBox cboPlatform 
      Height          =   315
      Left            =   4005
      TabIndex        =   28
      Text            =   "NGV"
      Top             =   3645
      Width           =   915
   End
   Begin VB.PictureBox picPitch 
      Height          =   3165
      Left            =   450
      ScaleHeight     =   3105
      ScaleWidth      =   3420
      TabIndex        =   22
      Top             =   45
      Width           =   3480
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   7290
      Top             =   3600
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "Open File"
      Height          =   375
      Left            =   45
      TabIndex        =   21
      Top             =   4635
      Width           =   1095
   End
   Begin VB.TextBox txtPath 
      Height          =   285
      Left            =   1170
      TabIndex        =   19
      Text            =   "C:\MTP\Data\GH\ATTREX\Setup\"
      ToolTipText     =   "Default Path is read from the Pointing.INI file in the Pointing folder"
      Top             =   4320
      Width           =   2760
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "Exit"
      Height          =   360
      Left            =   7065
      TabIndex        =   6
      Top             =   4635
      Width           =   735
   End
   Begin VB.ComboBox cboRMSmax 
      Height          =   315
      Left            =   1620
      TabIndex        =   4
      Text            =   "0.015"
      Top             =   3915
      Width           =   885
   End
   Begin VB.ComboBox cboNavg 
      Height          =   315
      Left            =   1890
      TabIndex        =   2
      Text            =   "21"
      Top             =   3600
      Width           =   615
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "Re-calculate Statistics"
      Height          =   405
      Left            =   4950
      TabIndex        =   1
      Top             =   4230
      Width           =   2850
   End
   Begin VB.TextBox txtFile 
      Height          =   330
      Left            =   1140
      TabIndex        =   0
      Text            =   "Clin110830143256.csv"
      Top             =   4665
      Width           =   5850
   End
   Begin VB.Label lblLabel 
      Caption         =   "Elevation Angles"
      Height          =   255
      Index           =   9
      Left            =   2745
      TabIndex        =   30
      Top             =   3960
      Width           =   1260
   End
   Begin VB.Label lblLabel 
      Caption         =   "Platform"
      Height          =   255
      Index           =   8
      Left            =   2745
      TabIndex        =   29
      Top             =   3690
      Width           =   630
   End
   Begin VB.Label lblLabel 
      Alignment       =   1  'Right Justify
      Caption         =   "500"
      Height          =   195
      Index           =   7
      Left            =   3465
      TabIndex        =   27
      Top             =   3240
      Width           =   420
   End
   Begin VB.Label lblLabel 
      Caption         =   "0"
      Height          =   195
      Index           =   6
      Left            =   450
      TabIndex        =   26
      Top             =   3240
      Width           =   150
   End
   Begin VB.Label lblLabel 
      Alignment       =   1  'Right Justify
      Caption         =   "0"
      Height          =   195
      Index           =   5
      Left            =   -45
      TabIndex        =   25
      Top             =   1530
      Width           =   420
   End
   Begin VB.Label lblLabel 
      Caption         =   "+100"
      Height          =   195
      Index           =   4
      Left            =   0
      TabIndex        =   24
      Top             =   0
      Width           =   420
   End
   Begin VB.Label lblLabel 
      Caption         =   "-100"
      Height          =   195
      Index           =   3
      Left            =   45
      TabIndex        =   23
      Top             =   3060
      Width           =   420
   End
   Begin VB.Label lblLabel 
      Caption         =   "Default Path"
      Height          =   240
      Index           =   0
      Left            =   45
      TabIndex        =   20
      Top             =   4320
      Width           =   1050
   End
   Begin VB.Label Label1 
      Caption         =   "El  Pavg     Prms   Pavg-EA N"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Left            =   4050
      TabIndex        =   18
      Top             =   45
      Width           =   3750
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   10
      Left            =   4050
      TabIndex        =   17
      Top             =   3015
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   9
      Left            =   4050
      TabIndex        =   16
      Top             =   2745
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   8
      Left            =   4050
      TabIndex        =   15
      Top             =   2475
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   7
      Left            =   4050
      TabIndex        =   14
      Top             =   2205
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   6
      Left            =   4050
      TabIndex        =   13
      Top             =   1935
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   5
      Left            =   4050
      TabIndex        =   12
      Top             =   1665
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   4
      Left            =   4035
      TabIndex        =   11
      Top             =   1395
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   3
      Left            =   4050
      TabIndex        =   10
      Top             =   1110
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   2
      Left            =   4050
      TabIndex        =   9
      Top             =   825
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   1
      Left            =   4035
      TabIndex        =   8
      Top             =   585
      Width           =   3810
   End
   Begin VB.Label lblResults 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Index           =   0
      Left            =   4050
      TabIndex        =   7
      Top             =   315
      Width           =   3810
   End
   Begin VB.Label lblLabel 
      Caption         =   "Prms Threshold (deg)"
      Height          =   255
      Index           =   2
      Left            =   45
      TabIndex        =   5
      Top             =   3915
      Width           =   1530
   End
   Begin VB.Label lblLabel 
      Caption         =   "Boxcar Samples"
      Height          =   255
      Index           =   1
      Left            =   45
      TabIndex        =   3
      Top             =   3660
      Width           =   1215
   End
End
Attribute VB_Name = "frmPointing"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Const nMax% = 5000

Dim Pitch!(1 To nMax), Roll!(1 To nMax), x!, y!, iMax%, PointingPath$

Sub LoadComboBoxH(C As Control, Name$)
Dim i0%, i1%, i2%, lu%, cMd$, V$, INIfile$
  
  INIfile$ = PointingPath$ + "Elevation.INI"
  
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0
  C.Clear
  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, cMd$                       'Read a line
          If cMd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem cMd$
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
  

End Sub


Function formats(ByVal x!, ByVal f$) As String
' Format a number x using the format f$ so that
' negative numbers have minus sign (they would anyway) and
' positive numbers and zero have a plus sign

  If x < 0 Then
    formats = "-" + Format(Abs(x), f$)
  Else
    formats = "+" + Format(x, f$)
  End If

End Function

Sub AvgRMS(ByRef a!(), ByVal n%, ByRef AVG!, ByRef RMS!)
Dim Sum1!, Sum2!, x!, i%, Sum3!
' Calculate average and rms of n numbers in A()
  If n < 2 Then Exit Sub
  For i = 1 To n
    x = a(i)
    Sum1 = Sum1 + x         'Sum time difference
    Sum2 = Sum2 + x ^ 2     'Sum square of time differences
  Next i
  AVG = Sum1 / n            'Calculate average
'  If Sum2 - AVG ^ 2 * n > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * n) / (n - 1)) Else RMS = 0# 'Calculate RMS
  For i = 1 To n
    Sum3 = Sum3 + (AVG - a(i)) ^ 2
  Next i
  If Sum3 > 0 Then RMS = Sqr(Sum3 / (n - 1)) Else RMS = 0
End Sub


Sub PlotPandR()
Dim Pmin!, Pmax!, Smin!, Smax!, r%, Pstep%, Proll!(1 To nMax)

    If chkAmpRoll.Value = 0 Then
      For r = 1 To iMax: Proll(r) = Roll(r): Next r
    Else
      For r = 1 To iMax: Proll(r) = 10# * Roll(r): Next r
    End If
'   Plot Pitch Data in picture window
    picPitch.Cls
    picPitch.ScaleMode = 0
    picPitch.ScaleTop = 0

' Set sample display scale
    Smin = 0
    picPitch.ScaleLeft = Smin
    picPitch.ScaleWidth = (Int(iMax / 100) + 1) * 100
    Smax = picPitch.ScaleLeft + picPitch.ScaleWidth
    lblLabel(7).Caption = Smax
    
    Pmax = 100
    picPitch.ScaleTop = Pmax
    picPitch.ScaleHeight = -200
    Pmin = picPitch.ScaleTop + picPitch.ScaleHeight
    
'   Draw Pitch/Roll-tic marks
    Pstep = 20
    For i = Pmin To Pmax Step Pstep
      picPitch.PSet (Smin, i)
      picPitch.Line -(Smax, i), QBColor(8)
    Next i
' 00 gray
' 01
' 02
' 03
' 04
' 05 purple
' 06 lgreen
' 07 l gray
' 08 gray
' 09 blue
' 10 green
' 11 cyan
' 12 red
' 13 magenta
' 14 yellow
' 15 white
'   Draw Sample-tic marks
    Pstep = 100
    For i = Smin To Smax Step Pstep
      picPitch.PSet (i, Pmin)
      picPitch.Line -(i, Pmax), QBColor(8)
    Next i

'   Draw data
    picPitch.PSet (1, Pitch(1))
    For r = 2 To iMax
      picPitch.Line -(r, Pitch(r)), QBColor(9)   'And continue from there
    Next r

    picPitch.PSet (1, Proll(1))
    For r = 2 To iMax
      picPitch.Line -(r, Proll(r)), QBColor(10)   'And continue from there
    Next r
'    cmdStart_Click

End Sub

Private Sub chkAmpRoll_Click()
  PlotPandR
End Sub


Private Sub cmdExit_Click()
Unload Me
End Sub

Private Sub cmdOpen_Click()
Dim File$, i%, INPlu%, a$
Dim j%, k%, LineC&, T!

' Set Initial directory
  CommonDialog1.InitDir = txtPath.Text
' Set Size of FileName buffer
  CommonDialog1.MaxFileSize = 1024
' Set filters.
  CommonDialog1.Filter = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*"
' Specify default filter.
  CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
  CommonDialog1.ShowOpen

  File$ = CommonDialog1.FileName

  If File$ = "" Then   ' Avoid opening a file if it is already loaded.
    Exit Sub
  Else
    txtFile.Text = File$
    INPlu = FreeFile
    Open txtFile.Text For Input As #INPlu
'   Read in the pitch and roll measurements
    i = 0

    Do
      j = 1
      Line Input #INPlu, a$
      If Left$(a$, 1) <> "*" Then
        i = i + 1
        j = InStr(j, a$, ",")
        Pitch(i) = Val(Mid$(a$, 1, j - 1))
        k = InStr(j + 1, a$, ",")
        Roll(i) = Val(Mid$(a$, j + 1, k - j - 1))
      End If
    Loop Until EOF(INPlu) Or Left$(a$, 1) = "*"
    Close INPlu
    iMax = i
   PlotPandR
   cmdStart_Click
  End If

End Sub

Private Sub cmdStart_Click()
Dim Pavg!(1 To nMax), Ravg!(1 To nMax), Prms!(1 To nMax), Rrms!(1 To nMax), Result!(1 To nMax)
Dim INPlu%, i%, j%, jMax%, k%, j2%, l%, x!, y!, RMSmax!, Navg%, PA!(1 To nMax), RA!(1 To nMax), AVG!, RMS!
Dim Favg!(1 To 11, 1 To 10), Frms!(1 To 11, 1 To 10), FFavg!(1 To 11), FFrms!(1 To 11)
Dim Navg2%, EA!(0 To 15), i1%, i2%, OUTlu%, lMax%, nResultAll%(1 To 11), ResultAll!(1 To 11, 1 To nMax)
Dim PavgSave!(1 To 10), a$

'Sub AvgRMS(ByRef A!(), ByVal n%, ByRef AVG!, ByRef RMS!)
  EA(0) = 0#
  For i = 1 To 10
    EA(i) = lstElev.List(i - 1)
  Next i
  EA(11) = 0#
  
  Navg = Val(cboNavg.Text)
  Navg2 = Int(Navg / 2)
  RMSmax = Val(cboRMSmax.Text)
' Moving boxcar average over Navg cycles, calculate Pavg and Prms

  For i = 1 To iMax
    For j = 1 To Navg
      PA(j) = Pitch(i + j - 1)
    Next j
    Call AvgRMS(PA(), Navg, AVG, RMS)
    Pavg(i + Navg2) = AVG
    Prms(i + Navg2) = RMS
  Next i

' Flag Pavg values that exceed RMSmax. Put those that don't in Result()
  For i = 1 To iMax - Navg
    If Prms(i + Navg2) < RMSmax Then
      Result(i) = Pavg(i + Navg2)
    Else
      Result(i) = 9999
    End If
  Next i
'
  For k = 1 To 11
    nResultAll(k) = 0
    For j = 1 To 1500
      ResultAll(k, j) = 0#
    Next j
  Next k
  k = 0
  j = 0
  l = 1
  For i = 1 To iMax - Navg
    If Result(i) <> 9999 And Result(i + 1) <> 9999 Then
      j = j + 1
      If j = 1 Then i1 = i Else i2 = i
      PA(j) = Result(i)
    Else
      If j > 10 And k < 12 Then
        k = k + 1
        For j2 = 1 To j
          ResultAll(k, nResultAll(k) + j2) = PA(j2)
        Next j2
        nResultAll(k) = nResultAll(k) + j
        Call AvgRMS(PA(), j, AVG, RMS)
        Favg(k, l) = AVG
        Frms(k, l) = RMS
        If k = 7 Then PavgSave(l) = AVG
        Debug.Print Format(k - 1, "00"); "  "; formats(Favg(k, l), "00.000"); "  "; Format(Frms(k, l), "0.000"); "  "; formats(Favg(k, l) - EA(k - 1), "0.00"); " "; Format(i2 - i1 + 1, "00")
        lblResults(k - 1).Caption = Format(k - 1, "00") + "  " + formats(Favg(k, l), "00.000") + "  " + Format(Frms(k, l), "0.000") + "  " + formats(Favg(k, l) - EA(k - 1), "0.00") + "   " + Format(i2 - i1 + 1, "00")
        'Print #OUTlu, lblResults(k - 1)
        j = 0
        picPitch.PSet (i1, Favg(k, l))
        picPitch.Line -(i2, Favg(k, l)), QBColor(14)
        If k = 11 Then
          k = 0
          l = l + 1
        End If
      End If
    End If
  Next i
  If k = 0 Then lMax = l - 1 Else lMax = l
'  Exit Sub
  
  For k = 1 To 11
  Next k
  
  OUTlu = FreeFile
  Open txtPath.Text + "Pointing.txt" For Output As OUTlu
  Print #OUTlu, txtFile.Text    'Write name of CSV file
  For k = 1 To 11
   For j = 1 To nResultAll(k)
     PA(j) = ResultAll(k, j)
   Next j
   Call AvgRMS(PA(), nResultAll(k), AVG, RMS)
    Debug.Print Format(k - 1, "00"); "  "; formats(AVG, "00.000"); "  "; Format(RMS, "0.000"); "  "; formats(AVG - EA(k - 1), "0.00"); " "; Format(nResultAll(k), "00")
    lblResults(k - 1).Caption = Format(k - 1, "00") + "  " + formats(AVG, "00.000") + "  " + Format(RMS, "0.000") + "  " + formats(AVG - EA(k - 1), "0.00") + "   " + Format(nResultAll(k), "00")
    Print #OUTlu, lblResults(k - 1)
  Next k
  Close OUTlu
  a$ = "Pavg:"
  For l = 1 To lMax
    a$ = a$ + "  " + Format(PavgSave(l), "0.000")
  Next l
  Debug.Print a$
  txtPavg.Text = a$
End Sub


Private Sub Form_Load()
Dim lu%, a$, AC$
  
  PointingPath$ = "C:\MTP\VB6\VBP\Misc\Pointing\"
  
  lu = FreeFile
  Open PointingPath$ + "Pointing.INI" For Input As lu
  Line Input #lu, a$
  txtPath.Text = a$
  Line Input #lu, AC$
  Close lu
  DoEvents
  
  
  LoadComboBoxH cboPlatform, "PLATFORM"  'Sets AC$
  For i = 1 To cboPlatform.ListCount
    If Left$(Trim(cboPlatform.List(i - 1)), 2) = AC$ Then Exit For
  Next i
  cboPlatform.ListIndex = i - 1  'This triggers loading all the other combo boxes

  LoadListBoxH lstElev, "ELEV_" + AC$, 1, 1#

  
  With cboNavg
    For i = 1 To 29 Step 2
    .AddItem Format(i, "0")
    Next i
  End With
  cboNavg.ListIndex = 2
  
  With cboRMSmax
    For i = 10 To 40 Step 2
    .AddItem Format(i * 0.01, "0.000")
    Next i
  End With
  cboRMSmax.ListIndex = 7

  
End Sub

Sub LoadListBoxH(C As Control, Name$, DecimalPts%, UnitScale!)
Dim i0%, i1%, i2%, lu%, cMd$, V$, INIfile$, fmt$
  
  INIfile$ = PointingPath$ + "Elevation.INI"
    
  If Len(Dir(INIfile$)) = 0 Then
    MsgBox "Could not find hardware configuration file " + INIfile$, vbOKOnly
    Exit Sub
  End If
  
  Select Case DecimalPts
  Case 0:: fmt$ = "#0"
  Case 1: fmt$ = "#0.0"
  Case 2: fmt$ = "#0.00"
  Case 3: fmt$ = "#0.000"
  End Select
  
  lu% = FreeFile
  Open INIfile$ For Input As lu%
  FIsize% = 0

  Do
    If EOF(lu%) Then GoTo Exit_Sub
    Input #lu%, cMd$                       'Read a line
    If Left$(cMd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, cMd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(cMd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, cMd$                       'Read a line
          If cMd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem Format$(Val(cMd$) * UnitScale, fmt$)
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
End Sub

