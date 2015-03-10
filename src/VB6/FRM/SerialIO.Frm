VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form frmSerialIO 
   Caption         =   "Serial Port Input and Output"
   ClientHeight    =   2580
   ClientLeft      =   1875
   ClientTop       =   1740
   ClientWidth     =   11505
   Icon            =   "SerialIO.frx":0000
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   2580
   ScaleWidth      =   11505
   Begin VB.Frame Frame3 
      Caption         =   "COM Port"
      Height          =   615
      Left            =   6600
      TabIndex        =   21
      Top             =   1920
      Width           =   1575
      Begin VB.OptionButton Option1 
         Caption         =   "3"
         Height          =   255
         Index           =   2
         Left            =   1080
         TabIndex        =   24
         Top             =   240
         Width           =   375
      End
      Begin VB.OptionButton Option1 
         Caption         =   "2"
         Height          =   255
         Index           =   1
         Left            =   600
         TabIndex        =   23
         Top             =   240
         Width           =   375
      End
      Begin VB.OptionButton optCOM 
         Caption         =   "1"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   22
         Top             =   240
         Width           =   375
      End
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "&Start Input"
      Height          =   495
      Left            =   9960
      TabIndex        =   20
      Top             =   2040
      Width           =   1455
   End
   Begin VB.TextBox txtListSize 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   7680
      MultiLine       =   -1  'True
      TabIndex        =   19
      Top             =   1560
      Width           =   495
   End
   Begin VB.CheckBox chkAutoName 
      Alignment       =   1  'Right Justify
      Caption         =   "Auto Name"
      Height          =   255
      Left            =   8280
      TabIndex        =   17
      Top             =   2040
      Width           =   1095
   End
   Begin VB.CheckBox chkSaveInput 
      Alignment       =   1  'Right Justify
      Caption         =   "Save Input"
      Height          =   375
      Left            =   8280
      TabIndex        =   16
      Top             =   1680
      Width           =   1095
   End
   Begin VB.CheckBox chkLine 
      Alignment       =   1  'Right Justify
      Caption         =   "Line Input"
      Height          =   255
      Left            =   8280
      TabIndex        =   15
      Top             =   1440
      Width           =   1095
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   10080
      Top             =   1800
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Frame Frame2 
      Caption         =   "O/P EOL"
      Height          =   1095
      Left            =   5520
      TabIndex        =   11
      Top             =   1440
      Width           =   975
      Begin VB.OptionButton optEOL 
         Caption         =   "CRLF"
         Height          =   255
         Index           =   2
         Left            =   120
         TabIndex        =   14
         Top             =   720
         Value           =   -1  'True
         Width           =   735
      End
      Begin VB.OptionButton optEOL 
         Caption         =   "CR"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   13
         Top             =   480
         Width           =   615
      End
      Begin VB.OptionButton optEOL 
         Caption         =   "None"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   12
         Top             =   240
         Width           =   735
      End
   End
   Begin VB.TextBox txtRecord 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   10800
      MultiLine       =   -1  'True
      TabIndex        =   10
      Top             =   1440
      Width           =   615
   End
   Begin VB.TextBox txtFilename 
      Height          =   285
      Left            =   960
      TabIndex        =   8
      Text            =   "C:\DC8\RAW\Default.txt"
      Top             =   2160
      Width           =   4455
   End
   Begin VB.Frame Frame1 
      Caption         =   "Delay"
      Height          =   615
      Left            =   120
      TabIndex        =   1
      Top             =   1440
      Width           =   5295
      Begin VB.HScrollBar HScroll1 
         Height          =   255
         LargeChange     =   10
         Left            =   2880
         Max             =   300
         TabIndex        =   5
         Top             =   240
         Value           =   10
         Width           =   2295
      End
      Begin VB.TextBox txtDelay 
         Alignment       =   1  'Right Justify
         Height          =   285
         Left            =   2040
         MultiLine       =   -1  'True
         TabIndex        =   4
         Top             =   240
         Width           =   495
      End
      Begin VB.OptionButton optDelay 
         Caption         =   "Fixed"
         Height          =   255
         Index           =   1
         Left            =   1200
         TabIndex        =   3
         Top             =   240
         Value           =   -1  'True
         Width           =   735
      End
      Begin VB.OptionButton optDelay 
         Caption         =   "Sim DC8"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   2
         Top             =   240
         Width           =   975
      End
      Begin VB.Label Label1 
         Caption         =   "sec"
         Height          =   255
         Left            =   2520
         TabIndex        =   6
         Top             =   240
         Width           =   375
      End
   End
   Begin VB.ListBox List1 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1320
      ItemData        =   "SerialIO.frx":0442
      Left            =   120
      List            =   "SerialIO.frx":0444
      TabIndex        =   0
      Top             =   0
      Width           =   11295
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   9480
      Top             =   1800
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin VB.Label Label 
      Caption         =   "List Size"
      Height          =   255
      Index           =   0
      Left            =   6600
      TabIndex        =   18
      Top             =   1560
      Width           =   615
   End
   Begin VB.Label Label3 
      Caption         =   "Record:"
      Height          =   255
      Left            =   9960
      TabIndex        =   9
      Top             =   1440
      Width           =   615
   End
   Begin VB.Label Label2 
      Caption         =   "Filename:"
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   2160
      Width           =   735
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileOutput 
         Caption         =   "&Output Data"
      End
      Begin VB.Menu mnuFileInput 
         Caption         =   "&Input Data"
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Close"
      End
   End
End
Attribute VB_Name = "frmSerialIO"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public INPlu%, TimeDelay!, Record%
Public FixedDelay As Boolean, ActiveInput As Boolean

Public EOL$, EOLcnt%, OUTlu%, COMport%
Sub InputSerialData()
Dim Line$, DeltaT!, T1!, T2!
Dim L%, L1%, L2%
Dim CRLF As Boolean, InString$
Dim ReadLine As Boolean

If chkLine.Value = 1 Then ReadLine = False Else ReadLine = False

If chkSaveInput.Value = 1 Then
  If chkAutoName.Value = 1 Then
    txtFilename.Text = "C:\DC8\RAW\" + Format(Date, "yyyymmdd") + Format(Time, "hhmm") + ".DC8"
  Else
    txtFilename.Text = "C:\DC8\RAW\Default.DC8"
  End If
  OUTlu = FreeFile
  Open txtFilename.Text For Output As OUTlu
End If
' Use COM1.
MSComm1.CommPort = COMport
' 9600 baud, no parity, 8 data, and 1 stop bit.
MSComm1.Settings = "9600,N,8," + Format(COMport, "0")
' Tell the control to read entire buffer when Input is used.
If ReadLine Then
  MSComm1.InputLen = 0       'Read entire input buffer
Else
  MSComm1.InputLen = 1       'Read single character
End If
' Open the port.
MSComm1.PortOpen = True
' Send the attention command to the modem.

T1 = Timer
T2 = T1
Line$ = ""
InString$ = ""
Record% = 0
'
Do
  DeltaT = T1 - T2
  CRLF = False
  L1 = 1
  
  Line$ = GetSerialIn$
  Record% = Record% + 1
  txtRecord.Text = Str$(Record%)
  DoEvents
  
  If OUTlu <> 0 Then Print #OUTlu, Line$
  
  ScrollList List1
  
  T2 = T1
Loop Until DeltaT > 20 Or OUTlu = 0 'Data has stopped if Close command

Close OUTlu
MSComm1.PortOpen = False  'Close Comm Port

End Sub


Sub ScrollList(lst As Control)
If lst.ListCount > 6 Then lst.TopIndex = lst.ListCount - 6
If lst.ListCount > Val(txtListSize.Text) Then
  lst.RemoveItem 0
End If
End Sub


Sub WaitFor(Secs!)
Dim wft!
wft = Timer
wft0:
    If Timer < wft Then wft = Timer
    If Timer < wft + Secs Then GoTo wft0
    
End Sub

Sub OutputSerialData()
Dim Line$
MSComm1.PortOpen = True
Record% = 0
Do
    Input #INPlu%, Line$
    Record% = Record% + 1
    List1.AddItem Line$
    MSComm1.Output = Line$ + EOL$
    txtRecord.Text = Record%
    DoEvents
    ScrollList List1
    WaitFor (TimeDelay)
Loop Until EOF(INPlu%)

End Sub
Function GetSerialIn$()
Static MTPin$
Dim COM1$, Char$, i%, L%

Do
  If MSComm1.InBufferCount > 0 Then
    COM1$ = MSComm1.Input
    L% = Len(COM1$)
    Select Case MSComm1.InputLen
    Case 0                                                   'Entire buffer read into COM1$
        For i% = 1 To L%                                'Check for CR in buffer
          Char$ = Mid$(COM1$, i%, 1)
          Select Case Char$
          Case vbCr
            GetSerialIn$ = MTPin$
            List1.AddItem MTPin$
            MTPin$ = Right$(COM1$, L% - i% - 1) 'Save leftovers
            Exit Function                                  'And exit
          Case vbLf                                         'Ignore LF
          Case Else
            MTPin$ = MTPin$ + Char$               'No CR, save everything
          End Select
        Next
    Case 1
      Select Case COM1$
      Case vbCr
        GetSerialIn$ = MTPin$
        List1.AddItem MTPin$
        MTPin$ = ""
        Exit Function
      Case vbLf
        ' Ignore linefeeds
      Case Else
        MTPin$ = MTPin$ + COM1$
      End Select
    End Select
    DoEvents
  End If
Loop
End Function


Private Sub cmdStart_Click()

If cmdStart.Caption = "&Start Input" Then
  cmdStart.Caption = "&End Input"
  InputSerialData
Else
  cmdStart.Caption = "&Start Input"
  OUTlu = 0
End If

End Sub

Private Sub Form_DblClick()
CaptureFormMJ Me, "", 3, True
End Sub

Private Sub Form_Load()
frmSerialIO.Top = frmMTPbin.Height
frmSerialIO.Left = 0
chkAutoName.Value = 1
chkSaveInput.Value = 1

txtListSize.Text = "20"
TimeDelay = 2 'sec
HScroll1.Value = TimeDelay * 10
optDelay(1).Value = True
optEOL_Click (2)
optCOM(0).Value = True
frmSerialIO.Enabled = False
End Sub

Private Sub HScroll1_Change()
txtDelay.Text = HScroll1.Value / 10
TimeDelay = txtDelay.Text
End Sub


Private Sub mnuFileClose_Click()
Close INPlu%
' Close the serial port.
If MSComm1.PortOpen = True Then MSComm1.PortOpen = False
txtFilename.Text = ""
End Sub




Private Sub mnuFileInput_Click()
InputSerialData
End Sub

Private Sub mnuFileOutput_Click()
Dim M%, yy_mm_dd$, Msg$, Style%, Title$, Help$, Ctxt%
Dim fqINP$

Dim response%
' Set Initial directory
CommonDialog1.InitDir = "C:\DC8"
' Set filters.
CommonDialog1.Filter = "Raw DC8 Data Files (DC??????.RAW)|*.RAW|All Files (*.*)|*.*"
' Specify default filter.
CommonDialog1.FilterIndex = 1
' Display the File Open dialog box.
CommonDialog1.ShowOpen

fqINP$ = CommonDialog1.Filename
txtFilename.Text = fqINP$
ChDrive Left$(fqINP$, 1)
On Error GoTo MkDirPrompt   'Continue if directory already exits

Carryon:
  On Error GoTo errhandler
  INPlu% = FreeFile
  Open fqINP$ For Input As INPlu%
  On Error GoTo 0

  OutputSerialData
  Exit Sub

MkDirPrompt:
  Msg = "Directory: " & "\DC8\" & fqINP$ & " does not exist; do you want to create it?" ' Define message.
  Style = vbYesNo + vbCritical + vbDefaultButton1 ' Define buttons.
  Title = "WARNING!"  ' Define title.
  Help = "" ' Define Help file.
  Ctxt = 0 ' Define topic
  response = MsgBox(Msg, Style, Title, Help, Ctxt)
  If response = vbYes Then
    MkDir "\DC8\" & fqINP$
    
    GoTo Carryon
  Else
    Stop
    Exit Sub
  End If

errhandler:
  MsgBox "Error encountered while trying to open file, please retry.", 48, "Open File"
  Close INPlu
  Exit Sub
End Sub


Private Sub optCOM_Click(Index As Integer)
COMport = Index + 1
End Sub

Private Sub optDelay_Click(Index As Integer)
Select Case Index
Case 0
  FixedDelay = False
  TimeDelay = 16.6
  txtDelay.Text = TimeDelay
  HScroll1.Value = TimeDelay * 10
Case 1
  FixedDelay = True
  TimeDelay = 2
  txtDelay.Text = TimeDelay
  HScroll1.Value = TimeDelay * 10
End Select

End Sub


Private Sub optEOL_Click(Index As Integer)

Select Case Index
Case 0
  EOL$ = ""
  EOLcnt% = 0
Case 1
  EOL$ = vbCr
  EOLcnt% = 1
Case 2
  EOL$ = vbCrLf
  EOLcnt% = 2
End Select

End Sub


Private Sub txtDelay_Change()
  
  TimeDelay = txtDelay.Text
  HScroll1.Value = TimeDelay * 10

End Sub


