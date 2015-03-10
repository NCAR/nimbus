VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Winsock UDP Chat"
   ClientHeight    =   6750
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5205
   Icon            =   "Chat_frmMain.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6750
   ScaleWidth      =   5205
   StartUpPosition =   2  'CenterScreen
   Begin ComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   285
      Left            =   0
      TabIndex        =   12
      Top             =   6465
      Width           =   5205
      _ExtentX        =   9181
      _ExtentY        =   503
      SimpleText      =   ""
      _Version        =   327682
      BeginProperty Panels {0713E89E-850A-101B-AFC0-4210102A8DA7} 
         NumPanels       =   2
         BeginProperty Panel1 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   2
            Object.Width           =   2566
            Text            =   "  No connection...  "
            TextSave        =   "  No connection...  "
            Key             =   "STATUS"
            Object.Tag             =   ""
            Object.ToolTipText     =   "The current status of the connection"
         EndProperty
         BeginProperty Panel2 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   1
            Object.Width           =   6535
            Key             =   "DATA"
            Object.Tag             =   ""
            Object.ToolTipText     =   "The last data transfer through the modem"
         EndProperty
      EndProperty
   End
   Begin VB.CommandButton cmdClear 
      Caption         =   "Clear"
      Height          =   285
      Left            =   90
      TabIndex        =   11
      Top             =   6090
      Width           =   1545
   End
   Begin VB.TextBox txtRemotePort 
      BackColor       =   &H00C0C0C0&
      Height          =   285
      Left            =   2010
      TabIndex        =   3
      Text            =   "10200"
      Top             =   720
      Width           =   1665
   End
   Begin VB.TextBox txtLocalPort 
      BackColor       =   &H00C0C0C0&
      Height          =   285
      Left            =   2040
      TabIndex        =   2
      Text            =   "10200"
      Top             =   450
      Width           =   1635
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "Connect"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   13.5
         Charset         =   161
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   885
      Left            =   3810
      TabIndex        =   8
      Top             =   120
      Width           =   1365
   End
   Begin VB.TextBox txtRemoteIP 
      BackColor       =   &H00C0C0C0&
      Height          =   285
      Left            =   2010
      TabIndex        =   1
      Text            =   "10.3.1.255"
      Top             =   120
      Width           =   1665
   End
   Begin VB.Frame Frame2 
      Caption         =   "Host IP"
      Enabled         =   0   'False
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   161
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2415
      Left            =   90
      TabIndex        =   6
      Top             =   3600
      Width           =   5025
      Begin VB.TextBox Text2 
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   12
            Charset         =   161
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00400000&
         Height          =   2115
         Left            =   120
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   5
         Top             =   210
         Width           =   4785
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Remote IP"
      Enabled         =   0   'False
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   161
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2325
      Left            =   90
      TabIndex        =   0
      Top             =   1260
      Width           =   5025
      Begin VB.TextBox Text1 
         BackColor       =   &H00E0E0E0&
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   9.75
            Charset         =   161
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H000000FF&
         Height          =   2025
         Left            =   120
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   4
         Top             =   210
         Width           =   4785
      End
   End
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   1830
      Top             =   6060
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      Caption         =   "Type your text and hit Enter to send it."
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   161
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00008000&
      Height          =   195
      Left            =   1200
      TabIndex        =   13
      Top             =   1050
      Visible         =   0   'False
      Width           =   2805
   End
   Begin VB.Label Label3 
      BorderStyle     =   1  'Fixed Single
      Caption         =   " Remote Port :"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   161
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   285
      Left            =   90
      TabIndex        =   10
      Top             =   720
      Width           =   1905
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BorderStyle     =   1  'Fixed Single
      Caption         =   " Local Port :"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   161
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   285
      Left            =   90
      TabIndex        =   9
      Top             =   420
      Width           =   1905
   End
   Begin VB.Label Label1 
      BorderStyle     =   1  'Fixed Single
      Caption         =   " Connect with IP :"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   161
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   285
      Left            =   90
      TabIndex        =   7
      Top             =   120
      Width           =   1905
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private IgnoreText As Boolean

Private Sub cmdClear_Click()
On Error Resume Next
Text1 = ""
With Text2
   'Clear the text window and
   .Text = " "
   'return the focus
   .SetFocus
End With
End Sub

Private Sub cmdConnect_Click()
On Error GoTo ErrHandler

With Winsock1
   'Set the remotehost property
   .RemoteHost = Trim(txtRemoteIP)
   'Set the remoteport property.
   'This should be equal to the
   'localhost property of the
   'remote machine.
   .RemotePort = Trim(txtRemotePort)
   'The localport property cannot be
   'changed,so check if it has already
   'been set.
   If .LocalPort = Empty Then
      .LocalPort = Trim(txtLocalPort)
      Frame2.Caption = .LocalIP
      .Bind .LocalPort
   End If
End With

'Make sure that the user can't change
'the local port
txtLocalPort.Locked = True
'Show the current status of the connection in
'the status bar
StatusBar1.Panels(1).Text = "  Connected to " & Winsock1.RemoteHost & "  "

Frame1.Enabled = True
Frame2.Enabled = True
Label4.Visible = True

Text2.SetFocus
Exit Sub

ErrHandler:
MsgBox "Winsock failed to establish connection with remote server", vbCritical
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = vbKeyF1 Then
ChDir App.Path
Shell "notepad.exe readme.txt", vbNormalFocus
End If

End Sub

Private Sub Form_Load()
Show
'MsgBox "Winsock UDT Chat" & vbCrLf & "by Theo Kandiliotis (ionikh@hol.gr)" & vbCrLf & vbCrLf & "F1 for help.", vbInformation
'txtRemoteIP = Winsock1.LocalIP
End Sub

Private Sub Text2_KeyPress(KeyAscii As Integer)

'The position of the last linefeed within the text
Static Last_Line_Feed As Long

'The new line of text
Dim New_Line As String
'Reset the position of the last line feed if the
'user has clear the chat window
If Trim(Text2) = vbNullString Then Last_Line_Feed = 0
'If the user pressed Enter...
If KeyAscii = 13 Then
   'Get the new line of text
   New_Line = Mid(Text2, Last_Line_Feed + 1)
   'Save the position of the current linefeed
   Last_Line_Feed = Text2.SelStart
   'Send the new text across the socket
   Winsock1.SendData New_Line
   StatusBar1.Panels(2).Text = "  Sent " & (LenB(New_Line) / 2) & " bytes  "
End If

End Sub

Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
'New_Text is the text that has just arrived
'from across the socket
Dim New_Text As String
'Get the new text
Winsock1.GetData New_Text
'Show the new text
Text1.SelText = New_Text
Frame1.Caption = Winsock1.RemoteHostIP
'Show the byte size of this transmission in the statusbar
StatusBar1.Panels(2).Text = "  Received " & bytesTotal & " bytes  "
End Sub

