VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form Label 
   Caption         =   "MTPcurrent.RAW"
   ClientHeight    =   3150
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5835
   LinkTopic       =   "Form1"
   ScaleHeight     =   3150
   ScaleWidth      =   5835
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkReOpen 
      Caption         =   "Reopen at EOF"
      Height          =   255
      Left            =   3330
      TabIndex        =   11
      Top             =   2820
      Value           =   1  'Checked
      Width           =   1455
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "Start"
      Height          =   330
      Left            =   4815
      TabIndex        =   10
      Top             =   2790
      Width           =   960
   End
   Begin VB.ComboBox cboPlatforms 
      Height          =   315
      Left            =   2595
      TabIndex        =   9
      Text            =   "NGV"
      Top             =   735
      Width           =   1200
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   75
      Top             =   2715
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.ComboBox cboMTPcurrent 
      Height          =   315
      Left            =   945
      TabIndex        =   6
      Text            =   "C:\MTP\RTA\RAW\"
      Top             =   405
      Width           =   4845
   End
   Begin VB.ListBox lstCurrentRecord 
      Height          =   1620
      ItemData        =   "frmMTPrtsim.frx":0000
      Left            =   60
      List            =   "frmMTPrtsim.frx":0002
      TabIndex        =   5
      Top             =   1110
      Width           =   5730
   End
   Begin VB.TextBox txtCurrentRecord 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   5235
      MultiLine       =   -1  'True
      TabIndex        =   3
      Text            =   "frmMTPrtsim.frx":0004
      Top             =   765
      Width           =   555
   End
   Begin VB.TextBox txtFilename 
      Height          =   285
      Left            =   945
      TabIndex        =   2
      Text            =   "C:\MTP\Data\NGV\NG_TEST\20071218\NG20071218.RAW"
      Top             =   90
      Width           =   4845
   End
   Begin VB.TextBox txtSimTime 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   2160
      MultiLine       =   -1  'True
      TabIndex        =   0
      Text            =   "frmMTPrtsim.frx":0006
      Top             =   750
      Width           =   315
   End
   Begin VB.Timer tmrRealTimeSim 
      Left            =   555
      Top             =   2745
   End
   Begin VB.Label Label 
      Caption         =   "Source File"
      Height          =   195
      Index           =   4
      Left            =   45
      TabIndex        =   8
      Top             =   120
      Width           =   885
   End
   Begin VB.Label Label 
      Caption         =   "MTPcurrent"
      Height          =   195
      Index           =   2
      Left            =   45
      TabIndex        =   7
      Top             =   435
      Width           =   885
   End
   Begin VB.Label Label 
      Caption         =   "Records Written"
      Height          =   225
      Index           =   1
      Left            =   3915
      TabIndex        =   4
      Top             =   810
      Width           =   1245
   End
   Begin VB.Label Label 
      Caption         =   "Write Interval (s)"
      Height          =   195
      Index           =   0
      Left            =   930
      TabIndex        =   1
      Top             =   795
      Width           =   1245
   End
End
Attribute VB_Name = "Label"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public RTSlu%, Rawcycle&, AC$
Sub LoadComboBoxH(C As Control, Name$)
Dim i0%, i1%, i2%, lu%, Cmd$, V$, INIfile$
  
  INIfile$ = "C:\MTP\Setup\Hardware.INI"
  
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
    Input #lu%, Cmd$                       'Read a line
    If Left$(Cmd$, 1) = "[" Then           'Ignore everything until category found
      i0% = InStr(2, Cmd$, "]")
      If i0% = 0 Then
        MsgBox "Missing right bracket not found!", vbOKOnly
        Exit Sub
      End If
      If Mid$(Cmd$, 2, i0% - 2) = Name$ Then
        C.Clear
        Do
          If EOF(lu%) Then GoTo Exit_Sub
          Input #lu%, Cmd$                       'Read a line
          If Cmd$ = "" Then GoTo Exit_Sub
          FIsize% = FIsize% + 1
          C.AddItem Cmd$
        Loop
      End If
    End If
  Loop

Exit_Sub:
  Close (lu%)
  

End Sub

Private Sub cmdStart_Click()
  
  If cmdStart.Caption = "Start" Then
    cmdStart.Caption = "Stop"
    AC$ = Left$(Trim(cboPlatforms.Text), 2)
    tmrRealTimeSim.Interval = Val(txtSimTime.Text) * 1000
    tmrRealTimeSim.Enabled = True
  Else
    Close RTSlu
    Unload Me
    End
  End If

End Sub

Private Sub Form_Load()
Dim Filename$, i%
  
  LoadComboBoxH cboPlatforms, "PLATFORM"

  With cboMTPcurrent
  .AddItem "C:\MTP\RTA\RAW\MTPcurrent.RAW"               'Local computer
  .AddItem "K:\jpl\group\mtp\www\missions\realtime\MTPcurrent.RAW"     'Mapped drive
  .AddItem "ftp://137.79.98.116/home/mahoney/MTPcurrent.RAW"           'FTP Server
  .ListIndex = 0
  End With
  
'   Source$ = "S"  'Realtime Simulation file
'   Open an existing RAW file and use it to write records to the MTPcurrent.RAW file used
'   for real time data analysis. This simulates the real time process of writing to MTPcurrent.RAW
'   Set Initial directory
    CommonDialog1.InitDir = "C:\MTP\Data\"
'   Set Size of FileName buffer
    CommonDialog1.MaxFileSize = 1024
'   Set filters.
    CommonDialog1.Filter = "RAW Files (*.RAW)|*.RAW|All Files (*.*)|*.*"
'   Specify default filter.
    CommonDialog1.FilterIndex = 1
    CommonDialog1.ShowOpen
    Filename$ = CommonDialog1.Filename
    txtFilename.Text = Filename$
    AC$ = Left$(fGetFilename(Filename$), 2)
    For i = 0 To cboPlatforms.ListCount - 1
      If Left$(cboPlatforms.List(i), 2) = AC$ Then Exit For
    Next i
    cboPlatforms.ListIndex = i
    
    RTSlu = FreeFile
    Open Filename$ For Input As RTSlu
    Rawcycle = 0

End Sub
Function fGetFilename(ByVal File$) As String
' Get Filename from last "/" or "\"
Dim i%, j%
  
  fGetFilename = ""
  j = Len(File$)
  i = j
  Do
    i = i - 1
    If Mid$(File$, i, 1) = "\" Or Mid$(File$, i, 1) = "/" Then
      fGetFilename = Mid$(File$, i + 1, Len(File$))
      Exit Function
    End If
  Loop Until i = 1
  
End Function


Private Sub Timer1_Timer()

End Sub


Private Sub tmrRealTimeSim_Timer()
Dim A$, B$, C$, D$, E$, small_d$, IWG1$, pt$, RAWlu%
'WB57, ER2 and M55 Format
'AG037:16:30:34 +9.8492 -83.9317 153.3   6.3    .6  6984  6570 -11.3  8.2  70.6  -183.4 1 128 060206 162826
'B 13694 13566 13694 13669 13523 13668 13649 13516 13674 13625 13487 13650 13607 13477 13643
'C 13580 13466 13641 13543 13450 13636 13505 13428 13631 13464 13419 13623 13403 13392 13621
'D 4B7 4C4 543 47A FFF 9F2 2E8 B6C 4E4 7CA 653 5CA 481 7E3 7E4 24C
'E 15005 15096 15251 14046 13984 14094

'DC8 Format
'Instrument Start at 01-20-2005 19:37:40-JPL Phoenix MTP- v.20021120
'AD020:19:42:55 +34.918 -117.868  106 +00.0 +00.0 02281 02059 +12.0 0000   0000 +180.0 1 000 050120 194430
'B 08672 08137 07723 08671 08135 07719 08671 08140 07729 08676 08134 07718 08668 08115 07724
'C 08672 08122 07728 08699 08161 07750 08723 08195 07790 08738 08206 07778 08727 08202 07784
'd 40.48 + 47.44 + 32.6 + 40.37 + 22.1 + 21.96 + 1.01 + 21.74 + 28.83 + 1.96 + 2.04 + 30.28 + 35.75 + 2.48 + 1.9 + 41.87 + 1.01
'D 528 424 64C 501 CD0 CD2 812 C82 6BA 7AE 7F1 68D 5B7 997 76B 4DB 806
'E 15665 14130 11035 08755 08239 07821

'NGV Format
'A 2007-12-18 23:21:40 002.30 00.05 001.11 00.10 01.30 0.01 267.00 0.20
'IWG1,20071218T232140,14.642,-96.4235,4229.12,,4255.74,4235.87,137.172,134.938,209.011,0.162247,0.0558355,185.039,189.068,4.23184,2.98645,0.90837,0.488137,2.95111,5.49085,-7.03531,14.0805,614.777,72.7016,860.533,8.57255,78.2577,0.0569099,0.821413,0.749384,,
'B 009653 013837 014684 009729 013910 014784 010241 014390 015307 010302 014467 015363 010306 014442 015380 010310 014441 015373 010310 014453 015369 010299 014442 015366 010292 014442 015369 009755 013976 014663
'M01: 2943 2109 2884 3097 3086 2954 2479 2936
'M02: 2101 1247 1429 4095 1531 1191 4095 1060
'Pt: 2436 14145 14143 16383 14187 14163 14028 14768
'E 011467 015347 016344 009836 013969 014870

  tmrRealTimeSim.Enabled = False

  Do
    Line Input #RTSlu, A$
  Loop Until Left$(A$, 1) = "A" Or EOF(RTSlu)
  
  If EOF(RTSlu) Then
    Close RTSlu
    If chkReOpen.Value = 1 Then
      RTSlu = FreeFile
      Open txtFilename.Text For Input As RTSlu
      Rawcycle = 0
      tmrRealTimeSim.Enabled = True
    Else
      tmrRealTimeSim.Enabled = False
    End If
    Exit Sub
  End If
  
  If AC$ = "NG" Then Line Input #RTSlu, IWG1$
  Line Input #RTSlu, B$
  Line Input #RTSlu, C$
  If AC$ = "DC" Then Line Input #RTSlu, small_d$
  Line Input #RTSlu, D$
  If AC$ = "NG" Then Line Input #RTSlu, pt$
  Line Input #RTSlu, E$

  RAWlu = FreeFile
  Open cboMTPcurrent.Text For Output As RAWlu
  lstCurrentRecord.Clear
  Print #RAWlu, A$
  lstCurrentRecord.AddItem A$
  
  If AC$ = "NG" Then
    Print #RAWlu, IWG1$
    lstCurrentRecord.AddItem IWG1$
  End If
  
  Print #RAWlu, B$
  lstCurrentRecord.AddItem B$
  
  Print #RAWlu, C$
  lstCurrentRecord.AddItem C$
  
  If AC$ = "DC" Then
    Print #RAWlu, small_d$
    lstCurrentRecord.AddItem small_d$
  End If
  
  Print #RAWlu, D$
  lstCurrentRecord.AddItem D$
  
  If AC$ = "NG" Then
    Print #RAWlu, pt$
    lstCurrentRecord.AddItem pt$
  End If
  
  Print #RAWlu, E$
  lstCurrentRecord.AddItem E$
  
  Print #RAWlu, ""
  Close RAWlu
  
  Rawcycle = Rawcycle + 1
  txtCurrentRecord = Str(Rawcycle)
  tmrRealTimeSim.Interval = Val(txtSimTime.Text) * 1000
  tmrRealTimeSim.Enabled = True

End Sub


