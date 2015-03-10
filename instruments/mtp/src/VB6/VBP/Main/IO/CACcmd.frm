VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{FE0065C0-1B7B-11CF-9D53-00AA003C9CB6}#1.1#0"; "COMCT232.OCX"
Begin VB.Form frmCACcmd 
   Caption         =   "CAC Command and IO"
   ClientHeight    =   3765
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   12975
   LinkTopic       =   "Form1"
   ScaleHeight     =   3765
   ScaleWidth      =   12975
   StartUpPosition =   3  'Windows Default
   Begin TabDlg.SSTab SSTab1 
      Height          =   3315
      Left            =   30
      TabIndex        =   4
      Top             =   0
      Width           =   12915
      _ExtentX        =   22781
      _ExtentY        =   5847
      _Version        =   393216
      Tabs            =   2
      Tab             =   1
      TabsPerRow      =   2
      TabHeight       =   520
      TabCaption(0)   =   "Real Time Data Mode"
      TabPicture(0)   =   "CACcmd.frx":0000
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "lstDifference"
      Tab(0).Control(1)=   "lstOUT"
      Tab(0).ControlCount=   2
      TabCaption(1)   =   "Command Mode"
      TabPicture(1)   =   "CACcmd.frx":001C
      Tab(1).ControlEnabled=   -1  'True
      Tab(1).Control(0)=   "Label(2)"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "Label1"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "Label(1)"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "Label(3)"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "UpDown1"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).Control(5)=   "chkAutoSend"
      Tab(1).Control(5).Enabled=   0   'False
      Tab(1).Control(6)=   "Text1"
      Tab(1).Control(6).Enabled=   0   'False
      Tab(1).Control(7)=   "cmbFooter"
      Tab(1).Control(7).Enabled=   0   'False
      Tab(1).Control(8)=   "cmbScale"
      Tab(1).Control(8).Enabled=   0   'False
      Tab(1).Control(9)=   "Frame1"
      Tab(1).Control(9).Enabled=   0   'False
      Tab(1).Control(10)=   "cmdScroll(4)"
      Tab(1).Control(10).Enabled=   0   'False
      Tab(1).Control(11)=   "cmdScroll(3)"
      Tab(1).Control(11).Enabled=   0   'False
      Tab(1).Control(12)=   "cmdScroll(2)"
      Tab(1).Control(12).Enabled=   0   'False
      Tab(1).Control(13)=   "cmdScroll(1)"
      Tab(1).Control(13).Enabled=   0   'False
      Tab(1).Control(14)=   "cmdScroll(0)"
      Tab(1).Control(14).Enabled=   0   'False
      Tab(1).Control(15)=   "cmdSend"
      Tab(1).Control(15).Enabled=   0   'False
      Tab(1).Control(16)=   "cmdTerminateRemote"
      Tab(1).Control(16).Enabled=   0   'False
      Tab(1).Control(17)=   "cboPlatform"
      Tab(1).Control(17).Enabled=   0   'False
      Tab(1).ControlCount=   18
      Begin VB.ComboBox cboPlatform 
         Height          =   315
         Left            =   690
         TabIndex        =   29
         Text            =   "Combo1"
         Top             =   1620
         Width           =   735
      End
      Begin VB.CommandButton cmdTerminateRemote 
         Caption         =   "Terminate Remote"
         Height          =   375
         Left            =   360
         TabIndex        =   28
         Top             =   2640
         Width           =   1695
      End
      Begin VB.ListBox lstDifference 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1110
         ItemData        =   "CACcmd.frx":0038
         Left            =   -74940
         List            =   "CACcmd.frx":003F
         TabIndex        =   27
         Top             =   2070
         Width           =   12765
      End
      Begin VB.CommandButton cmdSend 
         Caption         =   "&Send"
         Height          =   375
         Left            =   10680
         TabIndex        =   26
         Top             =   2460
         Width           =   975
      End
      Begin VB.ListBox lstOUT 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1530
         ItemData        =   "CACcmd.frx":004F
         Left            =   -74940
         List            =   "CACcmd.frx":0056
         TabIndex        =   25
         Top             =   360
         Width           =   12765
      End
      Begin VB.CommandButton cmdScroll 
         Caption         =   "&Home"
         Height          =   375
         Index           =   0
         Left            =   2130
         TabIndex        =   21
         Top             =   780
         Width           =   735
      End
      Begin VB.CommandButton cmdScroll 
         Caption         =   "&Up"
         Height          =   375
         Index           =   1
         Left            =   2130
         TabIndex        =   20
         Top             =   390
         Width           =   735
      End
      Begin VB.CommandButton cmdScroll 
         Caption         =   "&Down"
         Height          =   375
         Index           =   2
         Left            =   2130
         TabIndex        =   19
         Top             =   1170
         Width           =   735
      End
      Begin VB.CommandButton cmdScroll 
         Caption         =   "&Left"
         Height          =   375
         Index           =   3
         Left            =   1380
         TabIndex        =   18
         Top             =   780
         Width           =   735
      End
      Begin VB.CommandButton cmdScroll 
         Caption         =   "&Right"
         Height          =   375
         Index           =   4
         Left            =   2880
         TabIndex        =   17
         Top             =   780
         Width           =   735
      End
      Begin VB.Frame Frame1 
         Caption         =   "Toggle"
         Height          =   1305
         Left            =   3690
         TabIndex        =   10
         Top             =   360
         Width           =   2355
         Begin VB.CommandButton cmdToggle 
            Caption         =   "&Ghost"
            Height          =   315
            Index           =   1
            Left            =   120
            TabIndex        =   16
            Top             =   600
            Width           =   1065
         End
         Begin VB.CommandButton cmdToggle 
            Caption         =   "&Lapse Rate"
            Height          =   315
            Index           =   2
            Left            =   120
            TabIndex        =   15
            Top             =   930
            Width           =   1065
         End
         Begin VB.CommandButton cmdToggle 
            Caption         =   "&Trop Info"
            Height          =   345
            Index           =   3
            Left            =   1170
            TabIndex        =   14
            Top             =   240
            Width           =   1095
         End
         Begin VB.CommandButton cmdToggle 
            Caption         =   "&Paint"
            Height          =   345
            Index           =   0
            Left            =   120
            TabIndex        =   13
            Top             =   240
            Width           =   1065
         End
         Begin VB.CommandButton cmdToggle 
            Caption         =   "&Status"
            Height          =   315
            Index           =   4
            Left            =   1200
            TabIndex        =   12
            Top             =   600
            Width           =   1065
         End
         Begin VB.CommandButton cmdToggle 
            Caption         =   "Theta"
            Height          =   315
            Index           =   5
            Left            =   1200
            TabIndex        =   11
            Top             =   930
            Width           =   1065
         End
      End
      Begin VB.ComboBox cmbScale 
         Height          =   315
         Left            =   690
         TabIndex        =   9
         Text            =   "1"
         Top             =   1230
         Width           =   495
      End
      Begin VB.ComboBox cmbFooter 
         Height          =   315
         Left            =   7200
         TabIndex        =   8
         Top             =   720
         Width           =   2535
      End
      Begin VB.TextBox Text1 
         Height          =   285
         Left            =   690
         TabIndex        =   6
         Top             =   750
         Width           =   375
      End
      Begin VB.CheckBox chkAutoSend 
         Caption         =   "AutoSend"
         Height          =   255
         Left            =   90
         TabIndex        =   5
         Top             =   390
         Width           =   1095
      End
      Begin ComCtl2.UpDown UpDown1 
         Height          =   375
         Left            =   930
         TabIndex        =   7
         Top             =   750
         Width           =   240
         _ExtentX        =   423
         _ExtentY        =   661
         _Version        =   327681
         Enabled         =   -1  'True
      End
      Begin VB.Label Label 
         Caption         =   "Platform"
         Height          =   255
         Index           =   3
         Left            =   90
         TabIndex        =   30
         Top             =   1650
         Width           =   615
      End
      Begin VB.Label Label 
         Caption         =   "Scale"
         Height          =   255
         Index           =   1
         Left            =   90
         TabIndex        =   24
         Top             =   1230
         Width           =   495
      End
      Begin VB.Label Label1 
         Caption         =   "Footer Line"
         Height          =   255
         Left            =   6360
         TabIndex        =   23
         Top             =   720
         Width           =   855
      End
      Begin VB.Label Label 
         Caption         =   "MRI"
         Height          =   255
         Index           =   2
         Left            =   90
         TabIndex        =   22
         Top             =   750
         Width           =   375
      End
   End
   Begin VB.Timer tmrRealTime 
      Left            =   3510
      Top             =   3330
   End
   Begin VB.CommandButton cmdExit 
      Caption         =   "E&xit"
      Height          =   375
      Left            =   11940
      TabIndex        =   3
      Top             =   3390
      Width           =   1035
   End
   Begin VB.CommandButton cmdGetRaw 
      Caption         =   "Get RAW"
      Height          =   375
      Left            =   3510
      TabIndex        =   2
      Top             =   3360
      Width           =   1035
   End
   Begin VB.DriveListBox Drive1 
      Height          =   315
      Left            =   960
      TabIndex        =   1
      Top             =   3360
      Width           =   2535
   End
   Begin VB.Label Label 
      Caption         =   "CAC Drive"
      Height          =   255
      Index           =   0
      Left            =   60
      TabIndex        =   0
      Top             =   3390
      Width           =   855
   End
End
Attribute VB_Name = "frmCACcmd"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim CACdrive$, L%, kb$
Public CurrentStamp As Date 'Time of current real time record
Public LastStamp As Date    'Time of last real time record

Sub FindDifference(a$, b$, c$, i%)
Dim na%, nb%, nc%

  If b$ = "" Then b$ = a$  'Handle first pass
  Select Case i
  Case 1
'AD106:22:32:55 +34.480 -136.900  013 +00.0 +00.0 01100 01100 +17.1 0110   0270 +180.0 1 000 041129 223255
    c$ = b$
    
  Case 2, 3
'B 07317 06788 06536 07334 06805 06559 07320 06788 06545 07323 06787 06538 07316 06767 06528
'C 07318 06774 06531 07318 06775 06541 07323 06773 06541 07315 06777 06520 07306 06766 06523
' 123456123456
    If i = 2 Then c$ = "B" Else c$ = "C"
    For i = 1 To 15
      na = Val(Mid$(a$, (i - 1) * 6 + 2, 6))
      nb = Val(Mid$(b$, (i - 1) * 6 + 2, 6))
      nc = na - nb
      c$ = c$ + fInteger(nc, 6)
    Next i
  
  Case 4
'     Tnd   Tamp   Tsyn   Tmix  Ttgt1  Ttgt2   Acc+   Twin   Tmot  Spare   Vref  Tdat1  Tdat2  Vcc/2  15V/n Tplate   Acc-
'd +40.58 +46.92 +32.80 +41.23 +26.24 +26.58 +01.02 +07.19 +36.48 +01.96 +02.04 +34.33 +40.81 +02.48 +01.90 +41.67 +01.02
' 12345671234567
    c$ = "D"
    For i = 1 To 17
      na = Val(Mid$(a$, (i - 1) * 7 + 2, 6))
      nb = Val(Mid$(b$, (i - 1) * 7 + 2, 6))
      nc = na - nb
      If nc < 0 Then
        c$ = c$ + " " + Format(nc, "-00.00")
      Else
        c$ = c$ + " " + Format(nc, "+00.00")
      End If
    Next i

  Case 6
'E 13825 12345 09650 07339 06812 06567
    c$ = "E"
    For i = 1 To 6
      na = Val(Mid$(a$, (i - 1) * 6 + 2, 6))
      nb = Val(Mid$(b$, (i - 1) * 6 + 2, 6))
      nc = na - nb
      c$ = c$ + fInteger(nc, 6)
    Next i

  End Select

End Sub

Function fInteger$(n%, count%)
Dim AbsN$, StrN$, Nspace%

  AbsN$ = Trim(Abs(n))
  StrN$ = Trim(Str(AbsN))
  Nspace = count - Len(AbsN)
  If n < 0 Then
    fInteger$ = Space(Nspace - 1) + "-" + StrN$
  Else
    fInteger$ = Space(Nspace - 1) + "+" + StrN$
  End If
  
End Function


Sub main()
Dim kb$, Drive$, CMDlu%, L%, d$

ProgramStart:

' Check for Valid Commands
  Select Case L%
  Case 1
    Select Case Asc(kb$)
    Case 27: Stop
    Case 2: cmd$ = "Ctrl+B: Toggle bottom display line"
    Case 3: cmd$ = "Ctrl+C: Toggle control of display scaling"
    Case 7: cmd$ = "Ctrl+G: Toggle Ghost display"
    Case 8: cmd$ = "Ctrl+H: Toggle Trop Information"
    Case 9: cmd$ = "Ctrl+I: Toggle Status box"
    Case 12: cmd$ = "Ctrl+L: Toggle Lapse Rate lines"
    Case 16: cmd$ = "Ctrl+P: Toggle painted PSC regions"
    Case 17: cmd$ = "Ctrl+Q: Close files and quit!"
    Case 18: cmd$ = "Ctrl+R: Reset everything"
    Case 20: cmd$ = "Ctrl+T: Toggle potential temperature display"
    Case 24: cmd$ = "Ctrl+X: Toggle Risk COM1 input"
    Case 26: cmd$ = "Ctrl+Z: Toggle global minimum data"
    Case Asc("&"): cmd$ = ""
    Case Asc("1"): cmd$ = "Display scale 1 (largest)"
    Case Asc("2"): cmd$ = "Display scale 2"
    Case Asc("3"): cmd$ = "Display scale 3 (smallest)"
    Case Asc("+"): cmd$ = "Increase RC region"
    Case Asc("-"): cmd$ = "Decrease RC region"
    Case Else
        Print "Illegal instruction!"
        GoTo ProgramStart
    End Select
  Case 2
    Select Case Asc(kb$)
    Case 75: cmd$ = "Left Arrow"
    Case 77: cmd$ = "Right Arrow"
    Case 72: cmd$ = "Up Arrow"
    Case 80: cmd$ = "Down Arrow"
    Case 71: cmd$ = "Home = Scale 1"
    Case Else
        Print "Illegal instruction!"
        GoTo ProgramStart
    End Select
  End Select               'Finished checking legal commands

' Write command to: COMMAND.TXT and screen
  CMDlu% = FreeFile
  Open CACdrive$ + "\DC8\CAC\COMMAND.TXT" For Output As #CMDlu%
  Print #CMDlu%, USING; "# ###"; L%; Asc(kb$)
  Print USING; "# ### = " + cmd$; L%; Asc(kb$)
  Close #CMDlu%
  GoTo ProgramStart
 
End Sub

Sub SendCommand()

End Sub


Private Sub cmdExit_Click()

  Unload Me

End Sub

Private Sub cmdGetRaw_Click()
Dim CMDlu%

  tmrRealTime.Enabled = True
  tmrRealTime.Interval = 2000

End Sub

Private Sub cmdTerminateRemote_Click()
Dim CMDlu%

  CMDlu = FreeFile
  Open CACdrive$ + "\MTP\RTA\Command.TXT" For Output As CMDlu
'  Open "c:\MTP\RTA\Command.TXT" For Output As CMDlu
  Print #CMDlu, "Exit"
  Print #CMDlu, ""
  Close CMDlu
  
End Sub

Private Sub tmrRealTime_Timer()
Dim RAWlu%, OUTlu%, INPlu%, b$(1 To 6), c$, i%, n%, yyyymmdd$, hhmmss$
Static a$(1 To 8), Init As Boolean, RAWfile$, Gottem As Boolean, x!, L%

  On Error GoTo ErrorHandler
  tmrRealTime.Enabled = False
  If Not Init Then
    yyyymmdd$ = Right$(Date$, 4) + Format(Left$(Date$, 2), "00") + Mid$(Date$, 4, 2)
    RAWfile$ = "C:\MTP\RTA\RAW\RT" + yyyymmdd$ + ".RAW"
    If cboPlatform.Text = "DC8" Then L = 6 Else L = 5
    Init = True
  End If
  CurrentStamp = FileDateTime(CACdrive$ + "\MTP\RTA\RAW\CURRENT.RAW") ' Returns "2/12/93 4:35:47 PM".
  If CurrentStamp > LastStamp Then
    For i = 1 To 6
      b$(i) = a$(i)
    Next i
  ' Get a complete set of A-E lines
    Do
      INPlu% = FreeFile
      Open CACdrive$ + "\MTP\RTA\RAW\CURRENT.RAW" For Input As #INPlu%
      i = 0
      Gottem = False
      Do
        i = i + 1
        Line Input #INPlu, a$(i)
        If Left$(a$(1), 1) <> "A" Then i = i - 1  'Instrument start
        Debug.Print a$(i)
      Loop Until EOF(INPlu)
      Close INPlu
      If L = 5 Then
        a$(6) = a$(5)
        a$(5) = a$(4)
        a$(4) = "d +40.58 +46.92 +32.80 +41.23 +26.24 +26.58 +01.02 +07.19 +36.48 +01.96 +02.04 +34.33 +40.81 +02.48 +01.90 +41.67 +01.02"
      End If
      
      If Left$(a$(1), 1) = "A" And Left$(a$(2), 1) = "B" And Left$(a$(3), 1) = "C" And Left$(a$(4), 1) = "d" And Left$(a$(5), 1) = "D" And Left$(a$(6), 1) = "E" Then Gottem = True
      If Not Gottem Then
        tmrRealTime.Enabled = True  'Wait for file to update
        Exit Sub
      End If
    Loop Until Gottem
    
    OUTlu% = FreeFile
    Open "C:\MTP\RTA\RAW\CURRENT.RAW" For Output As #OUTlu%
    RAWlu% = FreeFile
    Open RAWfile$ For Append As #RAWlu%
    lstOUT.Clear
    lstDifference.Clear
    For i = 1 To 6
      If i = 4 Then
        If L = 6 Then
          Print #OUTlu, a$(i)
          Print #RAWlu, a$(i)
        End If
      Else
        Print #OUTlu, a$(i)
        Print #RAWlu, a$(i)
      End If
      If i = 4 Then
        If L = 6 Then
          lstOUT.AddItem "     Tnd   Tamp   Tsyn   Tmix  Ttgt1  Ttgt2   Acc+   Twin   Tmot  Spare   Vref  Tdat1  Tdat2  Vcc/2  15V/n Tplate   Acc-"
          lstOUT.AddItem a$(i)
        End If
      Else
        lstOUT.AddItem a$(i)
      End If
      If i <> 5 Then  ' is "D" line with raw mux counts
        n = i
        Call FindDifference(a$(i), b$(i), c$, n)
        lstDifference.AddItem c$
        Debug.Print a$(i)
      End If
    Next i
    Close RAWlu, OUTlu
    LastStamp = CurrentStamp
  End If
  tmrRealTime.Enabled = True  'Wait for file to update

ErrorHandler:
  Select Case Error
  Case "File not found"
    MsgBox "Remote computer could not be reached!", vbOKOnly
    Exit Sub
  End Select
End Sub
Private Sub Drive1_Change()
  
  On Error GoTo ErrorHandler
  
  CACdrive$ = Left$(Drive1.List(Drive1.ListIndex), 2)
  LastStamp = FileDateTime(CACdrive$ + "\DC8\RAW\CURRENT.RAW") ' Returns "2/12/93 4:35:47 PM".

ErrorHandler:
  Select Case Error
  Case "File not found"
    MsgBox "Remote computer could not be reached!", vbOKOnly
    Exit Sub
  End Select

End Sub


Private Sub Form_Load()
  
  CACdrive$ = "C:"
  With frmCACcmd
  .Left = (Screen.Width - .Width) / 2
  .Top = (Screen.Height - .Height) / 2
  End With
  
  With cboPlatform
  .AddItem "DC8"
  .AddItem "ER2"
  .AddItem "WB57"
  .AddItem "M55"
  .ListIndex = 1
  End With
  
End Sub



